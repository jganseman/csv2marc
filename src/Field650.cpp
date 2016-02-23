#include "Field650.h"

// initialize the translation list for keywords
Translator Field650::t650a("t650a.csv");     // The file must be in the same folder as the executable.
//Translator Field650::t650a;                 // Empty mapping, in case no translation should be done

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
    Helper::ReplaceAll(data, "--", ";");
    Helper::ReplaceAll(data, " - ", ";");
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
        try{
            // lift out anything between brackets
            Helper::RemoveAllOf((*it), "[]");        // by definition everything here is user added
            if ((*it).find("(") != std::string::npos)
            {
                unsigned int beginbrackets = (*it).find("(");
                unsigned int endbrackets = (*it).rfind(")");
                unsigned int betweenlength = endbrackets-beginbrackets-1;
                std::string betweenbrackets = (*it).substr(beginbrackets+1, betweenlength);
                Helper::Trim(betweenbrackets);
                (betweenbrackets)[0] = toupper(betweenbrackets[0]);
                MarcField::update(marcsubfield, t650a.translate(betweenbrackets));
                (*it) = (*it).substr(0, beginbrackets);
            }

            // trim whitespace
            Helper::Trim((*it));
            if (!((*it).empty() || (*it) == ""))
            {
                //capitalize first letter
                (*it)[0] = toupper((*it)[0]);
                MarcField::update(marcsubfield, t650a.translate((*it)));
            }

        } catch (exception& e)
        {
            if (verbose) throw MarcRecordException("ERROR Field 650 : empty keyword or excess semicolon.");
        }
    }
}

