#include "Field700.h"


// initialize relator terms
std::set<std::string> Field700::init()
{
    int nrterms=33;
    std::string validterms[]={
        "act.",
        "a.",
        "beg.",
        "bew.",
        "cadensschr.",
        "choreogr.",
        "comm.",
        "c.",
        "kop.",
        "dir.",
        "ed.",
        "prod.",
        "sign.",
        "red.",
        "ill.",
        "impr.",
        "inl.",
        "perf.",
        "docent",
        "tekstd.",
        "medium",
        "thema",
        "tech.",
        "optek.",
        "prod.",
        "pseudo.",
        "reconstr.",
        "reg.",
        "samenst.",
        "scen.",
        "vert.",
        "volt.",
        "arr."
         };
    std::set<std::string> tmp(validterms, validterms+nrterms);

    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field700::relatorterms(init());




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

    // iterate over all segments starting from the second (first one is in Field100)

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

        // this part: copied from Field100
        if (datasegments.size() > 1) // item has relatorial remarks
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                std::string tempstring;
                try{
                    tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
                    //first remove trailing and beginning whitespace; for next segmentation to work properly
                    tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                } catch(exception& e)
                {
                    throw MarcRecordException("ERROR Updating Field 700: empty author function.");
                }

                // note: sometimes added in wrong way, with 2 terms in one set of brackets. thus -> segment further
                std::vector<std::string> relatorsegments;
                std::stringstream relatorstream(tempstring);
                while(std::getline(relatorstream, segment, ' '))        // will still err if no spacing used
                {
                    relatorsegments.push_back(segment);
                }

                for (std::vector<std::string>::iterator it3 = relatorsegments.begin(); it3 != relatorsegments.end(); ++it3)
                {
                    //get string
                        std::string tempstring = (*it3);
                        //trim any beginning or ending spaces
                        tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                        //forcibly make this all lowercase
                        std::transform(tempstring.begin(), tempstring.end(), tempstring.begin(), ::tolower);
                        //update
                        relatorFixer(tempstring);
                        // check that the list conforms to the given list of names
                        if (!isValidRelator(tempstring))
                        {
                            throw MarcRecordException("WARNING Updating 700: Unknown author function: "+tempstring);
                        }
                        // to preserve the link with the author $a subfield, everything is internally stored in the
                        // $a subfield as one entire string that is already printed
                        relator = relator + "$e" + tempstring;
                }
            }
            fullstring = datasegments[0];       // discard all <relator> info from this string
        }


        /*
        if (datasegments.size() > 1) // item has relatorial remarks
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                // TODO parse in more detailed way
                //get string
                try {
                    std::string tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
                    //trim any beginning or ending spaces
                    tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                    //MarcField::update('e', tempstring);
                    relator = relator + "$e" + tempstring;
                } catch (exception& e)
                {
                    throw MarcRecordException("ERROR Updating Field 700: empty author function.");
                }

            }
            fullstring = datasegments[0];       // discard all <> info from this string
        }
        */

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
                try{
                    std::string tempstring = (*it2).substr(0,(*it2).find_first_of(')'));
                    //trim any whitespaces out
                    tempstring.erase(remove_if(tempstring.begin(), tempstring.end(), ::isspace), tempstring.end());
                    //trim any beginning or ending spaces
                    //tempstring = tempstring.erase(tempstring.find_last_not_of(" \n\r\t")+1).substr(tempstring.find_first_not_of(" \n\r\t"));
                    //MarcField::update('d', tempstring);
                    dates = dates + "$d" + tempstring;
                } catch (exception& e)
                {
                    throw MarcRecordException("ERROR Updating Field 700: error in author dates.");
                }
            }
            fullstring = datasegments[0];       // discard all (date) info from this string
        }

        // now add the author itself.

        try{
            //trim front and trailing whitespace
            fullstring = fullstring.erase(fullstring.find_last_not_of(" \n\r\t")+1).substr(fullstring.find_first_not_of(" \n\r\t"));
        } catch (exception e)
        {
            throw MarcRecordException("ERROR Updating Field 700: empty value among authors.");
        }

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




bool Field700::isValidRelator(std::string& data)
{
    return (relatorterms.find(data) != relatorterms.end());
}

void Field700::relatorFixer(std::string& data)
{
    // append shortening point where necessary
    if ((data == "a") || (data == "c") || (data == "ill") || (data == "ed")
        || (data == "tekstd") || (data == "pseudo") || (data == "inl") || (data == "bew")
        || (data == "vert") || (data == "perf") || (data == "red")
        )
    {
        data += ".";
        return;
    }

    // remove shortening point where necessary
    if (data == "thema.")
    {
        data = "thema";
        return;
    }

    // fix common spelling mistakes
    if (data.find("teks") != data.npos) data = "tekstd.";
    if (data.find("tesk") != data.npos) data = "tekstd.";
    if (data.find("tekt") != data.npos) data = "tekstd.";
    if (data.find("tel") != data.npos) data = "tekstd.";
    if (data.find("trad") != data.npos) data = "vert.";
    if (data.find("cop") != data.npos) data = "kop.";
    if (data.find("chor") != data.npos) data = "choreogr.";
    if (data.find("cond") != data.npos) data = "dir.";
    if (data.find("trans") != data.npos) data = "ed.";
    if (data.find("exec") != data.npos) data = "perf.";
    if (data.find("caden") != data.npos) data = "cadensschr.";

    return;
}
