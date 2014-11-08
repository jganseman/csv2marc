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
    // first make all uppercase for ease of parsing
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);

    //clean up the data: get out the ISBN. First, segment by ';'
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }

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
                throw MarcRecordException("Error Updating Field 020: string is not a valid ISBN : " + cleaneddata);

             MarcField::update(marcsubfield, cleaneddata);
         }
    }

    //TODO: some records have multiple ISBN nrs. These should appear on several different 020 fields, and not as several subfields a!
    // For now: this program is only made to have repeat subfields, no repeat entire fields. Try to find a solution with MarcEdit?
}
