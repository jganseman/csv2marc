#include "Field022.h"

// known list of ISSNs that are printed wrongly in the books themselves
std::set<std::string> Field022::init()
{
    int nrterms=1;
    std::string knownInvalidISSNs[]={
        "0932-611"
        };
    std::set<std::string> tmp(knownInvalidISSNs, knownInvalidISSNs+nrterms);

    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field022::knownInvalidISSNs(init());


Field022::Field022(int nr) : MarcField(nr)
{
    //ctor
}

Field022::~Field022()
{
    //dtor
}


void Field022::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // first make all uppercase for ease of parsing
    Helper::MakeUppercase(data);

    //clean up the data: get out the ISSN. First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // now, find segments that contains the letters 'ISSN'
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
         std::size_t found = (*it).find("ISSN");
         if (found == std::string::npos)
         {
            continue;
         }
         else
         {
             // get out all the numbers, copy them into 'cleaneddata'
             std::string cleaneddata = "";
             std::size_t found2 = (*it).find_first_of("0123456789X");
             while (found2!=std::string::npos)
             {
                cleaneddata += (*it)[found2];
                found2=(*it).find_first_of("0123456789X",found2+1);
             }
             // now manually put a '-' in the middle.
             cleaneddata.insert(4, "-");
                   // only insert first 9 characters

             if (! (cleaneddata.length() == 9))        // throw error when too many characters are present
             {
                 MarcField::update('y', cleaneddata);
                 if (knownInvalidISSNs.find(cleaneddata) == knownInvalidISSNs.end())    // only warn if not in list of known numbers
                 {
                    if (verbose) throw MarcRecordException("WARNING Field 022: invalid ISSN, put in subfield $y: " + cleaneddata);
                 }

             } else
             {
                 MarcField::update(marcsubfield, cleaneddata);
             }

         }
    }

}
