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

    // there could be multiple fields. In that case, separate by ".-" and add multiple subfields a
    // to compensate, use the "put everything in $a" trick and process the field in MarcRecord.h as Unique field
    Helper::ReplaceAll(data, ". -", ".-");

    do          // iterate at least once
    {
        std::string currentdata = data.substr(0, data.find(".-"));

        std::string current_e = "";
        std::string current_n = "";

                // ignore all data not starting with DIG or HD
        Helper::Trim(currentdata);
        if (currentdata.empty() || currentdata == "") return;

        std::string start = currentdata.substr(0,3);
        Helper::MakeUppercase(start);
        if (start != "DIG" && start != "HD-" && start != "HD " )
            return;

        // put all data in capital letters
        Helper::MakeUppercase(currentdata);
        // remove DIG qualifier
        Helper::ReplaceAll(currentdata, "DIG-", "");
        Helper::ReplaceAll(currentdata, "DIG", "");

        // trim again. if nothing left, quit here
        Helper::Trim(currentdata);
        if (currentdata.empty() || currentdata == "")
            return;

        // find the HD qualifier
        char color = 'b';
        if ((currentdata.find("HD") != currentdata.npos) && (currentdata.find("HD") <= currentdata.length()-4) )
        {
            std::string hd = currentdata.substr(currentdata.find("HD"), 4);
            // make sure the 3rd character is a hyphen
            hd[2] = '-';
            if ( hd[3] == '7' || hd[3] == '8' || hd[3] == '9' ) color = 'c';
            // put in subfield $n
            current_n = "$n"+hd;
        }
        else
        {
            if (verbose) throw MarcRecordException("ERROR field 533 : no HD specified");
        }

        // now the rest of the qualification: everything after the first :
        if (currentdata.find(":") == std::string::npos)
        {
            if (verbose) throw MarcRecordException("ERROR field 533 : no colon, tiff or jpeg");
        }

        std::string newdata = currentdata.substr(currentdata.find_first_of(":"));
        newdata = newdata.substr(newdata.find_first_not_of(":"));     // remove initial ":" if it is present
        Helper::Trim(newdata);
        Helper::ReplaceAll(newdata, "JPG", "JPEG");
        Helper::ReplaceAll(newdata, "TIF ", "TIFF ");
        current_e = "$e"+newdata;

        // now add a fixed string that this is an available scan
        if (color == 'b')
            MarcField::update('a', "Scan, grayscale."+current_e+current_n);
        else if (color == 'c')
            MarcField::update('a', "Scan, color."+current_e+current_n);

        if (data.find(".-") != data.npos)
            data = data.substr(data.find(".-")).substr(data.find_first_not_of(".-"));// prepare to process next line
        else
            return;

    }
    while (data.find(".-") != data.npos);

    // another fixed field: $7 , with coded info on the type
        //std::string fixedfield = "|||||||||be|n||"
    // see field 008 starting from position 6 for the meaning
    // since the possibilities of item type are restricted to microfilms, this is useless here
    // also, OCLC does not seem to support this field


    //MarcField::update(marcsubfield, data);
}
