#include "Field650.h"

Field650::Field650(int nr) : MarcField(nr)
{
    //ctor
}

Field650::~Field650()
{
    //dtor
}


void Field650::update(char marcsubfield, std::string data)
{

    if (data.empty() || data == "")
        return;

    // set indicators
    Setindicator1('0');             // no 'level is encoded' of topics; they're all just basically tags
    Setindicator2('4');             // no thesaurus is used

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    //TODO: this is not repeatable. Several 650 fields necessary...
    // a subdivision can be used in different subfields:
    //$a - Topical term or geographic name entry element (NR)
    //$v - Form subdivision (R)             // form (e.g. study scores)
    //$x - General subdivision (R)          // generic topic (e.g. history)
    //$y - Chronological subdivision (R)    // time (e.g. century)
    //$z - Geographic subdivision (R)       // place

    // for now, we put everything in subfield a.

    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        // trim whitespace
        try{
            Helper::Trim((*it));
            if (!((*it).empty() || (*it) == ""))
                MarcField::update(marcsubfield, (*it));
        } catch (exception& e)
        {
            throw MarcRecordException("ERROR Field 650 : empty keyword or excess semicolon.");
        }
    }
}

