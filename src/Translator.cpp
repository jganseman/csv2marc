#include "Translator.h"

Translator::Translator(std::string filename)
{
    //Parsing a translation file happens in the constructor of the Translator object.

    // A translation file is a csv file where the first column is translated into the second column.
    // The file is best placed in the same folder as the executable.
    // Tab-delimited CSVs work best

    // if no filename is given, load an empty translation table
    if (filename == "") return;


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

        // in Excel, CSV is standard encoded with ";" . If the line does not contain tabs, to be sure, convert the first ";" to tab
        if (line.find('\t') == line.npos)
        {
            if (line.find_first_of(';') != line.npos)
                line[line.find_first_of(';')] = '\t';
        }

        vector<std::string> segments = Helper::Segment(line, '\t');

        // Assert that the line exists. If empty, move on!
        if (segments.size() == 0)
            continue;

        // The line exists! Even if there is no translation available, put it in as key
        Helper::Trim(segments[0]);
        keys.insert(segments[0]);

        // There is a translation available! Load the translation as value, if not equal to original
        if (segments.size() > 1)
        {
            Helper::Trim(segments[1]);
            if ((segments[1] != "") && (segments[1] != segments[0]))
                values.insert(segments[1]);
        }

        // Now store the key-value pair in the mapping:
        if (segments.size() == 1)           // if no translation available, map to empty string
            mapping.insert(std::pair<std::string,std::string>(segments[0], ""));
        else
        {
             if (segments[1] != "")          // if translation available, map to translation if not equal to original
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
