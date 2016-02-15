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

    // if this field contains an editor, it is handled by the legacy subroutine in Field700
    // this may e.g. come from a superseded Allegro field #72 which was later changed into #73x
    if (data.find_first_of("<") != data.npos)
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


    if (data.empty() || data == "")
        return;

    std::string volume = "";

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // there might be multiple series statements (for subseries etc)
    // perform the concatenation trick here as with author (see field 700):
    // concatenate volume to the string as $v

    if (datasegments.size() > 1) // item has volume indicator
    {
        volume = "$v";
        for (std::vector<std::string>::iterator it = datasegments.begin()+1; it != datasegments.end(); ++it)
        {
            // trim trailing and beginning spaces
            Helper::Trim((*it));
            volume += (*it);
        }
    }

    // trim trailing and beginning spaces
    Helper::Trim(datasegments[0]);

    MarcField::update(marcsubfield, datasegments[0]+volume);


    // throw a warning if the field does not contain a semicolon but contains a regular colon.
    /*
    if ((volume == "") && (data.find_first_of(":") != data.npos))
    {
        throw MarcRecordException("WARNING field 490: colon instead of semicolon? : " + data);
    }
    */

    // throw a warning if the field contains characters that might indicate an author is present.
    /*
    if (data.find_first_of("<") != data.npos)
    {
        throw MarcRecordException("WARNING field 490: contains author? : " + data);
    }
    */
    // --> A legacy subroutine was added to Field700.cpp to parse authors in this list


}
