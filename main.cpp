#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <MarcRecord.h>
#include <Field001.h>

using namespace std;

// forward declaration of helper functions
void ProcessConstituents(std::set<std::string>& callnumbers, std::multimap<std::string, MarcRecord*>& allRecords, std::ostringstream& KCBerrs);

// definition of a few constants
#define PRINTKCB 1
#define PRINTCRB 1


int main ( int argc, char *argv[] )
{

if ( argc != 3 ) /* 2 arguments: filename to process and resulting filename  */
    {
        cout << "Usage: csv2marc csvfilename marcfilename" << endl;
        return -1;
    }

    // Open the input file and check if successful
    ifstream csvfile(argv[1]);
    if ( !csvfile.is_open() ) {
        cout << "Could not open input csv file" << endl;
        return -1;
    }

    //open the output file and check if successful
    ofstream marcfile(argv[2]);
    if ( !marcfile.is_open() ) {
        cout << "Could not open output csv file" << endl;
        return -1;
    }

    // all is still fine: process lines of csv one by one
    std::string line;
    MarcRecord* thisrecord;

    std::ostringstream KCBerrs;
    std::ostringstream CRBerrs;

    // for postprocessing: create a set of the call numbers used
    std::set<std::string> callnumbers;

    // Create large datastructure to hold all records
    std::multimap<std::string, MarcRecord*> allRecords;

    long j = -1;
    while(std::getline(csvfile, line))
    {
        j++;

        if (j%1000 == 0)
            cout << "Processed "  << j << " records" << endl;

        if (line.empty() || line == "")
            continue;

        //cout << line << endl;

        thisrecord = new MarcRecord(line);      // initialize a new marc record

        try
        {
            if (!thisrecord->hasfieldmap())
            {
                thisrecord->loadfieldmap("fieldmap.txt");
            }
            thisrecord->buildup();      // build up
        }
        catch (exception& e)
        {
            std::string what(e.what());

            //coarse escaping routine: do not print errors belonging to certain field
            /*
            if ( (what.find("382") != what.npos) || (what.find("383") != what.npos) || (what.find("384") != what.npos) ||
                (what.find("020") != what.npos) || (what.find("700") != what.npos) || (what.find("245") != what.npos) )
                continue;
                */

            //cout << "!!! ERROR: while processing csv line nr " << j << endl;
            if (thisrecord->getField(001) && !(thisrecord->getField(001)->isempty()))
            {
                // if it already has a placenumber, print by placenr
                if(thisrecord->isCRB())
                {
                    CRBerrs << "Error converting record pl=" << thisrecord->getField(001)->print().substr(5) ; // endl included in print()
                    CRBerrs << "   " << what << endl << endl;
                } else
                {
                    KCBerrs << "Error converting record pl=" << thisrecord->getField(001)->print().substr(5) ; // endl included in print()
                    KCBerrs << "   " << what << endl << endl;
                }

            }
            else    // unformatted error
            {
                std::replace( line.begin(), line.end(), '\t', ' ');     // for better printing
                KCBerrs << "ERR in " << line.substr(0,80) << " ..." << endl;
                KCBerrs << "       " << what << endl << endl;
            }

            // Only when the output is an actual error, and not a warning, delete and continue
            if (what.find("ERROR") != what.npos)
            {
                delete thisrecord;
                continue;           // continue processing
            }
        }

        // cout << "Outputted " << thisrecord << " ; This was record " << ++count << endl ;

        // add callnumber to set of callnumbers - if it exists!
        if (thisrecord->getField(001) && !(thisrecord->getField(001)->isempty()))
        {
            callnumbers.insert(thisrecord->getField(001)->Getsubfield('a'));
            allRecords.insert(std::make_pair(thisrecord->getField(001)->Getsubfield('a'), thisrecord));
        }

        //delete thisrecord;
    }

    // process the callnumber list. This checks whether constituent parts actually have mother records
    // and if necessary, adds the necessary fields 773 (in parts) and 774 (in mothers)
    ProcessConstituents(callnumbers, allRecords, KCBerrs);


    cout << " === CRB ERROR LIST === " << endl;
    cout << CRBerrs.str() << endl;

    cout << " === KCB ERROR LIST === " << endl;
    cout << KCBerrs.str() << endl;


    cout << "Finished processing " << j << " lines."<< endl;
    //cout << "Last line: " << line;

    // print all double callnumbers, by number. From static field
    Field001::printIDcounts();

    cout << "Now writing to MARC mnemonic file." << endl;
    // now write all records to file and delete datastructure
    long counter = 0;
    for (std::multimap<std::string, MarcRecord*>::iterator it = allRecords.begin(); it != allRecords.end(); ++it)
    {
        if (PRINTKCB && !((*it).second)->isCRB() )
        {
            marcfile << (*it).second << endl;
        }
        else if (PRINTCRB && ((*it).second)->isCRB() )
        {
            marcfile << (*it).second << endl;         // operator<< is overloaded for pointers to MarcRecord
        }
        delete (*it).second;
        counter++;
    }
    cout << "All Done. MARC file contains " << counter << " records." << endl;

    return 0;

}


// Note: the link from child to mother is done in MarcRecord::ProcessParts
void ProcessConstituents(std::set<std::string>& callnumbers, std::multimap<std::string, MarcRecord*>& allRecords, std::ostringstream& KCBerrs)
{
    for (std::set<std::string>::iterator it = callnumbers.begin(); it != callnumbers.end(); ++it)
    {
        // if callnumber contains onderdeel
        std::string curstring = (*it);
        Helper::MakeUppercase(curstring);
        std::size_t found = curstring.find("ONDERDEEL");
        if (found != curstring.npos)
        {
            // create string with the pre-onderdeel string
            std::string mothercallnr = (*it).substr(0, found);
            // trim whitespace
            Helper::Trim(mothercallnr);
            // find in set of callnumbers
            std::set<std::string>::iterator present = callnumbers.find(mothercallnr);
            if (present == callnumbers.end())       // Mother record NOT FOUND! throw warning
            {
                KCBerrs << "Warning: no mother record found for " << (*it) << endl;
            }
            else    // mother record found. Add field 774 to mother record
            {
                MarcRecord* mother = (*(allRecords.find(mothercallnr))).second;
                    // prepare string for 773
                    // note: organization code specifier not necessary in Koha
                    /*
                std::string hostcode = "(";
                hostcode += ORGCODE;
                hostcode += ")";
                    */

                // add field 774: Host entry
                MarcField* newfield = FieldFactory::getFactory()->getMarcField(774);
                newfield->Setindicator1('0');
                newfield->Setindicator2(DEFAULT_INDIC);
                newfield->update('w', /*hostcode+*/(*it) );
                mother->addField(newfield);
            }
        }
    }
    return;
}

