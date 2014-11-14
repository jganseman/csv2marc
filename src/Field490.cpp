#include "Field490.h"

Field490::Field490(int nr) : MarcField(nr)
{
    //ctor
}

Field490::~Field490()
{
    //dtor
}


void Field490::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // NOTE from MARC website:
    // Field 490 does not serve as a series added entry.
    // When field 490 is used and a series added entry is desired, both the series statement (field 490)
    // and a corresponding series added entry (fields 800-830) are recorded in the bibliographic record.


    // this is only here to set the correct indicators
    Setindicator1('0');
    /*
    0 - Series not traced
    1 - Series traced           // if 1, include field 830 !
    */

    Setindicator2(DEFAULT_INDIC);

    MarcField::update(marcsubfield, data);

}
