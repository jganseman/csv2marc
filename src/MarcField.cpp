#include "MarcField.h"

MarcField::MarcField(int nr, char defchar)
{
    fieldnr=nr;
    indicator1=defchar;
    indicator2=defchar;
}

MarcField::~MarcField()
{
    //dtor
}


void MarcField::update(int csvcol, std::string data)
{

}
