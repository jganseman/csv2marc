#include "Field505.h"

Field505::Field505(int nr) : MarcField(nr)
{
    //ctor

    // Formatted contents. Just set correct indicators for now
    Setindicator1('0');
}

Field505::~Field505()
{
    //dtor
}


void Field505::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;


    MarcField::update(marcsubfield, data);

}
