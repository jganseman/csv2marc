#include "Field264.h"

Field264::Field264(int nr) : MarcField(nr)
{
    //ctor
}

Field264::~Field264()
{
    //dtor
}

void Field264::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // set indicators
    Setindicator2('2');             // currently we use this for the Distributor info

    MarcField::update(marcsubfield, data);
}
