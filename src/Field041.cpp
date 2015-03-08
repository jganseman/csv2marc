#include "Field041.h"

Field041::Field041(int nr) : MarcField(nr)
{
    //ctor
}

Field041::~Field041()
{
    //dtor
}

void Field041::update(char marcsubfield, std::string data)
{
    // warning: this routine might also need to process legacy instrumentation strings.

    if (data.empty() || data == "")
        return;

    // This field contains the languages. Parse and translate to ISO language codes

    // first make all lowercase for ease of parsing
    Helper::MakeLowercase(data);

    //First, segment by ';'
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    //cout << "nrsegments: " << datasegments.size() << endl;

    // LEGACY instrumentation field handling
    if (marcsubfield == 'L')
    {
        std::string reparse = "" ;      // create a string containing only the language component
        for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
        {
            // trim beginning and trailing whitespace
            Helper::Trim((*it));
            // if it doesn't start with "l=", erase
            if ((*it).find("l=") != std::string::npos)
            {
                reparse += (*it);       // add to a string of all languages here
                reparse += " ";         // separated by spaces
            }
        }

        // now make a new segmentation of this string, based on spaces
        datasegments.clear();
        datasegments = Helper::Segment(reparse, ' ');

        marcsubfield = 'a'; // reset marcsubfield to std code for languages
    }


    char a = marcsubfield;
    int langcount=0;
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        if ((*it).empty())
            continue;

        if ((*it).find("nederlands") != std::string::npos)
            MarcField::update(a, "dut");
        else if ((*it).find("engels") != std::string::npos)
            MarcField::update(a, "eng");
        else if ((*it).find("frans-dialect") != std::string::npos)
            MarcField::update(a, "wal");                            // use code for walloon
        else if ((*it).find("frans") != std::string::npos)
            MarcField::update(a, "fra");
        else if ((*it).find("latijn") != std::string::npos)
            MarcField::update(a, "lat");
        else if ((*it).find("italiaans") != std::string::npos)
            MarcField::update(a, "ita");
        else if ((*it).find("duits-dialect") != std::string::npos)
            MarcField::update(a, "nds");                            // using code for plattduutsch.
        else if ((*it).find("duits") != std::string::npos)
            MarcField::update(a, "ger");
        else if ((*it).find("russisch") != std::string::npos)
            MarcField::update(a, "rus");
        else if ((*it).find("spaans") != std::string::npos)
            MarcField::update(a, "spa");
        else if ((*it).find("fins") != std::string::npos)
            MarcField::update(a, "fin");
        else if ((*it).find("grieks") != std::string::npos)
            MarcField::update(a, "gre");
        else if ((*it).find("pools") != std::string::npos)
            MarcField::update(a, "pol");
        else if ((*it).find("deens") != std::string::npos)
            MarcField::update(a, "dan");
        else if ((*it).find("noors") != std::string::npos)
            MarcField::update(a, "nor");
        else if ((*it).find("zweeds") != std::string::npos)
            MarcField::update(a, "swe");
        else if ((*it).find("catalaans") != std::string::npos)
            MarcField::update(a, "cat");
        else if ((*it).find("tsjechisch") != std::string::npos)
            MarcField::update(a, "cze");
        else if ((*it).find("français") != std::string::npos)
            MarcField::update(a, "fra");
        else if ((*it).find("chinees") != std::string::npos)
            MarcField::update(a, "chi");
        else if ((*it).find("scat") != std::string::npos)
            MarcField::update(a, "und");                            // undetermined
        else if ((*it).find("slovaaks") != std::string::npos)
            MarcField::update(a, "slo");
        else if ((*it).find("hongaars") != std::string::npos)
            MarcField::update(a, "hun");
        else if ((*it).find("afrikaans") != std::string::npos)
            MarcField::update(a, "afr");
        else if ((*it).find("allemand") != std::string::npos)
            MarcField::update(a, "ger");
        else if ((*it).find("anglais") != std::string::npos)
            MarcField::update(a, "eng");
        else if ((*it).find("arabisch") != std::string::npos)
            MarcField::update(a, "ara");
        else if ((*it).find("armeens") != std::string::npos)
            MarcField::update(a, "arm");
        else if ((*it).find("portugees") != std::string::npos)
            MarcField::update(a, "por");
        else if ((*it).find("ests") != std::string::npos)
            MarcField::update(a, "est");
        else if ((*it).find("english") != std::string::npos)
            MarcField::update(a, "eng");
        else if ((*it).find("deutsch") != std::string::npos)
            MarcField::update(a, "ger");
        else if ((*it).find("latin") != std::string::npos)
            MarcField::update(a, "lat");
        else if ((*it).find("italien") != std::string::npos)
            MarcField::update(a, "ita");
        else if ((*it).find("auvergnat") != std::string::npos)
            MarcField::update(a, "oci");                            // use code for occitan
        else if ((*it).find("occitaans") != std::string::npos)
            MarcField::update(a, "oci");
        else if ((*it).find("waals") != std::string::npos)
            MarcField::update(a, "wal");
        else if ((*it).find("hebreeuw") != std::string::npos)
            MarcField::update(a, "heb");
        else if ((*it).find("welsh") != std::string::npos)
            MarcField::update(a, "wel");
        else if ((*it).find("jiddisch") != std::string::npos)
            MarcField::update(a, "yid");
        else if ((*it).find("kroatisch") != std::string::npos)
            MarcField::update(a, "cro");
        else if ((*it).find("middelnederlands") != std::string::npos)
            MarcField::update(a, "dum");
        else if ((*it).find("oud-nederlands") != std::string::npos)
            MarcField::update(a, "gem");                                    // use code for germanic languages
        else if ((*it).find("platduits") != std::string::npos)
            MarcField::update(a, "nds");
        else if ((*it).find("roemeens") != std::string::npos)
            MarcField::update(a, "rum");                                // note: rom = romani
        else if ((*it).find("perzisch") != std::string::npos)
            MarcField::update(a, "per");
        else if ((*it).find("slavonisch") != std::string::npos)
            MarcField::update(a, "chu");                                // = church slavic
        else if ((*it).find("sloveens") != std::string::npos)
            MarcField::update(a, "slv");
        else if ((*it).find("turks") != std::string::npos)
            MarcField::update(a, "tur");
        else if ((*it).find("japans") != std::string::npos)
            MarcField::update(a, "jpn");
        else if ((*it).find("lunda") != std::string::npos)
            MarcField::update(a, "lun");
        else if ((*it).find("azeri") != std::string::npos)
            MarcField::update(a, "aze");
        else if ((*it).find("maleis") != std::string::npos)
            MarcField::update(a, "may");
        else if ((*it).find("galici") != std::string::npos)
            MarcField::update(a, "glg");
        else if ((*it).find("fries") != std::string::npos)
            MarcField::update(a, "fry");
        else if ((*it).find("iers") != std::string::npos)
            MarcField::update(a, "gle");
//        else if ((*it).find("romaans") != std::string::npos)
//            MarcField::update(a, "roa");
        else
        {
            // if string does not start with l= , it is not a language and should be skipped. Otherwise, put undefined
            Helper::Trim((*it));
            if ((*it)[0] == 'l')
            {
                MarcField::update(a, "und");
                throw MarcRecordException("WARNING Field 041 : Unrecognized language : " + (*it));
            }
        }
            //MarcField::update(a, "und");
            //throw MarcRecordException("Unrecognized language for Field041: "+data);
        langcount++;
    }

    // set indicators
    Setindicator1('0');             // if one of the languages is a translation, it should be '1'and subfield $h should be used to denote the source language
    Setindicator2(DEFAULT_INDIC);

}
