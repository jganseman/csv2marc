#include "MarcRecord.h"


// define the static members of this class
std::map<int, int> MarcRecord::fieldmap;
bool MarcRecord::fieldmaploaded = false;


MarcRecord::MarcRecord()
{
    //ctor
    csvline = "";
}

MarcRecord::MarcRecord(std::string const& in)
{
    //ctor
    csvline = in;
}

MarcRecord::~MarcRecord()
{
    //dtor
}

//print function
std::string const& MarcRecord::print() const
{
    return csvline;
}

bool const& MarcRecord::hasfieldmap() const
{
    return fieldmaploaded;
}


//build up the marc record from the initial string
void MarcRecord::buildup()
{
    //make sure record contains some text
    if (csvline.empty() || csvline.find_first_not_of(" \t\r\n", 1) == csvline.npos)     // first csv char might be a delimiter value.
    {
        throw MarcRecordException("CSV string is empty or non-literal");
    }

    // make sure there is a field mapping loaded
    if (fieldmap.empty())
    {
        throw MarcRecordException("No field mapping is loaded");
    }

    // parse csvline. Its fields are tab-separated, each field needs separate processing
    std::stringstream csvlinestream(csvline);
    std::string segment;
    while(std::getline(csvlinestream, segment, '_'))
    {
        csvfields.push_back(segment);
    }

    // for every field, create the corresponding Marc field

    for(std::vector<std::string>::iterator csvit = csvfields.begin(); csvit != csvfields.end(); ++csvit)
    {
        int csvcol = std::distance(csvfields.begin(), csvit);
        int marcnr = fieldmap[csvcol];

        // if field already exists but some subfield needs adding, then find it

        std::set<MarcField>::iterator fieldit = marcfields.find(marcnr);

        if (fieldit != marcfields.end())
        {
            // Get field out , update, put back in. Set elements cannot be modified directly (would risk to undo sorting)
            MarcField oldfield = *fieldit;
            oldfield.update(csvcol, (*csvit));
            marcfields.erase(fieldit);
            marcfields.insert(oldfield);
        }
        else
        {
            MarcField newfield(marcnr);
            newfield.update(csvcol, (*csvit));
            marcfields.insert(newfield);
        }

    }
}




//this loads a mapping from csv fields to MARC fields from an external file
void MarcRecord::loadfieldmap(std::string const& filename)
{
    fieldmap.clear();
    fieldmaploaded=false;

    try {

        std::ifstream infile(filename.c_str());
        if ( !infile.is_open() ) {
            throw MarcRecordException("Could not load field mapping file: " + filename);
        }
        std::string line;
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            int a, b;
            if (!(iss >> a >> b)) // comment line or empty line
            {
                continue;
            }
            else // process pair (a,b)
            {
                fieldmap.insert(std::pair<int, int>(a, b));
            }
        }

        fieldmaploaded=true;

    }
    catch (exception& e)
    {
        throw MarcRecordException("Could not load field mapping file: " + filename + ".\nAdditional exception message:\n" + e.what() );
    }
}





// friend operators outside the class definition:

std::ostream& operator<< (std::ostream& os, const MarcRecord& m)
{
    return os << m.print();
}

std::ostream& operator<< (std::ostream& os, const MarcRecord* m)
{
    return os << m->print();
}
