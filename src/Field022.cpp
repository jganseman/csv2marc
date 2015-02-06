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
             MarcField::update(marcsubfield, cleaneddata.substr(0,9));      // only insert first 9 characters

             if (! (cleaneddata.length() == 9))        // throw error when too many characters are present
             {
                 throw MarcRecordException("WARNING Field 022: ISSN too short/long: " + cleaneddata);
             }

         }
    }

}


// override print function to handle books with several ISBNs
std::string const Field022::print() const
{
    // print one entire field. First the numbers, then the indicators
    std::ostringstream output;
    output << '=' << std::setfill ('0') << std::setw (3) << Getfieldnr() << "  ";

    output << Getindicator1() << Getindicator2();

    //add all subfields. subfield indicator is dollar sign
    // subfields $a are not repeatable. start a new line for each new one

    bool already_had_a = false;

    for ( std::multimap<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
    {
        if ( ((*it).first == 'a') && already_had_a )       //start new line if we already had a previous a
            output << endl << '=' << std::setfill ('0') << std::setw (3) << Getfieldnr() << "  " << Getindicator1() << Getindicator2();

        if (!((*it).second.empty() || (*it).second == "" ))
            output << "$" << (*it).first << (*it).second;

        if ((*it).first == 'a')
            already_had_a = true;
    }

    output << endl;
    return output.str();
}
