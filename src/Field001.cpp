#include "Field001.h"


// first define the static members of this class
std::set<std::string> Field001::init()
{
    int nrterms=0;
    std::string validterms[]={};        // create empty list
    std::set<std::string> tmp(validterms, validterms+nrterms);     // use as constructor of this type
    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field001::usedIDs(init());


Field001::Field001(int nr) : MarcField(nr)
{
    //ctor
}

Field001::~Field001()
{
    //dtor
}


void Field001::update(char marcsubfield, std::string data)
{
    /* no placenumber errors too much on crossref records, that indeed have none */
    if (data.empty() || data == "")
        return; //throw MarcRecordException("ERROR Field 001: record has no place number.");

    // first, put the place number in anyway
    MarcField::update(marcsubfield, data);

    // only afterwards, throw an error
    if (usedIDs.find(data) == usedIDs.end())        // if not found in set of already used nrs
        usedIDs.insert(data);
    else
        throw MarcRecordException("ERROR Field 001: placenumber already in use : " + data);

}
