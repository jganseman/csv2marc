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


// same for the count of the IDs
std::map<std::string, int> Field001::init2()
{
    //std::pair <std::string,int> foo (std::make_pair("00", 0));
    std::map<std::string, int> tmp;     // fill with 0;0 to begin with. only used for retrieval anyway
    tmp["0"] = 0;
    return tmp;
}
// and initialize
std::map<std::string, int> Field001::IDcount(init2());



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
    //MarcField::update(marcsubfield, data);

    IDcount[data] += 1;     // add one to the counter. creates count initialized on 0 when data not yet in map

    // only afterwards, throw an error
    if (usedIDs.find(data) == usedIDs.end())        // if not found in set of already used nrs
    {
                usedIDs.insert(data);
                MarcField::update(marcsubfield, data);
    }
    else
    {
        //throw MarcRecordException("ERROR Field 001: placenumber already in use : " + data);

        //placenumber has been used. Append count number
        char buffer[6];
        data = data + "-" + itoa(IDcount[data], buffer, 10);

        MarcField::update(marcsubfield, data);      // first insert, then throw error
        //throw MarcRecordException("WARNING Field 001: placenumber used, creating new one : " + data);
    }

}
