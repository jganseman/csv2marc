#include "MarcRecord.h"


// define the static members of this class
std::map<int, t_marcfield> MarcRecord::fieldmap;
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
    //delete the list of fields
    for(t_fieldsetIterator it = marcfields.begin(); it!=marcfields.end(); ++it)
    {
        delete *it;
    }
}

//print function
std::string const MarcRecord::print() const
{
    //return csvline;
    std::ostringstream output;

    // print all fields
    for(t_fieldsetIterator it = marcfields.begin(); it!=marcfields.end(); ++it)
    {
        if (!(*it)->isempty())
            output << (*it)->print();
    }

    // put a newline after each record
    output << endl;

    return output.str();
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
    while(std::getline(csvlinestream, segment, '\t'))
    {
        csvfields.push_back(segment);
    }

    // for every field, create the corresponding Marc field

    for(std::vector<std::string>::iterator csvit = csvfields.begin(); csvit != csvfields.end(); ++csvit)        // iterate over all fields
    {
        int csvcol = std::distance(csvfields.begin(), csvit);
        int marcnr = fieldmap[csvcol].first;
        char marcsubfield = fieldmap[csvcol].second;

        //if this field was not present in the mapping, therefore marcsubfield == null, skip field
        if (marcsubfield == '\0')
            continue;

        // if field already exists but some subfield needs adding, then find it
        // Now it constructs a dummy object for the search. TODO: with unique_ptr in C++11 this can be remedied
        MarcField* dummy = new MarcField(marcnr);
        t_fieldsetIterator fieldit = marcfields.find(dummy);
        delete dummy;

        if (fieldit != marcfields.end())        // a field with this number already exists in this record. update.
        {
            // Get field out , update, put back in. Set elements cannot be modified directly (map is kept sorted, would risk to undo sorting)
            MarcField* oldfield = *fieldit;              // get the field out (this makes a copy)
            oldfield->update(marcsubfield, (*csvit));          // update with new data
            marcfields.erase(fieldit);                  // erase in map
            marcfields.insert(oldfield);                // and re-add
        }
        else                     // a field with this number does not exist yet in this record. create and update.
        {
            MarcField* newfield = FieldFactory::getFactory()->getMarcField(marcnr);
            newfield->update(marcsubfield, (*csvit));
            marcfields.insert(newfield);
        }

    }

    // at the end, add some fixed fields
    MarcField* newfield = new MarcField(3);
    newfield->update('a', "BBc");       // does not matter in which subfield this is put, it's a control field
    marcfields.insert(newfield);

    newfield = new MarcField(38);    // rightful owner of this record
    newfield->update('a', "BBc");
    marcfields.insert(newfield);

    newfield = new MarcField(40);    // creator of this record. Also has "Language of record" field. Set by default on Dutch
    newfield->update('a', "BBc");
    newfield->update('b', "dut");
    newfield->update('c', "BBc");
    marcfields.insert(newfield);

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
            char c;
            if (!(iss >> a >> b >> c)) // comment line or empty line
            {
                continue;
            }
            else // process pair (a,b)
            {

                fieldmap.insert( t_fieldmapelement(a, t_marcfield(b,c)));
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
