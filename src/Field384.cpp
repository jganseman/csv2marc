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
    Helper::Trim(data);

    //make all lowercase
    Helper::MakeLowercase(data);

    //segment by ';' to process multiple tonalities
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // LEGACY instrumentation field handling
    if (marcsubfield == 'L')
    {
        std::string reparse = "";
        for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
        {
            // trim beginning and trailing whitespace
            Helper::Trim((*it));
            // if it contains "t=", retain
            if ((*it).find("t=") != std::string::npos)
            {
                reparse += (*it);       // add to a string of all tonalities here
                reparse += " ";         // separated by spaces
            }
        }

        // now make a new segmentation of this string, based on spaces
        datasegments.clear();
        datasegments = Helper::Segment(reparse, ' ');

        marcsubfield = 'a'; // reset marcsubfield to std code for tonalities
    }


    // now, find segments that contains the letters 't='
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        if ((*it).empty())
            continue;

        // replace string "t=" by empty string
        Helper::Remove((*it), "t=");

        // if this still accidentally contains a language, skip it
         if ((*it).find("l=") != std::string::npos)
            continue;

        // trim beginning and trailing whitespace
        Helper::Trim((*it));

        //prepare empty string as new key
        std::string tonality;
        bool flat = false;
        bool sharp = false;
        bool modal = false;
        bool major = true;          // default major
        bool minor = false;

        //first letter is supposed to be the key
        if (!( ((*it)[0] == 'a') || ((*it)[0] == 'b') || ((*it)[0] == 'c') || ((*it)[0] == 'd') || ((*it)[0] == 'e') || ((*it)[0] == 'f') || ((*it)[0] == 'g') ))
        {
            throw MarcRecordException("ERROR Field 384: Unknown Tonality : " + (*it));
        }
        if ((*it)[1] == 'i')     //ais, bis, cis, dis ...
            sharp=true;
        else if (((*it)[1] == 'e') || ((*it)[1] == 's'))      // as, bes, ces, des, es...
            flat=true;

        std::size_t found = (*it).find("gr-t");
        if (found != std::string::npos)
            major=true;

        found = (*it).find("kl-t");
        if (found != std::string::npos)
            minor=true;

        found = (*it).find("mod");
        if (found != std::string::npos)
            modal=true;


        // this builds the string according to regular text

        tonality += ((*it)[0] -32);      // character arithmetic: subtract 32 to make uppercase
        tonality += ' ';
        if (flat) tonality += "flat ";
        if (sharp) tonality += "sharp ";
        if (major) tonality += "major";
        if (minor) tonality += "minor";
        if (modal) tonality += "modal";


        // this builds the string according to the UNIMARC standard, which is more detailed
        // see http://www.ifla.org/files/assets/uca/unimarc_updates/BIBLIOGRAPHIC/u-b_128_update.pdf
        /*
        tonality += (*it)[0];
        if (flat) tonality += "b";
        if (sharp) tonality += "x";
        if (minor) tonality += "m";
        if (modal) tonality = "zz";         // define in more detail later: throw warning after update of this field

        MarcField::update(marcsubfield, tonality);

        if (modal) throw MarcRecordException("WARNING Field 384: Modal Key. To define later in more detail : " + (*it));
        */
    }
}
