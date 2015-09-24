#include "Field583.h"

Field583::Field583(int nr) : MarcField(nr)
{
    //ctor
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

    MarcField::Setindicator1('1');      // set as not private. Visibility to be controlled by external software

    if (marcsubfield == 'k') // author
    {
        Helper::MakeLowercase(data);
        Helper::Trim(data);
        MarcField::update(marcsubfield, data);
    }

    if (marcsubfield == 'l') // status
    {
        Helper::MakeUppercase(data);
        Helper::Trim(data);
        MarcField::update(marcsubfield, data);
    }

    MarcField::update('a', "catalography;");


}
