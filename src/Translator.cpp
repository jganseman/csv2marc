#include "Translator.h"

Translator::Translator(std::string filename)
{
    //ctor

    // A translation file is a csv file where the first column is translated into the second column.
    // The file is best placed in the same folder as the executable.

    //open filename
    ifstream csvfile(filename.c_str());
    if ( !csvfile.is_open() ) {
        cout << "Controlled List Warning : Could not open file " + filename << endl;
        return;
    }

    // read in the csv file
    std::string line;
    while(std::getline(csvfile, line))
    {
        if (line.empty() || line == "")
            continue;

        // in Excel, CSV is standard encoded with ";" . IGNORE line if it already contains tab, else, convert all ";" to tab
        if (line.find('\t') != line.npos)
            continue;
        Helper::ReplaceAll(line, ";", "\t");

        vector<std::string> segments = Helper::Segment(line, '\t');
        if (segments.size() == 0)     // line must exist
            continue;

        Helper::Trim(segments[0]);
        keys.insert(segments[0]);

        if (segments.size() > 1)        // translation available
        {
            Helper::Trim(segments[1]);
            if ((segments[1] != "") && (segments[1] != segments[0]))
                values.insert(segments[1]);
        }

        if (segments.size() == 1)           // no translation available. Put in empty string.
            mapping.insert(std::pair<std::string,std::string>(segments[0], ""));
        else
        {
             if (segments[1] != "")          // translation available
             {
                 if (segments[1] != segments[0])        // translation is not same as original: replace
                    mapping.insert(std::pair<std::string,std::string>(segments[0], segments[1]));
                else                           // translation is same as original: put empty string
                    mapping.insert(std::pair<std::string,std::string>(segments[0], ""));
             }

        }

        // anything beyond the 2nd column is treated as comment
    }
}

Translator::~Translator()
{
    //dtor
}

std::string Translator::translate(std::string& input)
{
    if (mapping.empty())
        return input;

    std::string result = mapping[input];

    if (result.empty() || result == "")
        return input;
    else
        return result;
}


bool Translator::isKey(std::string& input)
{
    if (keys.empty()) return false;
    return ( keys.find(input) != keys.end() );
}


bool Translator::isValue(std::string& input)
{
    if (values.empty()) return false;
    return ( values.find(input) != values.end() );
}
