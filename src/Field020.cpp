#include "Field020.h"

// known list of ISBNs that are printed wrongly in the books themselves
std::set<std::string> Field020::init()
{
    int nrterms=14;
    std::string knownInvalidISBNs[]={
        "978761815946",
        "28714600000",
        "09410840708",
        "905400951",
        "377510190",
        "349218734",
        "892210624",
        "75632088",
        "75632096",
        "18771702X",
        "09607608",
        "906853992",
        "848592755",
        "03132569341"
        };
    std::set<std::string> tmp(knownInvalidISBNs, knownInvalidISBNs+nrterms);

    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field020::knownInvalidISBNs(init());



Field020::Field020(int nr) : MarcField(nr)
{
    //ctor
}

Field020::~Field020()
{
    //dtor
}


void Field020::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // first make all uppercase for ease of parsing
    Helper::MakeUppercase(data);

    //clean up the data: get out the ISBN. First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // now, find segments that contains the letters 'ISBN'
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
         std::size_t found = (*it).find("ISBN");
         if (found == std::string::npos)
         {
            continue;
         }
         else
         {
             std::string cleaneddata = "";
             std::size_t found2 = (*it).find_first_of("0123456789X");
             while (found2!=std::string::npos)
             {
                cleaneddata += (*it)[found2];
                found2=(*it).find_first_of("0123456789X",found2+1);
             }

             //assert number is 10 or 13 digits
             if (! (cleaneddata.length() == 10 || cleaneddata.length() == 13))
             {
                 // throw MarcRecordException("Error Updating Field 020: string is not a valid ISBN : " + cleaneddata);
                 MarcField::update('z', cleaneddata);   // invalid ISBNs go in subfield 020$z
                 if (knownInvalidISBNs.find(cleaneddata) == knownInvalidISBNs.end())   // only warn if not in list of known numbers
                 {
                    if (verbose) throw MarcRecordException("WARNING Field 020: invalid ISBN, put in subfield $z: " + cleaneddata);
                 }
             }
             else // length is OK
             {
                 MarcField::update(marcsubfield, cleaneddata);
             }

         }
    }

    //Subfields are non-repeatable according to MARC standard.
    //This is now solved by the extractDoubleSubfields routine in MarcRecord.cpp and
}

