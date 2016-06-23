#include "Field384.h"

Field384::Field384(int nr) : MarcField(nr)
{
    //ctor
}

Field384::~Field384()
{
    //dtor
}


void Field384::update(char marcsubfield, std::string data)
{
    //this field contains the key of the work. Transcode to English here
    if (data.empty() || data == "")
        return;

    // trim beginning and trailing whitespace
    Helper::Trim(data);

    //make all lowercase
    Helper::MakeLowercase(data);

    //segment by ';' to process multiple tonalities
    std::vector<std::string> datasegments = Helper::Segment(data, ';');

    // LEGACY instrumentation field handling
    if (marcsubfield == 'L')
    {
        std::string reparse = "";
        for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
        {
            // trim beginning and trailing whitespace
            Helper::Trim((*it));
            // if it contains "t=", retain
            if ((*it).find("t=") != std::string::npos)
            {
                reparse += (*it);       // add to a string of all tonalities here
                reparse += " ";         // separated by spaces
            }
        }

        // now make a new segmentation of this string, based on spaces
        datasegments.clear();
        datasegments = Helper::Segment(reparse, ' ');

        marcsubfield = 'a'; // reset marcsubfield to std code for tonalities
    }


    // now, find segments that contains the letters 't='
    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        if ((*it).empty())
            continue;

        // replace string "t=" by empty string
        Helper::Remove((*it), "t=");

        // if this still accidentally contains a language, skip it
         if ((*it).find("l=") != std::string::npos)
            continue;

        // trim beginning and trailing whitespace
        Helper::Trim((*it));

        //prepare empty string as new key
        std::string tonality;
        bool flat = false;
        bool sharp = false;
        //bool modal = false;
        bool major = false;          // default major
        bool minor = false;

        //first letter is supposed to be the key
        // add first letters of modes as well, though this isnt foolproof
        if (!( ((*it)[0] == 'a') || ((*it)[0] == 'b') || ((*it)[0] == 'c') || ((*it)[0] == 'd') || ((*it)[0] == 'e') || ((*it)[0] == 'f') || ((*it)[0] == 'g')
            || ((*it)[0] == 'h') || ((*it)[0] == 'p') || ((*it)[0] == 'm') || ((*it)[0] == 'l') || ((*it)[0] == 'i') ) )
        {
            if (verbose) throw MarcRecordException("ERROR Field 384: Unknown Tonality : " + (*it));
        }

        if ((*it)[1] == 'i')     //ais, bis, cis, dis ...
            sharp=true;
        else if (((*it)[1] == 'e') || ((*it)[1] == 's'))      // as, bes, ces, des, es...
            flat=true;

        std::size_t found = (*it).find("gr-t");
        if (found != std::string::npos)
            major=true;

        found = (*it).find("kl-t");
        if (found != std::string::npos)
        {
            major=false; minor=true;
        }


        /*
        found = (*it).find("mod");
        if (found != std::string::npos)
            modal=true;
        */

        // put "hypo-" strings first, to avoid early match on their substrings
        std::string modality = "";
        if ((*it).find("atonaal") != std::string::npos) modality="atonal";
        else if ((*it).find("hypodorisch") != std::string::npos) modality="hypodorian";
        else if ((*it).find("dorisch") != std::string::npos) modality="dorian";
        else if ((*it).find("hypophrygisch") != std::string::npos) modality="hypophrygian";
        else if ((*it).find("phrygisch") != std::string::npos) modality="phrygian";
        else if ((*it).find("hypomixolydisch") != std::string::npos) modality="hypomixolydian";
        else if ((*it).find("mixolydisch") != std::string::npos) modality="mixolydian";
        else if ((*it).find("hypolydisch") != std::string::npos) modality="hypolidian";
        else if ((*it).find("lydisch") != std::string::npos) modality="lydian";
        else if ((*it).find("hypoaeolisch") != std::string::npos) modality="hypoaeolian";
        else if ((*it).find("aeolisch") != std::string::npos) modality="aeolian";
        else if ((*it).find("hypoionisch") != std::string::npos) modality="hypoionian";
        else if ((*it).find("ionisch") != std::string::npos) modality="ionian";
        else if ((*it).find("peregrin") != std::string::npos) modality="tonus peregrinus";

        // this builds the string according to regular text

        tonality += ((*it)[0] -32);      // character arithmetic: subtract 32 to make uppercase
        tonality += ' ';
        if (flat) tonality += "flat ";
        if (sharp) tonality += "sharp ";
        if (major) tonality += "major";
        if (minor) tonality += "minor";
        //if (modal) tonality += "modal";
        if (!(modality.empty()))        // replace entire string by modality; without any key indicator
            tonality=modality;

        // if major and not modal, capitalize first letter
        if (major && modality.empty())
            tonality[0] = toupper(tonality[0]);

        // give warning if key is invalid:
        if ((!major) && (!minor) && (modality.empty()) )
        {
            major=true;
            MarcField::update(marcsubfield, tonality);
            throw MarcRecordException("WARNING Field 384: Cannot determine major/minor/modal: " + (*it));
        }
        else
        {
            MarcField::update(marcsubfield, tonality);
        }


        // this builds the string according to the UNIMARC standard, which is more detailed
        // see http://www.ifla.org/files/assets/uca/unimarc_updates/BIBLIOGRAPHIC/u-b_128_update.pdf
        /*
        tonality += (*it)[0];
        if (flat) tonality += "b";
        if (sharp) tonality += "x";
        if (minor) tonality += "m";
        //if (modal) tonality = "zz";         // define in more detail later: throw warning after update of this field

        if (modality == "atonal") tonality = "zz";
        else if (modality == "dorian") tonality = "01";
        else if (modality == "hypodorian") tonality = "02";
        else if (modality == "phrygian") tonality = "03";
        else if (modality == "hypophrygian") tonality = "04";
        else if (modality == "lydian") tonality = "05";
        else if (modality == "hypolydian") tonality = "06";
        else if (modality == "mixolydian") tonality = "07";
        else if (modality == "hypomixolydian") tonality = "08";
        else if (modality == "aeolian") tonality = "09";
        else if (modality == "hypoaeolian") tonality = "10";
        else if (modality == "ionian") tonality = "11";
        else if (modality == "hypoionian") tonality = "12";
        else if (modality == "tonus peregrinus") tonality = "13";
        */

        /*
        if (modal) throw MarcRecordException("WARNING Field 384: Modal Key. To define later in more detail : " + (*it));
        */
    }
}
