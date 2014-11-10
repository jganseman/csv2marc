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

    // usually this is called to set a date.


    /* fixed[5] : if it has a correction date, set to c (corrected)
    */

    /*
    // fixed[6] : if input is a string which contains manuscript, set to manuscript etc
    a - Language material (regular book)
    c - Notated music
    d - Manuscript notated music
    j - Musical sound recording
    k - Two-dimensional nonprojectable graphic
    m - Computer file
    r - Three-dimensional artifact or naturally occurring object
    t - Manuscript language material (e.g. letters)
    */

    /* fixed[7] : if part of larger unit, set to d and use field 773 to identify superunit (only if superunit needs to be lend out as a whole).
                    if serial, set to t. Do not use a (monograph part), as that is intended for book chapters or individual articles.

    d - Subunit
    m - Monograph/Item
    s - Serial
    */

    /* fixed[19] : Same as above: when set, set to a. When subunit, set to b

    a - Set (only if [7] = m)
    b - Part with independent title (only if [7] = d)
    c - Part with dependent title
    */

}

bool FieldLDR::isempty() const
{
    return false;       // this is always filled in from the constructor
}
