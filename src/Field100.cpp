#include "Field100.h"

Field100::Field100(int nr) : MarcField(100)
{
    //ctor
}

Field100::~Field100()
{
    //dtor
}


void Field100::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    Setindicator1('1');      // always suppose it's a family name
    Setindicator2(DEFAULT_INDIC);

    //First, segment by ';'
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }

    // only use the first author for field 100. Put the rest in field 700.
    std::string fullstring = datasegments[0];
    std::string author;     // goes in subfield a
    std::string dates;      // goes in subfield d
    std::string relator;    // goes in subfield e
    // in subfields $0 en $4 goes a link to the authority control ID


    // the relator (function of this author) is anything in between <>
    datasegments.clear();
    datastream.clear();
    datastream.str(fullstring);             // set new contents
    while(std::getline(datastream, segment, '<'))
    {
        datasegments.push_back(segment);
    }

    if (datasegments.size() > 1) // item has relatorial remarks
    {
        for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
        {
            // TODO parse in more detailed way
            //get string
            std::string tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
            //trim any beginning or ending spaces
            tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
            MarcField::update('e', tempstring);
        }
        fullstring = datasegments[0];       // discard all <> info from this string
    }

    // the dates are anything in between ()
    datasegments.clear();
    datastream.clear();
    datastream.str(fullstring);             // set new contents
    while(std::getline(datastream, segment, '('))
    {
        datasegments.push_back(segment);
    }

    if (datasegments.size() > 1) // item has dates
    {
        for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
        {
            // TODO parse in more detailed way
            std::string tempstring = (*it2).substr(0,(*it2).find_first_of(')'));
            //trim any beginning or ending spaces
            tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
            MarcField::update('d', tempstring);
            break;      // only do this once; can't have multiple dates...
        }
        fullstring = datasegments[0];       // discard all <> info from this string
    }

    // now add the author itself.
    // trim any beginning or ending spaces
    fullstring = fullstring.erase(fullstring.find_last_not_of(" \n\r\t")+1).substr(fullstring.find_first_not_of(" \n\r\t"));
    MarcField::update('a', fullstring);

}
