#include "Field382.h"

// STATIC: list of valid instruments, to check against
std::set<std::string> Field382::init()
{
    unsigned int nrterms=211;
    std::string validterms[]={
        "C",
        "S",
        "Mz",
        "A",
        "Ct",
        "T",
        "Bar",
        "B",
        "Cf",
        "Qp",
        "Sp",
        "Stp",
        "Hs",
        "Ms",
        "Ls",
        "Zs",
        "Zs-vrouw",
        "Zs-man",
        "Spreekstem",      // type V : vocal . subtotal 19

        "koor",
        "gemengd",
        "mannen",
        "vrouwen",
        "kinder",
        "meisjes",
        "knapen",
        "unisono",
        "gelijk",
        "spreek",           //Type koor. subtotal 10. NOTE: optionally followed by SATB-spreek-nr

        "vi-pic",
        "vi",
        "av",
        "tv",
        "vc-pic",
        "vc",
        "cb",
        "va",
        "g",
        "g-s",
        "g-a",
        "bar",
        "g-b",
        "g-t",
        "g-cb",
        "vedel",
        "str",
        "evi",
        "eav",
        "evc",
        "ecb",
        "violetta",         // type str. subtotal 22

        "fl-pic",
        "fl",
        "ho",
        "eh",
        "kl-pic",
        "kl",
        "kl-b",
        "kl-cb",
        "fa",
        "fa-cb",
        "sax",
        "sax-m",
        "sax-a",
        "sax-t",
        "sax-bar",
        "sax-b",
        "blfl",
        "blfl-t",
        "fla",
        "oa",
        "oc",
        "bh",
        "kh",
        "zink",
        "shakuhachi",
        "fijfer",
        "schalmei",
        "sho",
        "panfluit",
        "ocarina",
        "hout",         // type hout. subtotaal 31

        "hn",
        "hn-a",
        "hn-t",
        "tpt-pic",
        "tpt",
        "trbn",
        "trbn-b",
        "tb",
        "tb-t",
        "tb-b",
        "tb-cb",
        "tb-hn",
        "bu-pic",
        "bu",
        "cor",
        "ophi",
        "euph",
        "saxh",
        "klar",
        "kop",
        "cimbasso",     // type kop. subtotaal 21

        "bla",      // type bla. subtotal 1

        "timp",
        "trom",
        "drum",
        "div",
        "mar",
        "vib",
        "xyl",
        "cim",
        "glock",
        "tria",
        "tamtam",
        "djembe",
        "bodhran",
        "dundun",
        "tsuzumi",
        "zweep",
        "buisklok",
        "tomtom",
        "klok",
        "xylorimba",
        "body",         // type perc . subtotaal 21

        "kv",
        "pf",
        "klc",
        "klvd",
        "org",
        "harm",
        "cel",
        "spinet",
        "virginaal",
        "midikeyboard",
        "eorg",
        "epf",          //type toets. subtotal 12

        "ha",
        "gi",
        "egi",
        "egi-b",
        "man",
        "bj",
        "luit",
        "luit-b",
        "cister",
        "lier",
        "mandora",
        "zither",
        "ukelele",
        "tok",
        "balalaika",
        "koto",
        "biwa",
        "cimbalon",
        "orpharion",    // type tok. subtotal: 19

        "om",
        "synth",
        "tape",
        "hifi",
        "microfoon",
        "versterker",
        "luidspreker",
        "sequencer",
        "elek",
        "radio",
        "comp",
        "sampler",
        "video",
        "CD",        // type elek. subtotal : 14

        "bc",           // type beg: 1

        "acc",
        "bandoneon",
        "bei",
        "doedelzak",
        "draailier",
        "etno",
        "acteur",
        "danser",
        "publiek",
        "soundengineer",
        "speeldoos",
        "glasharmonica",
        "alpenhoorn",
        "bajan",
        "telefoon",
        "licht",
        "melodica",
        "mondharmonica",
        "dirigent",
        "platenspeler",     // type varia. subtotal: 20

        "instr",        // type instr. subtotal: 1

        "kamer",
        "symfonie",
        "strijk",
        "blaas",
        "harmonie",
        "slagw",
        "renais",
        "barok",
        "jazz",
        "volks",
        "salon",
        "trombone",
        "accordeon",
        "klarinet",
        "saxofoon",
        "cello",
        "gamelan",
        "tokkel",
        "ensemble"      // type orkest . subtotal: 19 . NOTE: may be followed by telnr specification

        };
    // for ease of checking: remove all '-' characters in this list
    /*
    for (unsigned int i=0; i<nrterms; ++i)
    {
        Helper::ReplaceAll(validterms[i], "-", "");
    }
    */

    std::set<std::string> tmp(validterms, validterms+nrterms);

    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field382::validterms(init());




Field382::Field382(int nr) : MarcField(nr)
{
    //ctor
    //verbose=false;
}

Field382::~Field382()
{
    //dtor
}


void Field382::update(char marcsubfield, std::string data)
{
    //this field contains details on the instrumentation. If wanted, parse here, and check correctness
    if (data.empty() || data == "")
        return;

    // several parallel instrumentations are possible, separated by ';'
    std::vector<std::string> alldatasegments = Helper::Segment(data, ';');

    std::string number = "";        // placeholder for correct number
    std::string general = "";       // placeholder for correct general description
    std::string details = "";

    // each segment should be of the form: "number : highlevel < details >"
    for (std::vector<std::string>::iterator it = alldatasegments.begin(); it != alldatasegments.end(); ++it)
    {
        std::string instrumentation = *it;
        // remove beginning and trailing whitespaces
        Helper::Trim(instrumentation);


        // LEGACY subroutine: this might contain t=tonality or l=language style strings. Give warning when so
        if (instrumentation.empty() || instrumentation == "")
            if (verbose) throw MarcRecordException("WARNING Field 382: possibly semicolon too much");
                // only throw ERR when multiple present, since those are separated by spaces and not by semicolons
        if (instrumentation[0] == 't') continue;
        if (instrumentation[0] == 'l') continue;
            // Handling of LEGACY tonalities and languages now done using subfield L in fields 041 and 384

        // up next: fixing some often occurring spelling mistakes
        Helper::ReplaceAll(instrumentation, "toest", "toets");
        Helper::ReplaceAll(instrumentation, "spreekstem", "Spreekstem");

        // get string until first ':'
        std::size_t found = instrumentation.find_first_of(":");
        if (found==std::string::npos && instrumentation != "N")     // some pieces are for N unspecified instr
            if (verbose) throw MarcRecordException("ERROR Field 382: no quantity of instruments: " + instrumentation);

        number = instrumentation.substr(0, found);
        // remove all spaces
        Helper::EraseWhitespace(number);

        // first character must be a number or N
        found = number.find_first_of("123456789N");
        if (found==std::string::npos)
            if (verbose) throw MarcRecordException("ERROR Field 382: invalid quantity of instruments: " + instrumentation);
        //if last character is N and string is longer than 1, then second-to-last must be '+'
        if (number.length() > 1 && number[number.length()-1] == 'N')
        {
            if (number[number.length()-2] != '+')
                if (verbose) throw MarcRecordException("ERROR Field 382: malformed quantity of instruments: " + instrumentation);
        }

        // NOTE : here we know the number is correctly formed.
        // Move on to the general description in between : and <

        found = instrumentation.find_first_of(":");
        // if no general instrumentation is provided, then only the number of instruments is present. Then skip all the following
        if (found==std::string::npos)
        {
            MarcField::update(marcsubfield, number);
            continue;
        }

        std::size_t found2 = instrumentation.find_first_of("<");
        general = instrumentation.substr(found+1, found2-(found+1));      // 2nd param is length!
        // remove beginning and trailing whitespaces
        Helper::Trim(general);
        if (general.empty() || general == "")
            if (verbose) throw MarcRecordException("ERROR Field 382: no general instrumentation description: " + instrumentation);

        // segment by space
        std::vector<std::string> gensegments = Helper::Segment(general, ' ');

        // iterate over segments, make sure the 14 categories appear in the right order ...
        std::string options[] = {"V", "koor", "str", "hout", "kop", "bla", "perc", "toets", "tok", "elek", "beg", "varia", "instr", "orkest"};
        int optioncount = 14;

// code below is a routine that enforces the order of occurence as in the array above

        unsigned int cursegment = 0;
        for (int i=0; i<optioncount; ++i)
        {
            unsigned int len = options[i].size();
                // compare begin of segment with the currently looked at option
            if (gensegments[cursegment].substr(0, len).compare(options[i]) == 0)
            {
                // This segment starts with option i. Approved.
                // Check if the last character is a number or N
                    // NOTE: absence of number indicates unknown, not error.
                //found = gensegments[cursegment].find_last_of("1234567890N");
                //if (found == std::string::npos)
                //    if (verbose) throw MarcRecordException("ERROR Field 382: no quantity after " + gensegments[cursegment]);

                //Move to the next segment.
                cursegment++;
                //When that was the last segment: break the loop
                if (cursegment == gensegments.size())
                    break;
            }

        }

        if (cursegment != gensegments.size())
            if (verbose) throw MarcRecordException("WARNING Field 382: wrong category order: " + general);



// this version does not enforce order
/*
        unsigned int recognized = 0;
        for (std::vector<std::string>::iterator jt = gensegments.begin(); jt!=gensegments.end(); ++jt)
        {
            Helper::Trim((*jt));        // trim whitespace of this segment
            for (int k=0; k<optioncount; ++k)
            {
                unsigned int len = options[k].size();
                if ((*jt).substr(0, len).compare(options[k]) == 0)
                    recognized++;
            }
        }

        if (recognized < gensegments.size()) {
            if (verbose) throw MarcRecordException("ERROR Field 382: wrong category in: " + general);
        } else if (recognized > gensegments.size()) {
            if (verbose) throw MarcRecordException("ERROR Field 382: excess category in: " + general);
        }
*/
// end of second version

        found = instrumentation.find_first_of("<");

        if (found != std::string::npos)     // only proceed if detailed info is present:
        {
            found2 = instrumentation.find_first_of(">");
            details = instrumentation.substr(found+1, found2-(found+1));          // 2nd param in length!
            if (details.empty() || details == "")
                if (verbose) throw MarcRecordException("ERROR Field 382: no detailed instrumentation description: " + instrumentation);

            //do a few search-and-replace operations for often occurring errors or double codes
            Helper::ReplaceAll(details, "gi-e", "egi");
            Helper::ReplaceAll(details, "gi-e-b", "egi-b");
            // Decision by Johan: replace strijk by kamer, blaas by harmonie
            Helper::ReplaceAll(details, "strijk-", "kamer-");
            Helper::ReplaceAll(details, "blaas-", "harmonie-");

            // remove beginning and trailing whitespaces
            Helper::Trim(details);

            // check validity of all instruments that occur in here.
            std::vector<std::string> instruments = Helper::Segment(details, ' ');
            for (std::vector<std::string>::iterator kt = instruments.begin(); kt!=instruments.end(); ++kt)
            {
                std::string curinstrument = (*kt);
                // remove all numbers, points and brackets
                Helper::RemoveAllOf(curinstrument, "1234567890.()");
                // now, the - should go out of the symphonies, but remain in the others...
                //temporary solution: remove all but the first occurrence of '-'
                size_t firstocc = curinstrument.find_first_of('-');
                size_t lastocc = curinstrument.find_last_of('-');
                while (firstocc != lastocc)
                {
                    curinstrument.erase(lastocc,1);
                    lastocc = curinstrument.find_last_of('-');
                }

                if (curinstrument.empty() || curinstrument == "")
                    if (verbose) throw MarcRecordException("ERROR Field 382: empty instrument in: " + instrumentation);
                if (!isValidTerm(curinstrument))
                    if (verbose) throw MarcRecordException("ERROR Field 382: invalid instrument: " + curinstrument);
            }


        }
        else
        {
            details = "";
        }

        // compose correctly formed instrumentation string and input in MARC
        // one for every ;-separated instrumentation
        std::string goodstring = "";
        goodstring += number;
        goodstring += " : ";
        goodstring += general;
        goodstring += " < ";
        goodstring += details ;
        goodstring += " >";

        MarcField::update(marcsubfield, goodstring);

        // for certainty: make strings empty again for the next instrumentation string parsing
        number = "";
        general = "";
        details = "";
        goodstring = "";

    }

}



bool Field382::isValidTerm(std::string& data)
{
    // routine that decides if the string BEGINS with a valid instrument
    /*
    for (std::set<std::string>::iterator it = validterms.begin(); it != validterms.end(); ++it)
    {
        std::string temp = (*it);
        if ( data.find(temp) == 0 )
            return true;
    }
    return false;
    */
    // alternatively: routine that decides if the string is EXACTLY as a valid instrument
    // NOTE: not usable for telephone nr specification of orchestras or choirs, so lift that out first

    std::string datacopy = data;
    datacopy = datacopy.substr(0, datacopy.find_first_of("-"));
    return (validterms.find(datacopy) != validterms.end());

}

void Field382::fixTerminology(std::string& data)
{
    //Helper::ReplaceAll(data, "autograaf : partituur", "partituur autografisch");
}

