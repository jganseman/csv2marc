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
            if (!(*it)->isempty())      // there might have creeped empty fields in, created because of the fieldmap
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
    //cout << csvline << endl;

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
        Helper::Trim(csvline);
        if (csvline.empty())
            return;
    } catch (exception e)
    {
        return;         // error encountered in basic data in this line / empty
    }

    std::string errorlist = "";     // accumulate all errors about this record here

    // for every CSV field, create the corresponding Marc field
    for(std::vector<std::string>::iterator csvit = csvfields.begin(); csvit != csvfields.end(); ++csvit)        // iterate over all CSV fields
    {
        int csvcol = std::distance(csvfields.begin(), csvit);       // calculate the column number

        // if this column does not contain data, move on
        if ((*csvit).empty())
            continue;

        for(std::multimap<int, t_marcfield >::iterator mapit = fieldmap.lower_bound(csvcol); mapit != fieldmap.upper_bound(csvcol); ++mapit)        // iterate over all associated MARC fields
        {
            int marcnr = (*mapit).second.first;
            char marcsubfield = (*mapit).second.second;

            //if this field was not present in the mapping, therefore marcsubfield == null, skip field
            if (marcsubfield == '\0')
                continue;

            // if field already exists but some subfield needs adding, then find it
            // Now it constructs a dummy object for the search. TODO: with unique_ptr in C++11 this can be remedied
            MarcField* thisfield = getField(marcnr);

            try {

                if (thisfield != 0)       // a field with this number already exists in this record. update.
                {
                    thisfield->update(marcsubfield, (*csvit));          // update with new data
                }
                else                     // a field with this number does not exist yet in this record. create and update.
                {
                    MarcField* newfield = FieldFactory::getFactory()->getMarcField(marcnr);
                    // first insert the field, so at least we have the data
                    // if any exceptions contain the word ERROR, it will later be deleted anyway
                    marcfields.insert(newfield);
                    newfield->update(marcsubfield, (*csvit));       // then build the subfield
                }

            } catch(exception& e) {
                if (!errorlist.empty())         // add newline when necessary
                    errorlist += "\r\n";
                errorlist += e.what();          // add any error to the errorlist of this record
            }
        }
    }

    // check validity of record
    // ( skip records with status X (crossref) and status C (copies) )
    if (!isvalid())
        return;

    // Now, do all the postprocessing that's required to finetune this record
    try {
        AddFixedValues();
    } catch(exception& e) {
        errorlist += e.what();          // add any error to the errorlist of this record
    }

    try {
        CheckTitle();
    } catch(exception& e) {
        errorlist += e.what();          // add any error to the errorlist of this record
    }

    try {
        ProcessNonRepeatableFields();
    } catch(exception& e) {
        errorlist += e.what();          // add any error to the errorlist of this record
    }

    try {
        ProcessParts();
    } catch(exception& e) {
        errorlist += e.what();          // add any error to the errorlist of this record
    }

    try {
        AddKohaData();
    } catch(exception& e) {
        errorlist += e.what();          // add any error to the errorlist of this record
    }
    // throw out the error list
    if (errorlist != "")
    {
        throw MarcRecordException(errorlist);
    }
    //cout << "End buildup" << endl;

}

void MarcRecord::AddFixedValues()
{
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
}


void MarcRecord::CheckTitle()
{
    //According to the MARC21 standard, a title field must be present. If not, add [untitled].
    MarcField* myfield = getField(245);

    if (myfield == 0)
    {
        MarcField* newfield = FieldFactory::getFactory()->getMarcField(245);
        newfield->update('a', "[untitled]");
        marcfields.insert(newfield);
        throw MarcRecordException("WARNING Field 245: No title present. Putting [untitled].");
    } else if (myfield->Getsubfield('a').empty())
    {
        myfield->update('a', "[untitled]");
        throw MarcRecordException("WARNING Field 245: No title present. Putting [untitled].");
    }
}


void MarcRecord::ProcessNonRepeatableFields()
{
    // finally, process fields that have been marked as unique -> decouple them into separate fields
    // The list of fields to which this applies has been defined in MarcRecord.h
    for (int i=0; i<NrOfUniqueFields; ++i)
    {
        int curfield = UniqueFields[i];
        char cursubfield = UniqueSubfields[i];
        MarcField* badfield = getField(curfield);
        if (badfield == 0)       // Do not process non-existing fields
            continue;

        vector< pair < char, string > > extractedFields = badfield->extractDoubleSubfields(cursubfield);
        for(unsigned int j = 0; j<extractedFields.size(); ++j )
        {
            if (extractedFields[j].second == "")        // if no data: return
                continue;
            MarcField* newfield = FieldFactory::getFactory()->getMarcField(curfield);
            newfield->Setindicator1(badfield->Getindicator1());
            newfield->Setindicator2(badfield->Getindicator2());
                // NOTE: Parsing and reformatting already happened when data was added to the original field.
                // Now just force-add, bypassing the reformatting routine in the derived classes: use base class
                // This requires us to override the virtualization. Explicitly use the function from MarcField class:
            newfield->MarcField::update(cursubfield, extractedFields[j].second );
            marcfields.insert(newfield); // first insert, then update?
            //if (curfield != 650) cout << "inserted new field " << newfield->Getfieldnr() << cursubfield << " with data " << newfield->Getsubfield(cursubfield) << endl;
        }
    }
}


void MarcRecord::ProcessParts()
{
    // when the place number contains ONDERDEEL,
        // - change bibliography level (leader) to "a" (part of monograph)
        // - add a field 773$w linking to the mother record.
    MarcField* f001 = getField(001);
    if (f001 && !(f001->isempty()))
    {
        std::string recordnr = f001->Getsubfield('a');
        Helper::MakeUppercase(recordnr);
        std::size_t found = recordnr.find("ONDERDEEL");
        if (found != recordnr.npos)
        {
            // set bibliographic level to: monograph part
            (dynamic_cast<FieldLDR*>(getField(0)))->Setfixedstring(7, 'a');
            // for "onderdeel" (physical part), we use http://wiki.koha-community.org/wiki/Serial_Analytics
            // ... seems only partially developed in Koha for the moment.

            // prepare string for 773
            // note: hostcode not necessary for Koha
            /*
            std::string hostcode = "(";
            hostcode += ORGCODE;
            hostcode += ")";
            */
            std::string location = f001->Getsubfield('a');
            location = location.substr(0, found);

            // add field 773: Host entry
            MarcField* newfield = FieldFactory::getFactory()->getMarcField(773);
            newfield->Setindicator1('0');
            newfield->Setindicator2(DEFAULT_INDIC);
            newfield->update('w', /*hostcode+*/location );
            marcfields.insert(newfield);
        }
        else        // TODO process DEEL according to use http://wiki.koha-community.org/wiki/Sets_with_Volumes
        {           // Note that LEADER[19] plays a role here, as does field 830
                    // note that in B-Bc; "deel" is recognized by having brackets "()" in the place nr (inventory nr is pretty inconsistent)
            return;
        }
    }
    else
    {
        throw MarcRecordException("ERROR Field 001: no placenr present in field : " + this->print());
    }
}


void MarcRecord::AddKohaData()
{
    // all Koha-specific data goes into field 952. This mostly is data important for the holdings
    // see http://wiki.koha-community.org/wiki/Holdings_data_fields_%289xx%29#MARC21_Holding_field.2C_3.0.x.2C_3.2.x_and_3.4.x_default_-_field_952
    MarcField* f952 = FieldFactory::getFactory()->getMarcField(952);

    f952->update('a', "B-Bc");    // home branch, use library code defined in KOHA (not MARC code)
    f952->update('b', "B-Bc");    // holdings branch
    try {
        std::string recordnr = getField(1)->Getsubfield('a');
        f952->update('o', recordnr);   // shelf number
        f952->update('p', recordnr);    // barcode
    } catch (exception& e)
    {
        throw MarcRecordException("ERROR field 952: no recordnr?");
    }

    // process LOST status, stored in field 583 (Action Note = status)
    MarcField* f583 = getField(583);
    if (f583)
    {
        std::string status = f583->Getsubfield('a');
        Helper::MakeUppercase(status);
        if (status.find("LOST") != std::string::npos)
            f952->update('1', "1");         // stored in subfield $1, LOST code = "1"
    }

    // process NOT FOR LOAN status, stored in field 506 (Availability Note)
    MarcField* f506 = getField(506);
    if (f506)
    {
        std::string nfl = f506->Getsubfield('a');
        if (nfl.find("*") != std::string::npos)
            f952->update('7', "1");         // stored in subfield $7, NOT FOR LOAN code = "1"
    }

    // process KOHA ITEM TYPE, stored in LEADER
    MarcField* fLDR = getField(000);
    char itemtype = fLDR->print()[6+6];  // add 4 characters for the "=LDR__" line
    std::string kohacode;
    switch (itemtype)
    {
        case 'r': kohacode = "RE"; break;     // realia: kits, 3D objects
        case 'm': kohacode = "CF"; break;      // computer-readable file, cdrom, microfilm
        case 'k': kohacode = "IM"; break;      // 2d imagery and projected graphics
        case 'g': kohacode = "VI"; break;      // video, film, dvd
        case 'j': kohacode = "CD"; break;      // music recordings: cd, vinyl, tape -> split up later!
        case 'a': kohacode = "TP"; break;      // books
        case 't': kohacode = "TM"; break;      // book manuscript
        case 'c': kohacode = "MP"; break;      // scores
        case 'd': kohacode = "MM"; break;      // score manuscripts
        default: kohacode = "MP";
    }
    // additional item types, recognized by callnumber
    MarcField* f001 = getField(001);
    if (f001 && !(f001->isempty()))
    {
        std::string recordnr = f001->Getsubfield('a');
        Helper::MakeUppercase(recordnr);
        if (recordnr.substr(0, 4) == "ARC-") {              // archive
            kohacode = "AR";
        } else if (recordnr.substr(0, 3) == "SS-") {        // periodicals
            kohacode = "PE";
        } else if (recordnr.substr(0, 4) == "E-TS") {        // more periodicals
            kohacode = "PE";
        } else if (recordnr.substr(0, 4) == "E-CD") {        // cds
            kohacode = "CD";
        } else if (recordnr.substr(0, 4) == "R-CD") {        // more cds
            kohacode = "CD";
        } else if (recordnr.substr(0, 4) == "E-LP") {        // LP, vinyl
            kohacode = "LP";
        } else if (recordnr.substr(0, 4) == "E-VD") {        // video
            kohacode = "VI";
        }
    }
    // additional itemtypes, recognized by content type field
    if (itemtype == 'j')
    {
        MarcField* f336 = getField(336);
        if (f336 && !(f336->isempty()))
        {
            std::string mattype = f001->Getsubfield('a');
            Helper::MakeLowercase(mattype);
            if (mattype.find("geluidsband") != mattype.npos) {
                kohacode = "TA";
            } else if (mattype.find("cassette") != mattype.npos) {
                kohacode = "TA";
            }
        }
    }

    f952->update('y', kohacode);

    marcfields.insert(f952);

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
    for(t_fieldsetIterator it = marcfields.begin(); it!=marcfields.end(); ++it)
    {
        if ((*it)->Getfieldnr() == nr)
            return (*it);
    }
    return 0;

    /*
    MarcField* dummy = new MarcField(nr);
    t_fieldsetIterator fieldit = marcfields.find(dummy);
    delete dummy;
    if (fieldit == marcfields::end)
    {
        return 0;
    }
    else
    {
        return *fieldit;
    }
    */
}


bool MarcRecord::isCRB() const
{
    MarcField* f588 = getField(588);
    if (f588)
    {
        std::string author = f588->Getsubfield('a');
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
