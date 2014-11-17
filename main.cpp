#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <MarcRecord.h>


using namespace std;

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


    long j = -1;
    while(std::getline(csvfile, line))
    {
        j++;

    /*
    for(int j=0; j<1000; j++)
    {
        std::getline(csvfile, line);
    */

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
            //cout << "!!! ERROR: while processing csv line nr " << j << endl;
            std::replace( line.begin(), line.end(), '\t', ' ');     // for better printing
            cout << line.substr(0,80) << " ..." << endl;
            cout << "Error message was: " << e.what() << endl << endl;
            delete thisrecord;
            continue;           // continue processing
        }

        //cout << thisrecord << endl;
        marcfile << thisrecord << endl;         // operator<< is overloaded for pointers to MarcRecord
        // cout << "Outputted " << thisrecord << " ; This was record " << ++count << endl ;

        delete thisrecord;
    }

    cout << "Finished processing " << j << " lines."<< endl;
    cout << "Last line: " << line;
    return 0;

}
