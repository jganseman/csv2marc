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
    verbose=false;
}

Field001::~Field001()
{
    //dtor
}


void Field001::update(char marcsubfield, std::string data)
{
    /* don't throw errors, else too much errs on crossref records, that indeed have none */
    if (data.empty() || data == "")
        return; //throw MarcRecordException("ERROR Field 001: record has no place number.");

    fixTerminology(data);

    IDcount[data] += 1;     // add one to the counter. Initialized on 0 when this nr is not yet in map

    // only afterwards, throw an error
    if (usedIDs.find(data) == usedIDs.end())        // if not found in set of already used nrs
    {
        usedIDs.insert(data);
        MarcField::update(marcsubfield, data);
    }
    else
    {
        //callnumber has been used. Append count number
        char buffer[6];
        data = data + "-" + itoa(IDcount[data], buffer, 10);

        MarcField::update(marcsubfield, data);      // first insert, then throw error
        if (verbose) throw MarcRecordException("WARNING Field 001: callnumber used, creating new one : " + data);
    }

    // check for semicolon, this might indicate a double callnumber. Except for scans (which contain HD)
    if ((data.find_first_of(";") != data.npos) && (data.find_first_of("HD") == data.npos))
    {
        throw MarcRecordException("WARNING field 001: more than one callnumber? " + data);
    }

}

void Field001::printIDcounts()
{
    cout << "Printing counts >1 of double callnumbers" << endl;
    long count = 0;
    long total = 0;
    for (std::map<std::string, int>::iterator it = IDcount.begin(); it != IDcount.end(); ++it)
    {
        if ((*it).second >= 2)
        {
             cout << (*it).first << "\t\t" << (*it).second << endl;
             count += 1;
             total += (*it).second;
        }

    }
    cout << "TOTALS: " << total << " records from " << count << " callnumbers." << endl;
}

// This routine does some search and replace of often occurring errors in the callnumbers
void Field001::fixTerminology(std::string& data)
{
    // most SLZ records start with "SLZ ;", replace by "SLZ-".
    // For safety, do the same for SLZ and FOLIO
    Helper::ReplaceAll(data, "SLZ ; ", "SLZ-");
    Helper::ReplaceAll(data, "FOLIO ; ", "FOLIO-");
    Helper::ReplaceAll(data, "UNICA ; ", "UNICA-");

    // rearrange Folio / SLZ: put it in front of the number, to enable linking with item parts
    if (data.find("; FOLIO") != data.npos) {
        data = "FOLIO-" + data.substr(0, data.find("; FOLIO"));
    } else if ((data.find("FOLIO") != data.npos) && (data.find("FOLIO") != 0)) {     // skip FOLIO that is already in front
        data = "FOLIO-" + data.substr(0, data.find("FOLIO"));
    } else if (data.find("; SLZ") != data.npos) {
        data = "SLZ-" + data.substr(0, data.find("; SLZ"));
    } else if ((data.find("SLZ") != data.npos) && (data.find("SLZ") != 0)) {        // skip SLZ that is already in front
        data = "SLZ-" + data.substr(0, data.find("SLZ"));
    } else if (data.find("; UNICA") != data.npos) {
        data = "UNICA-" + data.substr(0, data.find("; UNICA"));
    } else if ((data.find("UNICA") != data.npos) && (data.find("UNICA") != 0)) {        // skip SLZ that is already in front
        data = "UNICA-" + data.substr(0, data.find("UNICA"));
    }

    //FCP callnumbers are doubles. FCP is not necessary in the callnumber since it does not point to a specific location. Throw it out.
    if (data.find("FCP") != data.npos)
    {
         data = data.substr(0, data.find(";"));
    }

    // sometimes onderdeel is spelled with - before, which inhibits linking afterwards
    Helper::ReplaceAll(data, "-onderdeel", " onderdeel");

    Helper::Trim(data);
}
