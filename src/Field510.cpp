#include "Field510.h"

Field510::Field510(int nr) : MarcField(nr)
{
    //ctor
    Setindicator1('4');
}

Field510::~Field510()
{
    //dtor
}


void Field510::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // first make all uppercase for ease of parsing
    Helper::MakeUppercase(data);

    //clean up the data: get out the ISBN. First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // now, find segments that contains the letters 'RISM'
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
         std::string rism = "RISM";
         std::size_t found = (*it).find(rism);
         if (found == std::string::npos)
         {
            continue;
         }
         else
         {
             //then put the rest of string in subfield 4c
             (*it).replace((*it).find(rism),rism.length(),"");      // remove the word RISM
             Helper::Trim((*it));       // trim beginning and trailing whitespace

             // hack for grouped subfields: such that $a and $c are always kept together (instead of first printing all $a's then all $c's)
             // insert this as one string in subfield a .
             MarcField::update('a', rism+"$c"+(*it));
         }
    }

    //Subfields are non-repeatable according to MARC standard.
    //This is solved in the PRINT function by printing a new line for every subfield that is present

}


// override print function to handle books with several ISBNs
std::string const Field510::print() const
{
    // print one entire field. First the numbers, then the indicators
    std::ostringstream output;
    output << '=' << std::setfill ('0') << std::setw (3) << Getfieldnr() << "  ";

    output << Getindicator1() << Getindicator2();

    //add all subfields. subfield indicator is dollar sign
    // subfields $a are not repeatable. start a new line for each new one

    bool already_had_a = false;

    // this routine prints an entirely new line for each subfield, since it is non-repeatable
    for ( std::multimap<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
    {
        if ( ((*it).first == 'a') && already_had_a )       //start new line if we already had a previous a
            output << endl << '=' << std::setfill ('0') << std::setw (3) << Getfieldnr() << "  " << Getindicator1() << Getindicator2();

        if (!((*it).second.empty() || (*it).second == "" ))
            output << "$" << (*it).first << (*it).second;

        if ((*it).first == 'a' || (*it).first == 'z')
            already_had_a = true;
    }

    output << endl;
    return output.str();
}
