#include "Field534.h"

Field534::Field534(int nr) : MarcField(nr)
{
    //ctor
}

Field534::~Field534()
{
    //dtor
}


void Field534::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    MarcField::update(marcsubfield, data);

}
