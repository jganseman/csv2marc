#include "Field533.h"

Field533::Field533(int nr) : MarcField(nr)
{
    //ctor
}

Field533::~Field533()
{
    //dtor
}


void Field533::update(char marcsubfield, std::string data)
{
    // this field will be used to parse data from the Varia field.
    // usually formatted as: DIG HD-x ; tiff and jpeg data

    if (data.empty() || data == "")
        return;

    // ignore all data not starting with DIG or HD
    Helper::Trim(data);
    std::string start = data.substr(0,3);
    Helper::MakeUppercase(start);
    if (start != "DIG" && start != "HD-" && start != "HD " )
        return;

    // put all data in capital letters
    Helper::MakeUppercase(data);
    // remove DIG qualifier
    Helper::ReplaceAll(data, "DIG", "");

    // trim again. if nothing left, quit here
    Helper::Trim(data);
    if (data.empty() || data == "")
        return;

    // find the HD qualifier
    char color = 'b';
    if ((data.find("HD") != data.npos) && (data.find("HD") <= data.length()-4) )
    {
        std::string hd = data.substr(data.find("HD"), 4);
        // make sure the 3rd character is a hyphen
        hd[2] = '-';
        if (hd[3] == '7') color = 'c';
        // put in subfield $n
        MarcField::update('n', hd);
    }
    else
    {
        if (verbose) throw MarcRecordException("ERROR field 533 : no HD specified");
    }

    // now the rest of the qualification: everything after the first :
    if (data.find(":") == std::string::npos)
    {
        if (verbose) throw MarcRecordException("ERROR field 533 : no colon, tiff or jpeg");
    }

    std::string newdata = data.substr(data.find_first_of(":"));
    Helper::Trim(newdata);
    Helper::ReplaceAll(newdata, "JPG", "JPEG");
    MarcField::update('e', newdata);

    // now add a fixed string that this is an available scan
    if (color == 'b')
        MarcField::update('a', "Scan, grayscale");
    else if (color == 'c')
        MarcField::update('a', "Scan, color");

    // another fixed field: $7 , with coded info on the type
        //std::string fixedfield = "|||||||||be|n||"
    // see field 008 starting from position 6 for the meaning
    // since the possibilities of item type are restricted to microfilms, this is useless here
    // also, OCLC does not seem to support this field


    //MarcField::update(marcsubfield, data);
}
