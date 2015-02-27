#include "MarcRecord.h"


// define the static members of this class
std::multimap<int, t_marcfield> MarcRecord::fieldmap;
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
    if (isvalid())
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
    else        // do not print records marked as invalid
    {
        return "";
    }
}


bool const& MarcRecord::hasfieldmap() const
{
    return fieldmaploaded;
}


//build up the marc record from the initial string
void MarcRecord::buildup()
{
    // parse csvline. Its fields are tab-separated, each field needs separate processing
    std::stringstream csvlinestream(csvline);
    std::string segment;
    while(std::getline(csvlinestream, segment, '\t'))
    {
        csvfields.push_back(segment);
    }

    // if the line is empty, do not even build a record
    try{
        //trim front and trailing whitespace
        std::string checkempty = csvline.erase(csvline.find_last_not_of(" \n\r\t")+1).substr(csvline.find_first_not_of(" \n\r\t"));
        if (checkempty.empty())
            return;
    } catch (exception e)
    {
        return;
        //throw MarcRecordException("ERROR Updating Field 700: empty value among authors.");
    }


    // for every CSV field, create the corresponding Marc field

    for(std::vector<std::string>::iterator csvit = csvfields.begin(); csvit != csvfields.end(); ++csvit)        // iterate over all fields
    {
        int csvcol = std::distance(csvfields.begin(), csvit);

        for(std::multimap<int, t_marcfield >::iterator mapit = fieldmap.lower_bound(csvcol); mapit != fieldmap.upper_bound(csvcol); ++mapit)        // iterate over all fields
        {
            int marcnr = (*mapit).second.first;
            char marcsubfield = (*mapit).second.second;

            //if this field was not present in the mapping, therefore marcsubfield == null, skip field
            if (marcsubfield == '\0')
                continue;

            // if field already exists but some subfield needs adding, then find it
            // Now it constructs a dummy object for the search. TODO: with unique_ptr in C++11 this can be remedied
            MarcField* dummy = new MarcField(marcnr);
            t_fieldsetIterator fieldit = marcfields.find(dummy);
            delete dummy;

                    // special case of notes: many field maps to general note 500. needs duplication
            if ((fieldit != marcfields.end()) && ((*fieldit)->Getfieldnr() != 500))       // a field with this number already exists in this record. update.
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
    }

    // skip records with status X (crossref) and status C (copies)
    MarcField* dummy = new MarcField(583);
    t_fieldsetIterator fieldit = marcfields.find(dummy);
    delete dummy;
    if (fieldit != marcfields.end())
    {
        if ((*fieldit)->Getsubfield('a') == "C" || (*fieldit)->Getsubfield('a') == "X")
            return;
    }


    // At the end of regular records, add some fixed fields
    MarcField* newfield = FieldFactory::getFactory()->getMarcField(3);
    newfield->update('a', ORGCODE);       // does not matter in which subfield this is put, it's a control field
    marcfields.insert(newfield);

    newfield = FieldFactory::getFactory()->getMarcField(38);    // rightful owner of this record
    newfield->update('a', ORGCODE);
    marcfields.insert(newfield);

    newfield = FieldFactory::getFactory()->getMarcField(40);    // creator of this record. Also has "Language of record" field. Set by default on Dutch
    newfield->update('a', ORGCODE);
    newfield->update('b', "dut");
    //newfield->update('c', ORGCODE);
    marcfields.insert(newfield);

    //According to the MARC21 standard, a title field must be present. If not, add [untitled].
    dummy = new MarcField(245);
    fieldit = marcfields.find(dummy);
    delete dummy;
    if (fieldit == marcfields.end())
    {
        newfield = FieldFactory::getFactory()->getMarcField(245);
        newfield->update('a', "[untitled]");
        marcfields.insert(newfield);
        throw MarcRecordException("WARNING Field 245: No title present. Putting [untitled].");
    } else if ((*fieldit)->Getsubfield('a').empty())
    {
        (*fieldit)->update('a', "[untitled]");
        throw MarcRecordException("WARNING Field 245: No title present. Putting [untitled].");
    }

    /*else {
        if ((*fieldit)->Getfieldnr() != 245)
            cout << "Found field 245 at field " << (*fieldit)->Getfieldnr() << endl;
    }*/
    // this was debug info

    // finally, process fields that have been marked as unique
    for (int i=0; i<NrOfUniqueFields; ++i)
    {
        int curfield = UniqueFields[2];
        char cursubfield = UniqueSubfields[2];
        MarcField* badfield = getField(curfield);
        vector< pair < char, string > > extractedFields = badfield->extractDoubleSubfields(cursubfield);

        for(unsigned int j = 0; j<extractedFields.size(); ++j )
        {
            MarcField* newfield = FieldFactory::getFactory()->getMarcField(curfield);
            newfield->update(cursubfield, extractedFields[j].second );
            marcfields.insert(newfield);
        }
    }


    //then, copy the new record number from field 001 into field 952 (KOHA specific)
    std::string recordnr = getField(1)->Getsubfield('a');
    getField(952)->update('o', recordnr);        // shelf number
    getField(952)->update('p', recordnr);        // barcode


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



bool MarcRecord::isvalid() const
{
    bool hastitle = false;
    bool hasplacenr = false;

    //if record has status field c or x , it is a cross reference of other records
    //and therefore should not be retained in the print-out
    for(t_fieldsetIterator it = marcfields.begin(); it!=marcfields.end(); ++it)
    {
        if ((*it)->Getfieldnr() == 583)       // overloaded operator
        {
            if ((*it)->Getsubfield('a') == "C" || (*it)->Getsubfield('a') == "X" || (*it)->Getsubfield('a') == "c" || (*it)->Getsubfield('a') == "x")
                return false;
        }

        // if the record does not have a title in field 245, it is invalid
        if ((*it)->Getfieldnr() == 245)
        {
            hastitle=true;
        }

        // if the record has no place nr, it is invalid
        if ((*it)->Getfieldnr() == 1)
        {
            if ((*it)->Getsubfield('a') != "")
                hasplacenr=true;
        }
    }

    return (hastitle && hasplacenr);
}


MarcField* MarcRecord::getField(int nr) const
{
    MarcField* dummy = new MarcField(nr);
    t_fieldsetIterator fieldit = marcfields.find(dummy);
    delete dummy;
    if (fieldit == marcfields.end())
    {
        return 0;
    }
    else
    {
        return *fieldit;
    }
}


bool MarcRecord::isCRB() const
{
    for(t_fieldsetIterator it = marcfields.begin(); it!=marcfields.end(); ++it)
    {
        if ((*it)->Getfieldnr() == 590)       // this is where the author is stored
        {
            std::string author = (*it)->Getsubfield('a');
            if  ( (author == "mv") || (author == "im") || (author == "mcl") || (author == "sdp") || (author == "lm")
                || (author == "ah") || (author == "ar") || (author == "bkb") || (author == "cc") || (author == "cf")
                || (author == "df") || (author == "dl") || (author == "ed") || (author == "fb") || (author == "hl")
                || (author == "id") || (author == "ls") || (author == "mdd") || (author == "mt")
                || (author == "rs") || (author == "th") || (author == "vdf") )
            {
                return true;
            } else
            {
                return false;
            }
        }
    }
    return false;
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
