#include "Field700.h"


// initialize the translation list for relator terms
Translator Field700::t700e("t100e.csv");     // The file must be in the same folder as the executable.
Translator Field700::t700a("t100a.csv");


Field700::Field700(int nr) : MarcField(nr)
{
    //ctor

    Setindicator1('1');      // always suppose it's a family name
    Setindicator2(DEFAULT_INDIC);
}

Field700::~Field700()
{
    //dtor
}



void Field700::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // deal with editors that have been recorded in series statements. Hack in as a different subfield, but test on content!
    if (marcsubfield == 'L')
    {
        if (data.find_first_of("<") != data.npos)       // if this data contains an editorial statement
        {
            // add a bogus author to deal with field 700 only processing from the 2nd author onwards
            data = "DUMMY ; "+data;
            marcsubfield='a';       // process this data as an author
        }
        else
            return;         // otherwise end this function
    }

    //First, segment by ';'
    Helper::ReplaceAll(data, ":", ";");     // sometimes wrong separation mark used. Replace it.
    std::vector<std::string> alldatasegments = Helper::Segment(data, ';');

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
        std::vector<std::string> datasegments = Helper::Segment(fullstring, '<');

        // this part: copied from Field100
        if (datasegments.size() > 1) // item has relatorial remarks
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                std::string tempstring;
                try{
                    tempstring = (*it2).substr(0,(*it2).find_first_of('>'));
                    //first remove trailing and beginning whitespace; for next segmentation to work properly
                    Helper::Trim(tempstring);
                } catch(exception& e)
                {
                    if (verbose) throw MarcRecordException("ERROR Field 700: empty author function.");
                }

                // note: sometimes added in wrong way, with 2 terms in one set of brackets. thus -> segment further
                std::vector<std::string> relatorsegments = Helper::Segment(tempstring, ' ' ); // will still err if no spacing used

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
                        // check that the list conforms to the given list of names
                        if (!isValidRelator(tempstring))
                        {
                            if (verbose) throw MarcRecordException("ERROR Field 700: Unknown author function: "+tempstring);
                        }
                        // to preserve the link with the author $a subfield, everything is internally stored in the
                        // $a subfield as one entire string that is already printed
                        relator = relator + "$e" + t700e.translate(tempstring);
                }
            }
            fullstring = datasegments[0];       // discard all <relator> info from this string
        }


        // the dates are anything in between ()
        datasegments.clear();
        datasegments = Helper::Segment(fullstring, '(');

        if (datasegments.size() > 1) // item has dates
        {
            for (std::vector<std::string>::iterator it2 = datasegments.begin()+1; it2 != datasegments.end(); ++it2)
            {
                // TODO parse in more detailed way
                try{
                    std::string tempstring = (*it2).substr(0,(*it2).find_first_of(')'));
                    //trim any whitespaces out
                    Helper::EraseWhitespace(tempstring);
                    //MarcField::update('d', tempstring);
                    // only put in date if it contains numbers. Else, it's probably a title and should be in $c
                        if (tempstring.find_first_of("0123456789") == tempstring.npos)
                        {
                           dates = dates + "$c" +  tempstring;     // WARNING: called "dates" but could contain a title (with $c)
                        }
                        else
                        {
                           dates = dates + "$d" + tempstring;
                        }
                } catch (exception& e)
                {
                    if (verbose) throw MarcRecordException("ERROR Field 700: error in author dates.");
                }
            }
            fullstring = datasegments[0];       // discard all (date) info from this string
        }

        // now add the author itself.

        try{
            //trim front and trailing whitespace
            Helper::Trim(fullstring);
        } catch (exception e)
        {
            if (verbose) throw MarcRecordException("ERROR Field 700: empty value among authors.");
        }

        // This is done in one line, to preserve the link between the author and its relator terms.
        MarcField::update('a', t700a.translate(fullstring)+dates+relator);
        //MarcField::update('a', fullstring+dates+relator);

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
    return t700e.isKey(data);
    //return (relatorterms.find(data) != relatorterms.end());
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
