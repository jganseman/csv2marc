#include "Field383.h"

// initialize the translation list for relator terms
Translator Field383::t383c("t383c.csv");     // The file must be in the same folder as the executable.


Field383::Field383(int nr) : MarcField(nr)
{
    //ctor
}

Field383::~Field383()
{
    //dtor
}


// notes for field 383:
// $a is a sequential designation (e.g. sonate nr 41). repeatable. not used in B-Bc database
// $b is the opus nr, denoted "op. 12". repeatable.
// $c is a thematic catalogue number. repeatable.
// $d is the thematic catalogue designation . non-repeatable.

void Field383::update(char marcsubfield, std::string data)
{
    //this field contains the opus number
    // and a truckload of errors. Get those out first

    if (data.find("<") != data.npos)
        throw MarcRecordException("ERROR field 383 : instrumentation in opusnr? : " + data);

    if (data.find("gr-t") != data.npos)
        throw MarcRecordException("ERROR field 383 : tonality in opusnr? : " + data);

    if (data.find("kl-t") != data.npos)
        throw MarcRecordException("ERROR field 383 : tonality in opusnr? : " + data);

    // now, assume we can segment on ';' . However, somtimes it's '=' .
    // -> first, replace all equal signs by ;
    Helper::ReplaceAll(data, "=", ";");
    Helper::RemoveAllOf(data, "[]");
    Helper::ReplaceAll(data, "oeuvre", "Opus");
    Helper::ReplaceAll(data, "Oeuvre", "Opus");
    Helper::ReplaceAll(data, "OEuvre", "Opus");
    Helper::ReplaceAll(data, "Opera", "Opus");
    Helper::ReplaceAll(data, "Opéra", "Opus");
    Helper::ReplaceAll(data, "Krebs-WV", "KrebsWV");

    // then segment
    std::vector<std::string> segments = Helper::Segment(data, ';');

    for (std::vector<std::string>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
        // search for a string "opus". then, it is an opus nr.
        if ((*it).find("pus") != string::npos)
        {
            // get everything after the 'pus', trim, remove '-', then add to $b
            std::string opusnr = (*it).substr((*it).find("pus")+3);
            Helper::Remove(opusnr, "-");
            Helper::Remove(opusnr, ".");
                // Note: ']' might be left, from conversion of data between []
            Helper::Trim(opusnr);
            MarcField::update('b', "op. "+opusnr);
            continue;       // immediately move on
        }
        else if ((*it).find("op") != string::npos)
        {
            // get everything after the 'pus', trim, remove '-', then add to $b
            std::string opusnr = (*it).substr((*it).find("op")+2);
            Helper::Remove(opusnr, "-");
            Helper::Remove(opusnr, ".");
            Helper::Remove(opusnr, "]");
            Helper::Trim(opusnr);
            MarcField::update('b', "op. "+opusnr);
            continue;       // immediately move on
        }
        else if ((*it).find("Op") != string::npos)
        {
            // get everything after the 'pus', trim, remove '-', then add to $b
            std::string opusnr = (*it).substr((*it).find("Op")+2);
            Helper::Remove(opusnr, "-");
            Helper::Remove(opusnr, ".");
            Helper::Remove(opusnr, "]");
            Helper::Trim(opusnr);
            MarcField::update('b', "op. "+opusnr);
            continue;       // immediately move on
        }

        // skip everything that is not an opus nr
        if ((*it).find("boek") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));
        if ((*it).find("Boek") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));
        if ((*it).find("book") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));
        if ((*it).find("Book") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));
        if ((*it).find("livre") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));
        if ((*it).find("Livre") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383 : volume info in opusnr : " + (*it));

        // else, it is probably a thematic catalogue nr. Then, first comes the catalog name, then the nr
        // problem: segmentation. No good way, but the following is probably safest:
        // replace all '-' by ' ', then the first is the catalogue; the rest is its number

        // NEW: put thematic catalogue nr entirely in $c . Reserve $d for specifier of catalogue as per the MLA list (mlati, see examples on MARC21 field 383 website)


        Helper::ReplaceAll((*it), "-", " ");
                // check catalogname for spelling
        Helper::ReplaceAll((*it), "HOB", "Hob");
        Helper::ReplaceAll((*it), "Kv", "KV");

        // error on Nr-s
        if ((*it).find("nr") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383: not an opus? : " + (*it));
        if ((*it).find("Nr") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383: not an opus? : " + (*it));
        if ((*it).find("n°") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383: not an opus? : " + (*it));
        if ((*it).find("N°") != string::npos)
            if (verbose) throw MarcRecordException("WARNING field 383: not an opus? : " + (*it));

        Helper::Trim((*it));
        std::vector<std::string> themanrs = Helper::Segment((*it), ' ');
        if (themanrs.size() <= 1)
            if (verbose) throw MarcRecordException("ERROR field 383: unspecified catalogname (no dash/space?): " + (*it));

        MarcField::update('c', t383c.translatepartial(*it));

        /*

        std::string catalogname = themanrs[0];
        std::string catalognr = themanrs[1];
        //std::string catalognr = (*it).substr((*it).find_first_of(" "));

        if ( strncmp(themanrs[1].c_str(), "Anh", 3) == 0)      // if string starts with Anh, it's an Anhang catalogue
        {
            catalogname += themanrs[1];
            Helper::Trim(catalognr);
            catalognr = catalognr.substr(catalognr.find_first_of(" "));
        }

        if ( strncmp(themanrs[1].c_str(), "anh", 3) == 0)      // if string starts with anh
        {
            catalogname += themanrs[1];
            Helper::Trim(catalognr);
            catalognr = catalognr.substr(catalognr.find_first_of(" "));
        }

        Helper::Trim(catalognr);
        MarcField::update('c', catalognr);

        //update catalogname, only if no catalog field already present
        if (Getsubfield('d') == "")
            MarcField::update('d', catalogname);
        */
    }

    // opus number should be in $b, thematic index numbers (like KV, BWV etc) should be in $c

}
