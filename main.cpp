#include <fstream>
#include <iostream>
#include <string>
#include <MarcRecord.h>


using namespace std;

int main ( int argc, char *argv[] )
{

if ( argc != 3 ) /* 2 arguments: filename to process and resulting filename  */
    {
        cout << "Usage: csv2marc csvfilename marcfilename" << endl;
        return -1;
    }

    // Open the input file
    ifstream csvfile(argv[1]);

    // check if success
    if ( !csvfile.is_open() ) {
        cout << "Could not open input csv file" << endl;
        return -1;
    }

    //open the output file
    ofstream marcfile(argv[2]);

    if ( !marcfile.is_open() ) {
        cout << "Could not open output csv file" << endl;
        return -1;
    }

    // all is still fine: process lines of csv one by one
    std::string line;
    MarcRecord* thisrecord;
    int count = 0;

    //while(std::getline(csvfile, line))
    //{

    for(int j=0; j<100; j++)
    {
        std::getline(csvfile, line);

        //cout << line << endl;

        thisrecord = new MarcRecord(line);      // initialize a new marc record

        try
        {
            thisrecord->buildup();      // build up
        }
        catch (exception& e)
        {
            cout << "!!! ERROR: Exception Thrown !!!" << endl;
            cout << "Printing csv data line:" << endl;
            cout << thisrecord << endl;
            cout << "Error message was:" << endl;
            cout << e.what() << endl;
            cout << "... Exiting program ...";
            delete thisrecord;
            return -1;
        }

        cout << thisrecord << endl;
        marcfile << thisrecord << endl;         // operator<< is overloaded for pointers to MarcRecord
        cout << "Outputted " << thisrecord << " ; This was record " << ++count << endl ;

        delete thisrecord;
    }

    cout << "Finished!" << endl;
    return 0;

}
