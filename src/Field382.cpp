#include "Field382.h"

Field382::Field382(int nr) : MarcField(nr)
{
    //ctor
}

Field382::~Field382()
{
    //dtor
}


void Field382::update(char marcsubfield, std::string data)
{
    //this field contains details on the instrumentation. If wanted, parse here, and check correctness


    MarcField::update(marcsubfield, data);
}
