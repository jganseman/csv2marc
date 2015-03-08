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
    if (data.empty() || data == "")
        return;

    // first make all uppercase for ease of parsing
    Helper::MakeUppercase(data);

    //clean up the data: get out the ISMN. First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

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

             MarcField::update(marcsubfield, cleaneddata);
             // set indicator to ISMN
             Setindicator1('2');

            //assert number starts with 9790 or M
             if (! (cleaneddata[0] == 'M' || cleaneddata.substr(0,4) == "9790"))
                throw MarcRecordException("WARNING Field 024: string is not a valid ISMN : " + cleaneddata);
         }
    }

    // TODO: DISCARDS RISM nrs now -> FIND OUT WHERE TO STORE THOSE IN MARC RECORDS! (510 ???)
}

// override print function to handle books with several ISBNs
std::string const Field024::print() const
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

