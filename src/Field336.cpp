#include "Field336.h"

// initialize material terms
std::set<std::string> Field336::init()
{
    int nrterms=80;
    std::string validterms[]={
        "chord progressions",               // for types c and d: printed and handwritten music
        "klavierreductie",
        "lead sheet",
        "partituur",
        "partijen",
        "koorpartituur",
        "reductie",
        "partijen-liedboekprincipe",
        "reductie",
        "tabulatuur",
        "tonic sol-fa",
        "zakpartituur",
        "bladmuziek",                           // this one used for scanned items without mother record! See main.cpp
                                                // total 13

        "brief",                             // for types a and t: printed and handwritten text
        "catalogus tentoonstelling",
        "catalogus veiling",
        "pamflet",
        "poëzie",
        "fotoboek",
        "jaarboek",
        "libretto",
        "overdruk",
        "persknipsel",
        "programma",
        "brochure",
        "cartoonboek",
        "tijdschrift",
        "catalogus",
        "contract",
        "diploma",
        "liedtekst"                         // total 18

        "aquarel",                         // for type k : images
        "gravure",
        "houtsnede",
        "ets",
        "aanplakbiljet",
        "foto",
        "lithografie",
        "prent",
        "prentbriefkaart",
        "reproductie",
        "schilderij",
        "tekening",
        "plan",                          // total 13

        "bas-reliëf",                         // for type r : items
        "beeld",
        "medaille",
        "vaas",
        "plaat",
        "model",
        "bouwdoos",
        "puzzel",
        "spel",                   // total 9

        "CD",                         // for type j : audio
        "minidisc",
        "DVD",
        "geluidsband",
        "geluidscartridge",
        "geluidscassette",
        "grammofoonplaat",
        "LP",                        // total 8

        "filmcassette",               // for type g : film
        "filmrol",
        "videoband",
        "videocartridge",
        "videocassette",
        "videoplaat",
        "DVD",                      // total 7

        "cassetteband",               // for type m : machinereadable
        "CD-rom",               // note: in English capitalized, in nl not.
        "magneetband",
        "ponsband",
        "diskette",
        "HD",
        "ponskaart",
        "microfiche",
        "microfilm",
        "microfilmstrook",
        "microkaart",
        "mp3"                   // total 12

        };
    std::set<std::string> tmp(validterms, validterms+nrterms);

    return tmp;
 }

// initialize that list as static member of class
std::set<std::string> Field336::validterms(init());


Field336::Field336(int nr) : MarcField(nr)
{
    //ctor
    //verbose=false;
}

Field336::~Field336()
{
    //dtor
}


void Field336::update(char marcsubfield, std::string data)
{
    //this field contains the top level material description. It needs to be in sync with the LDR material parsing routine
    if (data.empty() || data == "")
        return;

    //replace some common mistakes
    fixTerminology(data);

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // now, process each segment separately
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        char mattype = extracttype((*it));
        if (mattype == 'x')
            if (verbose) throw MarcRecordException("ERROR Field 336: could not auto-determine classification. Classified as Score: " + data);

        // now that we've defined the type, we can throw the first part out.
        std::vector<std::string> sepcolon = Helper::Segment((*it), ':');

        // if there was no colon, continue with the next semicolon-separated segment.
        if (sepcolon.size() == 1)
            continue;

        // else, treat all following data as separate items
        for (std::vector<std::string>::iterator it2 = sepcolon.begin()+1; it2 != sepcolon.end(); ++it2)
        {
            std::string thissection = (*it2);
            Helper::Remove(thissection, "autografisch");        // remove "autografisch" as error
            Helper::Trim(thissection);

            // whatever remains should be one of the valid types
            if (!isValidTerm(thissection))
                if (verbose) throw MarcRecordException("ERROR Field 336: invalid material: " + (*it));
        }

        // if validated, store everything after first semicolon
        std::string cleaneddata = (*it).substr((*it).find_first_of(':')+1);
        Helper::Trim(cleaneddata);

        MarcField::update(marcsubfield, cleaneddata);
    }
}


char Field336::extracttype(std::string data)
{
    // give a warning if classification could not be found
    char mattype='x';

    Helper::MakeUppercase(data);

    size_t found = data.find("VOORWERP");              // 3D objects
    if (found != std::string::npos) mattype='r';

    found = data.find("KIT");
    if (found != std::string::npos) mattype='r';

    found = data.find("CD-ROM");                // machine data
    if (found != std::string::npos) mattype='m';

    found = data.find("MACHINE");
    if (found != std::string::npos) mattype='m';

    found = data.find("MICROVORM");
    if (found != std::string::npos) mattype='m';

    found = data.find("AFBEELDING");            // image
    if (found != std::string::npos) mattype='k';

    found = data.find("FOTO");
    if (found != std::string::npos) mattype='k';

    found = data.find("DVD");                   // projectables (dvd etc)
    if (found != std::string::npos) mattype='g';

    found = data.find("FILM");
    if (found != std::string::npos) mattype='g';

    found = data.find("VIDEO");
    if (found != std::string::npos) mattype='g';

    found = data.find("CD");                    // music (cd, dvd)
    if (found != std::string::npos)
    {
        if (mattype != 'm')
            { mattype='j'; }      // only if not cd-rom
    }

    found = data.find("LP");
    if (found != std::string::npos) mattype='j';

    found = data.find("CASSETTE");
    if (found != std::string::npos) mattype='j';

    found = data.find("TAPE");
    if (found != std::string::npos) mattype='j';

    found = data.find("GELUID");
    if (found != std::string::npos) mattype='j';

    found = data.find("TEKST");
    if (found != std::string::npos) mattype='a';

    found = data.find("MUZIEK");            // music is more important than text
    if (found != std::string::npos) mattype='c';

    // recognize a manuscript . This is from the Material field
    found = data.find("MANUSC");
    if (found != std::string::npos)
    {
        if (mattype == 'a')
            mattype = 't';
        else
            mattype = 'd';
    }

    return mattype;
}


bool Field336::isValidTerm(std::string& data)
{
    return (validterms.find(data) != validterms.end());
}

void Field336::fixTerminology(std::string& data)
{
    Helper::ReplaceAll(data, "autograaf : partituur", "partituur autografisch");
    Helper::ReplaceAll(data, "autograaf : partijen", "partijen autografisch");
    Helper::ReplaceAll(data, "pianopartituur", "partituur");
    Helper::ReplaceAll(data, "vioolpartituur", "partituur");
    Helper::ReplaceAll(data, "Partituur", "partituur");
    Helper::ReplaceAll(data, "Lead sheet", "lead sheet");
    Helper::ReplaceAll(data, "Tijdschrift", "tijdschrift");
    Helper::ReplaceAll(data, "cd", "CD");
    Helper::ReplaceAll(data, "CD-ROM", "CD-rom");
    Helper::ReplaceAll(data, "prentkaart", "prentbriefkaart");
    Helper::ReplaceAll(data, "partijen liedboekprincipe", "partijen-liedboekprincipe");
    Helper::ReplaceAll(data, "catalogus-tentoonstelling", "catalogus tentoonstelling");
    Helper::ReplaceAll(data, "catalogus-veiling", "catalogus veiling");
    Helper::ReplaceAll(data, "gedichten", "poëzie");
    Helper::ReplaceAll(data, "brieven", "brief");
    Helper::ReplaceAll(data, "sol fa", "sol-fa");
    Helper::ReplaceAll(data, "partituren", "partituur");
    Helper::ReplaceAll(data, "'s", "");             // remove plurals
    Helper::ReplaceAll(data, "+", ":");
    Helper::ReplaceAll(data, "partij ", "partijen ");
    Helper::ReplaceAll(data, "tekstblad", "liedtekst");
    Helper::ReplaceAll(data, "plannen", "plan");
}
