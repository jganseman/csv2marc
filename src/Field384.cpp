#include "Field384.h"

Field384::Field384(int nr) : MarcField(nr)
{
    //ctor
}

Field384::~Field384()
{
    //dtor
}


void Field384::update(char marcsubfield, std::string data)
{
    //this field contains the key of the work. Transcode to English here
    if (data.empty() || data == "")
        return;

    // trim beginning and trailing whitespace
    data = data.erase(data.find_last_not_of(" \n\r\t")+1).substr(data.find_first_not_of(" \n\r\t"));

    //make all lowercase
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);

    //prepare empty string as new key
    std::string tonality;
    bool flat = false;
    bool sharp = false;
    bool modal = false;
    bool major = true;          // default major
    bool minor = false;

    //first letter is supposed to be the key
    if (!( (data[0] == 'a') || (data[0] == 'b') || (data[0] == 'c') || (data[0] == 'd') || (data[0] == 'e') || (data[0] == 'f') || (data[0] == 'g') ))
    {
        throw MarcRecordException("ERROR Field 384: Unknown Tonality : " + data);
    }
    if (data[1] == 'i')     //ais, bis, cis, dis ...
        sharp=true;
    else if ((data[1] == 'e') || (data[1] == 's'))      // as, bes, ces, des, es...
        flat=true;

    std::size_t found = data.find("gr-t");
    if (found != std::string::npos)
        major=true;

    found = data.find("kl-t");
    if (found != std::string::npos)
        minor=true;

    found = data.find("mod");
    if (found != std::string::npos)
        modal=true;



    // this builds the string according to regular text
    /*
    tonality += (data[0] -32);      // character arithmetic: subtract 32 to make uppercase
    tonality += ' ';
    if (flat) tonality += "flat ";
    if (sharp) tonality += "sharp ";
    if (major) tonality += "major";
    if (minor) tonality += "minor";
    if (modal) tonality += "modal";
    */


    // this builds the string according to the UNIMARC standard, which is more detailed
    // see http://www.ifla.org/files/assets/uca/unimarc_updates/BIBLIOGRAPHIC/u-b_128_update.pdf
    tonality += data[0];
    if (flat) tonality += "b";
    if (sharp) tonality += "x";
    if (minor) tonality += "m";
    if (modal) tonality = "zz";         // define in more detail later: throw warning after update of this field

    MarcField::update(marcsubfield, tonality);

    if (modal) throw MarcRecordException("WARNING Field 384: Modal Key. To define later in more detail : " + data);
}
