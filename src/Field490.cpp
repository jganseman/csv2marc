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


    if (data.empty() || data == "")
        return;

    std::string volume = "";

    //First, segment by ';'
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }

    // there might be multiple series statements (for subseries etc)
    // perform the concatenation trick here as with author (see field 700):
    // concatenate volume to the string as $v

    if (datasegments.size() > 1) // item has volume indicator
    {
        volume = "$v";
        for (std::vector<std::string>::iterator it = datasegments.begin()+1; it != datasegments.end(); ++it)
        {
            // trim trailing and beginning spaces
            (*it) = (*it).erase((*it).find_last_not_of(" \n\r\t")+1).substr((*it).find_first_not_of(" \n\r\t"));
            volume += (*it);
        }
    }

    // trim trailing and beginning spaces
    datasegments[0] = datasegments[0].erase(datasegments[0].find_last_not_of(" \n\r\t")+1).substr(datasegments[0].find_first_not_of(" \n\r\t"));

    MarcField::update(marcsubfield, datasegments[0]+volume);
}
