#include "Field542.h"

Field542::Field542(int nr) : MarcField(nr)
{
    MarcField::Setindicator1('1');      // public note
    //ctor
}

Field542::~Field542()
{
    //dtor
}



void Field542::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    std::string start = data.substr(0,5);
    if (start != "Sabam")             // this will be processed in field 533
        return;

    MarcField::update(marcsubfield, data);

}
