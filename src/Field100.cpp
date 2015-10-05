#include "Field100.h"



// initialize the translation list for relator terms
Translator Field100::t100e("t100e.csv");     // The file must be in the same folder as the executable.
Translator Field100::t100a("t100a.csv");

// now begin with the normal class stuff

Field100::Field100(int nr) : MarcField(100)
{
    //ctor
}

Field100::~Field100()
{
    //dtor
}


void Field100::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    Setindicator1('1');      // always suppose it's a family name
    Setindicator2(DEFAULT_INDIC);

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // only use the first author for field 100. Put the rest in field 700.
    std::string fullstring = datasegments[0];
    std::string author;     // goes in subfield a
    std::string dates;      // goes in subfield d
    std::string relator;    // goes in subfield e
    // in subfields $0 en $4 goes a link to the authority control ID

    // the relator (function of this author) is anything in between <>
    datasegments.clear();
    datasegments = Helper::Segment(fullstring, '<');

    if (datasegments.size() > 1) // item has relatorial remarks
    {
        for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
        {
            std::string tempstring;
            try{
                tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
                //first remove trailing and beginning whitespace; such that next segmentation works properly
                Helper::Trim(tempstring);
            } catch(exception& e)
            {
                if (verbose) throw MarcRecordException("ERROR Updating Field 100: empty author function.");
            }

            // note: sometimes added in wrong way, with 2 terms in one set of brackets. thus -> segment further
            std::vector<std::string> relatorsegments = Helper::Segment(tempstring, ' ');        // will still err if no spacing used

            for (std::vector<std::string>::iterator it3 = relatorsegments.begin(); it3 != relatorsegments.end(); ++it3)
            {
                //get string
                std::string tempstring = (*it3);
                //trim any beginning or ending spaces
                Helper::Trim(tempstring);
                //forcibly make this all lowercase
                Helper::MakeLowercase(tempstring);
                //update
                relatorFixer(tempstring);
                MarcField::update('e', t100e.translate(tempstring));
                // check that the list conforms to the given list of names
                if (!isValidRelator(tempstring))
                {
                    if (verbose) throw MarcRecordException("ERROR Updating 100: Unknown author function: "+tempstring);
                }
            }
        }
        fullstring = datasegments[0];       // discard all <> info from this string
    }

    // the dates are anything in between ()
    datasegments.clear();
    datasegments = Helper::Segment(fullstring, '(');

    if (datasegments.size() > 1) // item has dates
    {
        for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
        {
            // TODO parse in more detailed way
            std::string tempstring = (*it2).substr(0,(*it2).find_first_of(')'));
            //trim any whitespaces out
            Helper::EraseWhitespace(tempstring);
            MarcField::update('d', tempstring);
            break;      // only do this once; can't have multiple dates...
        }
        fullstring = datasegments[0];       // discard all <> info from this string
    }

    // now add the author itself.
    // trim any beginning or ending spaces
    Helper::Trim(fullstring);
    MarcField::update('a', t100a.translate(fullstring));

}




bool Field100::isValidRelator(std::string& data)
{
    return t100e.isKey(data);
    //return (relatorterms.find(data) != relatorterms.end());
}


void Field100::relatorFixer(std::string& data)
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
    if (data.find("tekd") != data.npos) data = "tekstd.";
    if (data.find("tel") != data.npos) data = "tekstd.";
    if (data.find("trad") != data.npos) data = "vert.";
    if (data.find("cop") != data.npos) data = "kop.";
    if (data.find("chor") != data.npos) data = "choreogr.";
    if (data.find("cond") != data.npos) data = "dir.";
    if (data.find("trans") != data.npos) data = "ed.";
    if (data.find("exec") != data.npos) data = "perf.";
    if (data.find("caden") != data.npos) data = "cadensschr.";
    if (data.find("rev") != data.npos) data = "ed.";
    if (data.find("com") != data.npos) data = "comm.";

    return;
}

