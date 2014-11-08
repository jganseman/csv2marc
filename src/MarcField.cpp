#include "MarcField.h"

MarcField::MarcField(int nr, char defchar)
{
    fieldnr=nr;
    indicator1=defchar;
    indicator2=defchar;
}

MarcField::~MarcField()
{
    //dtor
}

std::string const MarcField::print() const
{
    // print one entire field. First the numbers, then the indicators
    std::ostringstream output;
    output << '=' << std::setfill ('0') << std::setw (3) << fieldnr << '\t';

    // only print indicators and full subfields if field is equal to or larger than 10
    if (fieldnr >= 10 )
    {
        output << indicator1 << indicator2;

        //add all subfields.subfield indicator is dollar sign

        for ( std::map<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
        {
            if (!((*it).second.empty() || (*it).second == "" ))
                output << "$" << (*it).first << (*it).second;
        }
    }
    else        // if these are control fields, just print all text
    {
        for ( std::map<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
        {
            if (!((*it).second.empty() || (*it).second == "" ))
                output << (*it).second;
        }
    }

    output << endl;
    return output.str();
}

void MarcField::update(char marcsubfield, std::string data)
{
    // do some basic data cleaning and string formatting here, if it's not worth writing an entire new class for
    // TODO in an ideal setting, those are all different subclasses with consistency checking...
    // TODO other possibility is to make a postprocess cleanup routine
    switch (fieldnr)
    {
            case 5: (data == "") ? data = "00000000000000.0" : data += "000000.0" ;         // Latest Transaction Time: append time to date;
    }


    // see whether this field already exists
    map<char, std::string>::iterator it = subfields.find(marcsubfield);
    if (it == subfields.end())
    {
        subfields.insert(std::make_pair(marcsubfield, data));
    }
    else    // append data
    {
        (*it).second += data;
    }



}

bool MarcField::isempty() const
{
    for ( std::map<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
    {
        if (!((*it).second.empty() || (*it).second == "" ))
            return false;
    }
    return true;
}

