#include "Field650.h"

Field650::Field650(int nr) : MarcField(nr)
{
    //ctor
}

Field650::~Field650()
{
    //dtor
}


void Field650::update(char marcsubfield, std::string data)
{

    if (data.empty() || data == "")
        return;

    // set indicators
    Setindicator1('0');             // no 'level is encoded' of topics; they're all just basically tags
    Setindicator2('4');             // no thesaurus is used

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    //TODO: this is not repeatable. Several 650 fields necessary...
    // a subdivision can be used in different subfields:
    //$a - Topical term or geographic name entry element (NR)
    //$v - Form subdivision (R)             // form (e.g. study scores)
    //$x - General subdivision (R)          // generic topic (e.g. history)
    //$y - Chronological subdivision (R)    // time (e.g. century)
    //$z - Geographic subdivision (R)       // place

    // for now, we put everything in subfield a.

    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        // trim whitespace
        try{
            Helper::Trim((*it));
            MarcField::update(marcsubfield, (*it));
        } catch (exception& e)
        {
            throw MarcRecordException("ERROR Field 650 : empty keyword");
        }
    }

}


// separate printing routine to make sure it prints all subfields a on different lines
std::string const Field650::print() const
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
        if ( ((*it).first == 'a') && already_had_a )       //start new line if not last $a field
            output << endl << '=' << std::setfill ('0') << std::setw (3) << Getfieldnr() << "  " << Getindicator1() << Getindicator2();

        if (!((*it).second.empty() || (*it).second == "" ))
            output << "$" << (*it).first << (*it).second;

        if ((*it).first == 'a')
            already_had_a = true;
    }

    output << endl;
    return output.str();
}
