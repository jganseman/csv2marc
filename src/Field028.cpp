#include "Field028.h"

Field028::Field028(int nr) : MarcField(nr)
{
    //ctor
}

Field028::~Field028()
{
    //dtor
}


void Field028::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // NOTE from MARC website: "Publisher's numbers that are given in an unformatted form are recorded in field 500 (General Note)." //TODO

    // this is only here to set the correct indicators
    Setindicator1('5');
    /*
    Type of publisher number
    0 - Issue number
    1 - Matrix number
    2 - Plate number
    3 - Other music number
    4 - Videorecording number
    5 - Other publisher number
    */

    Setindicator2('0');
    /*
    Note/added entry controller
    0 - No note, no added entry
    1 - Note, added entry
    2 - Note, no added entry
    3 - No note, added entry
    */

    MarcField::update(marcsubfield, data);

}
