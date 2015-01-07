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
    output << "=008  " << fixedstring << endl;
    return output.str();
}

void Field008::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // this field does not use marcsubfields. It might be possible to use the subfield indicator as an enum to discriminate inputs?

    // mostly we're going to put dates here as subfield. Only 6 chars of date -> delete 19/20 in beginning
    if (data.length() == 8 && atoi(data.c_str()))
    {
        for (int i=2; i<8; ++i) {fixedstring[i-2] = data[i];}
        return;             // only set this field
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
        for (int i=0; i<4; ++i) {fixedstring[i+7] = data[i];}
        return;             // only set this field
    }
    // also process date ranges, separated by "-" or " - "
    if (data.length() == 9 && atoi(data.substr(0,4).c_str()) && atoi(data.substr(5,9).c_str()) )
    {
        fixedstring[6] = 'm';
        for (int i=0; i<4; ++i) {fixedstring[i+7] = data[i];}
        for (int i=0; i<4; ++i) {fixedstring[i+11] = data[i+5];}
        return;
    }
    if (data.length() == 11 && atoi(data.substr(0,4).c_str()) && atoi(data.substr(7,11).c_str()) )
    {
        fixedstring[6] = 'm';
        for (int i=0; i<4; ++i) {fixedstring[i+7] = data[i];}
        for (int i=0; i<4; ++i) {fixedstring[i+11] = data[i+7];}
        return;
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
    // to set the language, this is a copy of Field041

    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, ';'))
    {
        datasegments.push_back(segment);
    }
    if (datasegments.size() > 1)
    {
        strcpy(&fixedstring[35], "mul"); fixedstring[38] = '|';     // reset char38 to remove null character of string terminator
        return;
    }

    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        if ((*it).find("nederlands") != std::string::npos)
            strcpy(&fixedstring[35], "dut");
        else if ((*it).find("engels") != std::string::npos)
            strcpy(&fixedstring[35], "eng");
        else if ((*it).find("frans-dialect") != std::string::npos)
            strcpy(&fixedstring[35], "wal");                            // use code for walloon
        else if ((*it).find("frans") != std::string::npos)
            strcpy(&fixedstring[35], "fra");
        else if ((*it).find("latijn") != std::string::npos)
            strcpy(&fixedstring[35], "lat");
        else if ((*it).find("italiaans") != std::string::npos)
            strcpy(&fixedstring[35], "ita");
        else if ((*it).find("duits-dialect") != std::string::npos)
            strcpy(&fixedstring[35], "nds");                            // using code for plattduutsch.
        else if ((*it).find("duits") != std::string::npos)
            strcpy(&fixedstring[35], "ger");
        else if ((*it).find("russisch") != std::string::npos)
            strcpy(&fixedstring[35], "rus");
        else if ((*it).find("spaans") != std::string::npos)
            strcpy(&fixedstring[35], "spa");
        else if ((*it).find("fins") != std::string::npos)
            strcpy(&fixedstring[35], "fin");
        else if ((*it).find("grieks") != std::string::npos)
            strcpy(&fixedstring[35], "gre");
        else if ((*it).find("pools") != std::string::npos)
            strcpy(&fixedstring[35], "pol");
        else if ((*it).find("deens") != std::string::npos)
            strcpy(&fixedstring[35], "dan");
        else if ((*it).find("noors") != std::string::npos)
            strcpy(&fixedstring[35], "nor");
        else if ((*it).find("zweeds") != std::string::npos)
            strcpy(&fixedstring[35], "swe");
        else if ((*it).find("catalaans") != std::string::npos)
            strcpy(&fixedstring[35], "cat");
        else if ((*it).find("tsjechisch") != std::string::npos)
            strcpy(&fixedstring[35], "cze");
        else if ((*it).find("français") != std::string::npos)
            strcpy(&fixedstring[35], "fra");
        else if ((*it).find("chinees") != std::string::npos)
            strcpy(&fixedstring[35], "chi");
        else if ((*it).find("scat") != std::string::npos)
            strcpy(&fixedstring[35], "und");                            // undetermined
        else if ((*it).find("slovaaks") != std::string::npos)
            strcpy(&fixedstring[35], "slo");
        else if ((*it).find("hongaars") != std::string::npos)
            strcpy(&fixedstring[35], "hun");
        else if ((*it).find("afrikaans") != std::string::npos)
            strcpy(&fixedstring[35], "afr");
        else if ((*it).find("allemand") != std::string::npos)
            strcpy(&fixedstring[35], "ger");
        else if ((*it).find("anglais") != std::string::npos)
            strcpy(&fixedstring[35], "eng");
        else if ((*it).find("arabisch") != std::string::npos)
            strcpy(&fixedstring[35], "ara");
        else if ((*it).find("armeens") != std::string::npos)
            strcpy(&fixedstring[35], "arm");
        else if ((*it).find("portugees") != std::string::npos)
            strcpy(&fixedstring[35], "por");
        else if ((*it).find("ests") != std::string::npos)
            strcpy(&fixedstring[35], "est");
        else if ((*it).find("english") != std::string::npos)
            strcpy(&fixedstring[35], "eng");
        else if ((*it).find("deutsch") != std::string::npos)
            strcpy(&fixedstring[35], "ger");
        else if ((*it).find("latin") != std::string::npos)
            strcpy(&fixedstring[35], "lat");
        else if ((*it).find("italien") != std::string::npos)
            strcpy(&fixedstring[35], "ita");
        else if ((*it).find("auvergnat") != std::string::npos)
            strcpy(&fixedstring[35], "oci");                            // use code for occitan
        else if ((*it).find("occitaans") != std::string::npos)
            strcpy(&fixedstring[35], "oci");
        else if ((*it).find("waals") != std::string::npos)
            strcpy(&fixedstring[35], "wal");
        else if ((*it).find("hebreeuw") != std::string::npos)
            strcpy(&fixedstring[35], "heb");
        else if ((*it).find("welsh") != std::string::npos)
            strcpy(&fixedstring[35], "wel");
        else if ((*it).find("jiddisch") != std::string::npos)
            strcpy(&fixedstring[35], "yid");
        else if ((*it).find("kroatisch") != std::string::npos)
            strcpy(&fixedstring[35], "cro");
        else if ((*it).find("middelnederlands") != std::string::npos)
            strcpy(&fixedstring[35], "dum");
        else if ((*it).find("oud-nederlands") != std::string::npos)
            strcpy(&fixedstring[35], "gem");                                    // use code for germanic languages
        else if ((*it).find("platduits") != std::string::npos)
            strcpy(&fixedstring[35], "nds");
        else if ((*it).find("roemeens") != std::string::npos)
            strcpy(&fixedstring[35], "rum");                                // note: rom = romani
        else if ((*it).find("perzisch") != std::string::npos)
            strcpy(&fixedstring[35], "per");
        else if ((*it).find("slavonisch") != std::string::npos)
            strcpy(&fixedstring[35], "chu");                                // = church slavic
        else if ((*it).find("sloveens") != std::string::npos)
            strcpy(&fixedstring[35], "slv");
        else if ((*it).find("turks") != std::string::npos)
            strcpy(&fixedstring[35], "tur");
        else {
            //strcpy(&fixedstring[35], "und");
            //throw MarcRecordException("Unrecognized language for Field041: "+data);
        }
        fixedstring[38] = '|';
    }


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
