#include "Field591.h"

Field591::Field591(int nr): MarcField(nr)
{
    verbose=false;
    //ctor
}

Field591::~Field591()
{
    //dtor
}


void Field591::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    //This records the total number of units in the item.
    //Which was taken out of field 300a and put here, to make it easier to manipulate separately

    Helper::EraseWhitespace(data);
    long converted = atol(data.c_str());
    // DOUBLE CHECK : it's only a unit field if there is no other character in the string
    if ((converted) && (data.find_first_not_of("0123456789") == data.npos))
    {
       // put this number in, and add "units" (not using items as that's used in the interface for available copies)
       (converted > 1) ? MarcField::update('a', data + " units") : MarcField::update('a', data + " unit");
    }

}
