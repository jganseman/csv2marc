#ifndef MARCRECORDEXCEPTION_H
#define MARCRECORDEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class MarcRecordException : public exception
{
    public:
        MarcRecordException();
        MarcRecordException(std::string const& in);
        MarcRecordException(const MarcRecordException& orig);
        virtual ~MarcRecordException() throw();
        virtual const char* what() const throw();
    protected:
    private:
        std::string message;
};

#endif // MARCRECORDEXCEPTION_H
