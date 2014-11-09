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
    Setindicator1('#');

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
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        // TODO parse in more detailed way
        MarcField::update('e', (*it).substr(0,(*it).find_first_of('>')));
    }
    fullstring = datasegments[0];       // discard all <> info from this string


    // the dates are anything in between ()
    datasegments.clear();
    datastream.clear();
    datastream.str(fullstring);             // set new contents
    while(std::getline(datastream, segment, '('))
    {
        datasegments.push_back(segment);
    }
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        // TODO parse in more detailed way
        MarcField::update('d', (*it).substr(0,(*it).find_first_of(')')));
    }
    fullstring = datasegments[0];       // discard all <> info from this string

    // now add the author itself
    MarcField::update('a', fullstring);

}
