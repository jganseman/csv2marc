#include "Field024.h"

Field024::Field024(int nr) : MarcField(nr)
{
    //ctor
}

Field024::~Field024()
{
    //dtor
}


void Field024::update(char marcsubfield, std::string data)
{
    // first make all uppercase for ease of parsing
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);

    //clean up the data: get out the ISMN. First, segment by ';'
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }

    // now, find segments that contains the letters 'ISMN'
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
         std::size_t found = (*it).find("ISMN");
         if (found == std::string::npos)
         {
            continue;
         }
         else
         {
             std::string cleaneddata = "";
             std::size_t found2 = (*it).find_first_of("M0123456789");       // nr cannot contain X
             while (found2!=std::string::npos)
             {
                cleaneddata += (*it)[found2];
                found2=(*it).find_first_of("M0123456789",found2+1);
             }

             //erase the first M, as that comes from the word ISMN
             if (cleaneddata[0] == 'M')
                cleaneddata = cleaneddata.substr(1);

             //assert number starts with 9790 or M
             if (! (cleaneddata[0] == 'M' || cleaneddata.substr(0,4) == "9790"))
                throw MarcRecordException("Error Updating Field 020: string is not a valid ISMN : " + cleaneddata);

             MarcField::update(marcsubfield, cleaneddata);
             // set indicator to ISMN
             Setindicator1('2');
         }
    }

    //TODO: some records have multiple ISMN nrs. These should appear on several different 024 fields, and not as several subfields a!
    // For now: this program is only made to have repeat subfields, no repeat entire fields. Try to find a solution with MarcEdit?

    // TODO: DISCARDS RISM nrs now -> FIND OUT WHERE TO STORE THOSE IN MARC RECORDS!
}
