#include "Field382.h"

Field382::Field382(int nr) : MarcField(nr)
{
    //ctor
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
    std::vector<std::string> alldatasegments;
    std::stringstream alldatastream(data);
    std::string allsegment;
    while(std::getline(alldatastream, allsegment, ';'))
    {
        alldatasegments.push_back(allsegment);
    }

    std::string number = "";        // placeholder for correct number
    std::string general = "";       // placeholder for correct general description
    std::string details = "";

    // each segment should be of the form: "number : highlevel < details >"
    for (std::vector<std::string>::iterator it = alldatasegments.begin(); it != alldatasegments.end(); ++it)
    {
        std::string instrumentation = *it;
        // remove beginning and trailing whitespaces
        instrumentation = instrumentation.erase(instrumentation.find_last_not_of(" \n\r\t")+1).substr(instrumentation.find_first_not_of(" \n\r\t"));


        // LEGACY subroutine: this might contain t=tonality or l=language style strings. Give warning when so
        if (instrumentation.empty() || instrumentation == "")
            throw MarcRecordException("WARNING Field 382: possibly semicolon too much");
                // only throw ERR when multiple present, since those are separated by spaces and not by semicolons
        if (instrumentation[0] == 't') continue;
        /*
            { if (instrumentation.size() > 10)
                throw MarcRecordException("ERROR Field 382: Instrumentation contains tonalities: " + instrumentation);
              else
                continue;
            }
            */
        if (instrumentation[0] == 'l') continue;
        /*
            { if (instrumentation.size() > 12)
                throw MarcRecordException("ERROR Field 382: Instrumentation contains languages: " + instrumentation);
              else
                continue;
            }
            */
        // Handling of LEGACY tonalities and languages now done using subfield L in fields 041 and 384

        // up next: fixing some often occurring spelling mistakes
            // --> also been fixed already
        /*
        if (instrumentation.find("toest") != std::string::npos)
            instrumentation.replace(instrumentation.find("toest"), 5, "toets");
            */

        // get string until first ':'
        std::size_t found = instrumentation.find_first_of(":");
        if (found==std::string::npos && instrumentation != "N")     // some pieces are for N unspecified instr
            throw MarcRecordException("ERROR Field 382: no quantity of instruments: " + instrumentation);

        number = instrumentation.substr(0, found);
        // remove all spaces
        number.erase(remove_if(number.begin(), number.end(), ::isspace), number.end());
        // make uppercase

        // first character must be a number or N
        found = number.find_first_of("123456789N");
        if (found==std::string::npos)
            throw MarcRecordException("ERROR Field 382: invalid quantity of instruments: " + instrumentation);
        //if last character is N and string is longer than 1, then second-to-last must be '+'
        if (number.length() > 1 && number[number.length()-1] == 'N')
        {
            if (number[number.length()-2] != '+')
                throw MarcRecordException("ERROR Field 382: malformed quantity of instruments: " + instrumentation);
        }

        // NOTE : here we know the number is correctly formed.
        // Move on to the general description in between : and <

        found = instrumentation.find_first_of(":");

        std::size_t found2 = instrumentation.find_first_of("<");
        general = instrumentation.substr(found+1, found2-(found+1));      // 2nd param is length!
        if (general.empty() || general == "")
            throw MarcRecordException("ERROR Field 382: no general instrumentation description: " + instrumentation);

        // remove beginning and trailing whitespaces
        general = general.erase(general.find_last_not_of(" \n\r\t")+1).substr(general.find_first_not_of(" \n\r\t"));


        // segment by space
        std::vector<std::string> gensegments;
        std::stringstream gendatastream(general);
        std::string gensegment;
        while(std::getline(gendatastream, gensegment, ' '))
        {
            // remove any remaining spaces in this particular segment
            gensegment.erase(remove_if(gensegment.begin(), gensegment.end(), ::isspace), gensegment.end());
            gensegments.push_back(gensegment);
        }

        // iterate over segments, make sure the 14 categories appear in the right order ...
        std::string options[] = {"V", "koor", "str", "hout", "kop", "bla", "perc", "toets", "tok", "elek", "beg", "varia", "instr", "orkest"};
        int optioncount = 14;

// code below is a routine that enforces the order of occurence as in the array above
        /*
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
                //    throw MarcRecordException("ERROR Field 382: no quantity after " + gensegments[cursegment]);

                //Move to the next segment.
                cursegment++;
                //When that was the last segment: break the loop
                if (cursegment == gensegments.size())
                    break;
            }

        }

        if (cursegment != gensegments.size())
            throw MarcRecordException("ERROR Field 382: wrong category or ordering: " + general);
        */

// this version does not enforce order
        //int nrofsegments = gensegments.size();
        unsigned int recognized = 0;
        for (std::vector<std::string>::iterator jt = gensegments.begin(); jt!=gensegments.end(); ++jt)
        {
            for (int k=0; k<optioncount; ++k)
            {
                unsigned int len = options[k].size();
                if ((*jt).substr(0, len).compare(options[k]) == 0)
                    recognized++;
            }
        }

        if (recognized < gensegments.size())
            throw MarcRecordException("ERROR Field 382: wrong category in: " + general);
        else if (recognized > gensegments.size())
            throw MarcRecordException("ERROR Field 382: excess category in: " + general);




        found = instrumentation.find_first_of("<");

        if (found != std::string::npos)     // only proceed if detailed info is present:
        {
            found2 = instrumentation.find_first_of(">");
            details = instrumentation.substr(found+1, found2-(found+1));          // 2nd param in length!
            if (details.empty() || details == "")
                throw MarcRecordException("ERROR Field 382: no detailed instrumentation description: " + instrumentation);
            // remove beginning and trailing whitespaces
            details = details.erase(details.find_last_not_of(" \n\r\t")+1).substr(details.find_first_not_of(" \n\r\t"));
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
