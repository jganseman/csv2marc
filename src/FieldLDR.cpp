#include "FieldLDR.h"

//check http://www.loc.gov/marc/bibliographic/bdleader.html for meaning of fields

FieldLDR::FieldLDR(int nr) : MarcField(0)
{
    //ctor
    //set default data in the fixed field: a score made today
    strcpy(&fixedstring[0], "     ");       // record length: 5 characters
    fixedstring[5] = 'n';       // record status: new
    fixedstring[6] = 'c';       // type of record: notated music
    fixedstring[7] = 'm';       // bibliographic level: single item
    fixedstring[8] = '#';       // control type: full
    fixedstring[9] = 'a';       // encoding: Unicode
    strcpy(&fixedstring[10], "22     "); // nr of indicators and base address of data (computer fills this in)
    fixedstring[17] = '#';      // full level encoding
    fixedstring[18] = '#';      // Given poor state of ISBD compliance in legacy DB, just say it does not use it.
                                // Otherwise, c = use ISBD punctuation except at end of subfield
    fixedstring[19] = '#';      // Not a set of items or item from a set

    strcpy(&fixedstring[20], "4500");   // fixed
}


FieldLDR::~FieldLDR()
{
    //dtor
}


std::string const FieldLDR::print() const
{
    // overloaded print function for fixed length field
    std::ostringstream output;
    output << "=LDR  " << fixedstring << endl;
    return output.str();
}

void FieldLDR::update(char marcsubfield, std::string data)
{
    // this field does not use marcsubfields. It might be possible to use the subfield indicator as an enum to discriminate inputs?

    if (data.empty() || data == "")
        return;

    // usually this is called to set a date.
    /* fixed[5] : if it has a correction date, set to c (corrected)
    */
    if (atol(data.c_str()))
    {
        fixedstring[5] = 'c';
        return;     // nothing more to do with this input
    }


    /*
    // fixed[6] : if input is a string which contains manuscript, set to manuscript etc
    a - Language material (regular book)
    c - Notated music
    d - Manuscript notated music
    g - Projected medium
    j - Musical sound recording
    k - Two-dimensional nonprojectable graphic
    m - Computer file
    r - Three-dimensional artifact or naturally occurring object
    t - Manuscript language material (e.g. letters)
    */

    // recognize a book by ISBN or ISSN number
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    std::size_t found = data.find("ISBN");
    if (found != std::string::npos)     // we found an ISBN number
    {
        fixedstring[6] = 'a';
        return;         // always good
    }
    found = data.find("ISSN");
    if (found != std::string::npos)     // we found an ISSN number
    {
        fixedstring[6] = 'a';
        fixedstring[7] = 's';       // set as serial
        return;         // always good
    }
    found = data.find("ISMN");
    if (found != std::string::npos)     // we found an ISMN number
    {
        fixedstring[6] = 'c';
        return;         // always good
    }


    // main type of record: order according to importance.
    // Least important first; then overwrite if string contains more important items
    // use marcsubfield m to access this routine!
if (marcsubfield == 'm')
{

    // give a warning if classification could not be found
    bool ok=false;

    found = data.find("VOORWERP");              // 3D objects
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'r';
    }
    found = data.find("KIT");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'r';
    }
    found = data.find("CD-ROM");                // machine data
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'm';
    }
    found = data.find("MACHINE");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'm';
    }
    found = data.find("AFBEELDING");            // image
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'k';
    }
    found = data.find("FOTO");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'k';
    }
    found = data.find("MICROVORM");             // projectables (dvd etc)
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'g';
    }
    found = data.find("DVD");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'g';
    }
    found = data.find("FILM");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'g';
    }
    found = data.find("VIDEO");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'g';
    }
    found = data.find("CD");                    // music (cd, dvd)
    if (found != std::string::npos)
    {
        ok=true;
        if (fixedstring[6] != 'm')
            { fixedstring[6] = 'j'; }      // only if not cd-rom
    }
    found = data.find("LP");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'j';
    }
    found = data.find("CASSETTE");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'j';
    }
    found = data.find("TAPE");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'j';
    }
    found = data.find("GELUID");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'j';
    }

    found = data.find("TEKST");
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'a';
    }
    found = data.find("MUZIEK");            // music is more important than text
    if (found != std::string::npos)
    {
        ok=true; fixedstring[6] = 'c';
    }

    // recognize a manuscript . This is from the Material field
    found = data.find("MANUSC");
    if (found != std::string::npos)
    {
        ok=true;
        if (fixedstring[6] == 'a')
            fixedstring[6] = 't';
        else
            fixedstring[6] = 'd';
    }

    if (!ok)
    {
        throw MarcRecordException("WARNING LDR: could not auto-determine classification. Classified as Score: " + data);
    }

}

    /* fixed[7] : if part of larger unit, set to d and use field 773 to identify superunit (only if superunit needs to be lend out as a whole).
                    if serial, set to t. Do not use a (monograph part), as that is intended for book chapters or individual articles.

    a - Book chapters or items that are physically attached to a larger item
    m - Monograph/Item
    s - Serial - journals, periodicals, transactions, also "numbered monographic series"
    */

    // for now only: if an ISSN is set, make it serial - see above

    //TODO: if ONDERDEEL in inventory nr, set fixed[7] = a and input in 773$y the parent record

    /* fixed[19] : Same as above: when set, set to a. When subunit, set to b
    a - Set (only if [7] = m)
    b - Part with independent title (only if [7] = d)
    c - Part with dependent title
    */

    //TODO: if DEEL in inventory nr, set fixed[7]=m but fixed[19]=b
    // ???

}

bool FieldLDR::isempty() const
{
    return false;       // this is always filled in from the constructor
}
