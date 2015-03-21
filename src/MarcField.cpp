#include "MarcField.h"

MarcField::MarcField(int nr, char defchar)
{
    fieldnr=nr;
    indicator1=defchar;
    indicator2=defchar;
//    parent=0;
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

        for ( t_subfields_constit it = subfields.begin(); it != subfields.end(); ++it)
        {
            if (!((*it).second.empty() || (*it).second == "" ))
                output << "$" << (*it).first << (*it).second;
        }
    }
    else        // if these are control fields, just print all text
    {
        for ( t_subfields_constit it = subfields.begin(); it != subfields.end(); ++it)
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
    Helper::Trim(data);

    // by default create a new subfield with the same code
    subfields.insert(std::make_pair(marcsubfield, data));

}

bool MarcField::isempty() const
{
    for ( t_subfields_constit it = subfields.begin(); it != subfields.end(); ++it)
    {
        if (!((*it).second.empty() || (*it).second == "" ))
            return false;
    }
    return true;
}


std::string MarcField::Getsubfield(char mychar) const
{
    /*
    for ( t_subfields_constit it = subfields.begin(); it != subfields.end(); ++it)
    {
        if ((*it).first == mychar)
            return (*it).second;
    }
    return "";
    */

    t_subfields_constit it = subfields.find(mychar);
    if (it != subfields.end())
        return (*it).second;
    else
        return "";

}




vector < pair <char, string> > MarcField::extractDoubleSubfields(char mychar)
{
    vector < pair <char, string> > doublesubfields;

    //cout << "extracting doubles in field " << mychar << endl;
    pair<t_subfields_it, t_subfields_it> range = subfields.equal_range(mychar);

    if (range.first == range.second)        // subfield not found
        return doublesubfields;

    range.first++;      // only increment is supported, not operator+

    if (range.second == range.first)      // only one subfield; nothing to uniquify
        return doublesubfields;

    for ( t_subfields_it it = range.first ; it != range.second; ++it)
    {
        doublesubfields.push_back(*it);
        //doublesubfields.push_back(std::make_pair(mychar, (*it).second));
        // TODO check MEMORY: maybe we need to make an explicit copy if we erase in subfieldmap later?
    }

    // erase those subfields in this field
    subfields.erase(range.first, range.second);

    return doublesubfields;
}

