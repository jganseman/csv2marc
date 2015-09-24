#include "Field500.h"

Field500::Field500(int nr) : MarcField(nr)
{
    //ctor
}

Field500::~Field500()
{
    //dtor
}


void Field500::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // Formatted contents. Just set correct indicators for now

    std::string start = data.substr(0,3);
    Helper::MakeUppercase(start);
    if (start == "DIG")             // this will be processed in field 533
        return;
    else if (start == "SAB")        // sabam numbers, will be processed in field 542
        return;

    MarcField::update(marcsubfield, data);

}
