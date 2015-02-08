#include "Field383.h"

Field383::Field383(int nr) : MarcField(nr)
{
    //ctor
}

Field383::~Field383()
{
    //dtor
}


void Field383::update(char marcsubfield, std::string data)
{
    //this field contains the opus number

    MarcField::update(marcsubfield, data);


    // opus number should be in $b, thematic index numbers (like KV, BWV etc) should be in $c

}
