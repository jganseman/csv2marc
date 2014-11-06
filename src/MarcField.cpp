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
    output << '=' << fieldnr << '\t' << indicator1 << indicator2;

    //add all subfields.subfield indicator is dollar sign

    for ( std::map<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
    {
        output << "$" << (*it).first << (*it).second;
    }

    output << endl;
    return output.str();
}

void MarcField::update(char marcsubfield, std::string data)
{
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
