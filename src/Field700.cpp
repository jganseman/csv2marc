#include "Field700.h"

Field700::Field700(int nr) : MarcField(nr)
{
    //ctor
}

Field700::~Field700()
{
    //dtor
}



void Field700::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    Setindicator1('1');      // always suppose it's a family name
    Setindicator2(DEFAULT_INDIC);

    //First, segment by ';'
    std::vector<std::string> alldatasegments;
    std::stringstream alldatastream(data);
    std::string allsegment;
    while(std::getline(alldatastream, allsegment, ';'))
    {
        alldatasegments.push_back(allsegment);
    }

    // iterate over all starting from the second
    for (std::vector<std::string>::iterator it = alldatasegments.begin()+1; it != alldatasegments.end(); ++it)
    {

        // only use the first author for field 100. Put the rest in field 700.
        std::string fullstring = *it;
        std::string author;     // goes in subfield a
        std::string dates;      // goes in subfield d
        std::string relator;    // goes in subfield e
        // in subfields $0 en $4 goes a link to the authority control ID

        // Note: to keep the link between an author and the corresponding date/relation
        // these subfields are added as text to the content of subfield a
        // such that it remains preserved when printing multiple lines of Field 700

        // the relator (function of this author) is anything in between <>
        std::vector<std::string> datasegments;
        std::stringstream datastream(fullstring);
        std::string segment;
        while(std::getline(datastream, segment, '<'))
        {
            datasegments.push_back(segment);
        }

        if (datasegments.size() > 1) // item has relatorial remarks
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                // TODO parse in more detailed way
                //get string
                std::string tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
                //trim any beginning or ending spaces
                tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                //MarcField::update('e', tempstring);
                relator = relator + "$e" + tempstring;
            }
            fullstring = datasegments[0];       // discard all <> info from this string
        }

        // the dates are anything in between ()
        datasegments.clear();
        datastream.clear();
        datastream.str(fullstring);             // set new contents
        while(std::getline(datastream, segment, '('))
        {
            datasegments.push_back(segment);
        }

        if (datasegments.size() > 1) // item has dates
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                // TODO parse in more detailed way
                std::string tempstring = (*it2).substr(0,(*it2).find_first_of(')'));
                //trim any beginning or ending spaces
                tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                //MarcField::update('d', tempstring);
                dates = dates + "$d" + tempstring;
            }
            fullstring = datasegments[0];       // discard all <> info from this string
        }

        // now add the author itself.
        // trim any beginning or ending spaces
        fullstring = fullstring.erase(fullstring.find_last_not_of(" \n\r\t")+1).substr(fullstring.find_first_not_of(" \n\r\t"));
        MarcField::update('a', fullstring+dates+relator);
    }

}


std::string const Field700::print() const
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
