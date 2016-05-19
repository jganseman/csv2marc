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
        //cout << "Translator added " << segments[0] << endl;
        // anything beyond the 2nd column is treated as comment
    }
}

Translator::~Translator()
{
    //dtor
}


/** If the original string is mapped to the word "DELETE", empty string is returned.
    Which results in the subfield being dropped (see MarcField.cpp)
*/
std::string Translator::translate(std::string& input)
{
    if (mapping.empty())
        return input;

    std::string result = mapping[input];

    if (result.empty() || result == "")
        return input;
    else if (result == "DELETE")
        return "";
    else
        return result;
}



/** This version does a partial replacement in the given string.
    Watch it: this may match the input string at any location (so not necessarily in the beginning only)
*/
std::string Translator::translatepartial(std::string& input)
{
    if (mapping.empty())
        return input;

    //cout << "Translating " << input << endl;

    // find the first occurrence of any key in the input line
    for(std::map<std::string, std::string>::iterator iter = mapping.begin(); iter != mapping.end(); ++iter)
    {
        if (input.find(iter->first) != input.npos)      // key found in input line!
        {
            //cout << "Found " << iter->first << endl;
            if (iter->second == "DELETE")                   // if this line needs to be deleted: return empty string
                return "";
            if (iter->second != "")
            {
                Helper::ReplaceAll(input, iter->first, iter->second);        // otherwise, just replace by value
                return input;
            }                                              // and exit immediately
        }
    }

    return input; // nothing found? just return the original
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
