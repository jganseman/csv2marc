#include "Field022.h"

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
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);

    //clean up the data: get out the ISSN. First, segment by ';'
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }

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
             std::string cleaneddata = "";
             std::size_t found2 = (*it).find_first_of("0123456789X");
             while (found2!=std::string::npos)
             {
                cleaneddata += (*it)[found2];
                found2=(*it).find_first_of("0123456789X",found2+1);
             }
             // put a '-' in the middle.
             cleaneddata.insert(4, "-");
             MarcField::update(marcsubfield, cleaneddata.substr(0,9));      // only insert first 9 characters
         }
    }

    //TODO: some records have multiple ISSN nrs. These should appear on several different 020 fields, and not as several subfields a!
    // For now: this program is only made to have repeat subfields, no repeat entire fields. Try to find a solution with MarcEdit?
}
