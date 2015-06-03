#include "Field020.h"

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
                 if (verbose) throw MarcRecordException("WARNING Field 020: invalid ISBN, put in subfield §z: " + cleaneddata);
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

