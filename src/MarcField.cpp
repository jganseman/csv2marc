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
    output << '=' << std::setfill ('0') << std::setw (3) << fieldnr << "  ";

    // only print indicators and full subfields if field is equal to or larger than 10
    if (fieldnr >= 10 )
    {
        output << indicator1 << indicator2;

        //add all subfields.subfield indicator is dollar sign

        for ( std::multimap<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
        {
            if (!((*it).second.empty() || (*it).second == "" ))
                output << "$" << (*it).first << (*it).second;
        }
    }
    else        // if these are control fields, just print all text
    {
        for ( std::multimap<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
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
    if (data.empty() || data == "")
        return;

    //trim front and trailing whitespace
    data = data.erase(data.find_last_not_of(" \n\r\t")+1).substr(data.find_first_not_of(" \n\r\t"));

    // by default create a new subfield with the same code
    subfields.insert(std::make_pair(marcsubfield, data));

    // see whether this field already exists
    /*
    map<char, std::string>::iterator it = subfields.find(marcsubfield);
    if (it == subfields.end())
    {
        subfields.insert(std::make_pair(marcsubfield, data));
    }
    else    // append data
    {
        (*it).second += data;
    }
    */
}

bool MarcField::isempty() const
{
    for ( std::multimap<char, std::string>::const_iterator it = subfields.begin(); it != subfields.end(); ++it)
    {
        if (!((*it).second.empty() || (*it).second == "" ))
            return false;
    }
    return true;
}

