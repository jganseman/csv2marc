#include "MarcRecordException.h"

MarcRecordException::MarcRecordException()
{
    //ctor
}

MarcRecordException::MarcRecordException(std::string const& in)
{
    //ctor
    message = in;
}


MarcRecordException::MarcRecordException(const MarcRecordException& orig)
{
    message = orig.message;
}

MarcRecordException::~MarcRecordException() throw()
{
    //dtor
}


const char* MarcRecordException::what() const throw()
{
    return message.c_str();
}
