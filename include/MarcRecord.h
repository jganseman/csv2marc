#ifndef MARCRECORD_H
#define MARCRECORD_H

#include <iostream>
#include <string>
#include <MarcRecordException.h>

using namespace std;

class MarcRecord
{
    public:
        MarcRecord();
        MarcRecord(std::string const& in);
        virtual ~MarcRecord();
        std::string const& to_str() const;
        void buildup();
    protected:
    private:
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord& m);    // print object
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord* m);    // print pointer to object
        std::string csvline;
};


#endif // MARCRECORD_H
