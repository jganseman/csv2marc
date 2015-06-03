#include "Field005.h"

Field005::Field005(int nr) : MarcField(nr)
{
    //ctor
    strcpy(&fixedstring[0], "00000000000000.0");
}

Field005::~Field005()
{
    //dtor
}



std::string const Field005::print() const
{
    // overloaded print function for fixed length field
    if (strcmp(fixedstring,"00000000000000.0"))         // if strings are NOT EQUAL, then strcmp = TRUE !!!
    {
        std::ostringstream output;
        output << "=005  " << fixedstring << endl;
        return output.str();
    }
    return "";
}



void Field005::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // assert string is a date
    if (!atol(data.c_str()))
        if (verbose) throw MarcRecordException("ERROR Field 005: string is not a valid date : " + data);

    // this is a field for the latest transaction. If given multiple dates, only use the last one
    std::string currentdata(fixedstring);

    //cout << "Data: fixedstring currentdata newdata " << fixedstring << '\t' << currentdata << '\t' << data << endl;

    if (currentdata.substr(0,8) < data.substr(0,8))         // we want the latest date in this field
    {
        for (int i=0; i<8; ++i) {fixedstring[i] = data[i];}
    }
}


bool Field005::isempty() const
{
    return !strcmp(fixedstring,"00000000000000.0");     // if strings are equal, strcmp = false !!!
}
