#include "MarcRecord.h"

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
std::string const& MarcRecord::to_str() const
{
    return csvline;
}


//build up the marc record from the initial string
void MarcRecord::buildup()
{
    throw MarcRecordException("Buildup not implemented yet");
}




// friend operators outside the class definition:

std::ostream& operator<< (std::ostream& os, const MarcRecord& m)
{
    return os << m.to_str();
}

std::ostream& operator<< (std::ostream& os, const MarcRecord* m)
{
    return os << m->to_str();
}
