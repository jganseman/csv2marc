#include "Field008.h"

Field008::Field008(int nr) : MarcField(8)
{
    //ctor
    // By default constructed for a score. Use update() function to adapt content

    strcpy(&fixedstring[0], "000000");       // Date of MARC record creation
    fixedstring[6] = '|';                   // no attempt to encode the following date just yet
    strcpy(&fixedstring[7], "||||||||");      // contains year of creation / publication
    strcpy(&fixedstring[15], "xx#");        // place of publication (country code)

    // here begin material specific coding elements. Need to be different for music and books
    // For music, see: http://www.loc.gov/marc/bibliographic/bd008m.html (LEADER[6]: [c|d|i|j])
    // For books, see: http://www.loc.gov/marc/bibliographic/bd008b.html (LEADER[6-7]: [a|t] [a|c|d|m])
    // For serials, see: http://www.loc.gov/marc/bibliographic/bd008s.html (LEADER[6-7]: [a] [b|i|s])

    strcpy(&fixedstring[18], "|||||||||||||||||");      // no attempt to code

    // here are the last fields
    strcpy(&fixedstring[35], "###");        // predominant language of the ITEM
    fixedstring[38] = '|';                   // record is not a romanization
    fixedstring[39] = 'd';                  // cataloged by independent library
    fixedstring[40] = '\0';                 // null terminator


}

Field008::~Field008()
{
    //dtor
}


std::string const Field008::print() const
{
    // overloaded print function for fixed length field
    std::ostringstream output;
    output << "=008\t" << fixedstring << endl;
    return output.str();
}

void Field008::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // this field does not use marcsubfields. It might be possible to use the subfield indicator as an enum to discriminate inputs?

    // mostly we're going to put dates here as subfield.
    if (data.length() == 8 && atoi(data.c_str()))
    {
        for (int i=2; i<8; ++i) {fixedstring[i-2] = data[i];}
    }

    // usually this is called to set a date.

    /* fixed[6] [7-10] [11-14]: if input is something with multiple publication dates, or different record dates etc, adapt acordingly
    b - No dates given; B.C. date involved
    c - Continuing resource currently published
    d - Continuing resource ceased publication
    e - Detailed date
    i - Inclusive dates of collection
    k - Range of years of bulk of collection
    m - Multiple dates
    n - Dates unknown
    p - Date of distribution/release/issue and production/recording session when different
    q - Questionable date
    r - Reprint/reissue date and original date
    s - Single known date/probable date
    t - Publication date and copyright date
    u - Continuing resource status unknown
    | - No attempt to code
    */
    if (data.length() == 4 && atoi(data.c_str()))           //suppose a 4-digit number is a publication date
    {
        fixedstring[6] = 's';
        for (int i=0; i<4; ++i) {fixedstring[i] = data[i];}
    }

    /* fixed[15-17] : place of publication, as a country code
    */

    // fixed[18-34] material specific coding elements. Need to be different for music and books
    // For music, see: http://www.loc.gov/marc/bibliographic/bd008m.html (LEADER[6]: [c|d|i|j])
    // For books, see: http://www.loc.gov/marc/bibliographic/bd008b.html (LEADER[6-7]: [a|t] [a|c|d|m])
    // For serials, see: http://www.loc.gov/marc/bibliographic/bd008s.html (LEADER[6-7]: [a] [b|i|s])

    /* fixed[35-37]: predominant language of the ITEM
    ### - No information provided
    zxx - No linguistic content
    mul - Multiple languages
    sgn - Sign languages
    und - Undetermined
    [aaa] - Three-character alphabetic code
    */

    /* fixed[36] : indicates whether the record is a romanization or not, or contains mathematical characters
    # - Not modified
    d - Dashed-on information omitted
    o - Completely romanized/printed cards romanized
    r - Completely romanized/printed cards in script
    s - Shortened
    x - Missing characters
    | - No attempt to code
    */

}

bool Field008::isempty() const
{
    return false;       // this is always filled in from the constructor
}
