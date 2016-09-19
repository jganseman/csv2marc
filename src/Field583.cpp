#include "Field583.h"

Field583::Field583(int nr) : MarcField(nr)
{
    // put in ctor, to have it only once included

    MarcField::Setindicator1('1');      // set as not private. Visibility to be controlled by external software
    MarcField::update('a', "Catalography:");
}

Field583::~Field583()
{
    //dtor
}



void Field583::update(char marcsubfield, std::string data)
{
    // Field 583 contains the author (subfield k) and status (subfield l)

    if (data.empty() || data == "")
        return;

    if (marcsubfield == 'k') // author
    {
        Helper::MakeLowercase(data);
        Helper::Trim(data);
        // thjs was originally in Record.cpp->isCRB().
        if  ( (data == "mv") || (data == "im") || (data == "mcl") || (data == "sdp") || (data == "lm")
            || (data == "ah") || (data == "ar") || (data == "bkb") || (data == "cc") || (data == "cf")
            || (data == "df") || (data == "dl") || (data == "ed") || (data == "fb") || (data == "hl")
            || (data == "id") || (data == "ls") || (data == "mdd") || (data == "mt") || (data == "md")
            || (data == "rs") || (data == "th") || (data == "mm") || (data == "hch")
            || (data == "yi") || (data == "smv") || (data == "nt"))
        {
                data = "crb-"+data;
        }
        else
        {
            data = "kcb-"+data;
        }
        MarcField::update(marcsubfield, data);
    }

    if ((marcsubfield == 'l') || (marcsubfield == 'c')) // status or timecode
    {
        Helper::MakeUppercase(data);
        Helper::Trim(data);
        MarcField::update(marcsubfield, data);
    }

}
