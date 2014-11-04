#ifndef MARCRECORD_H
#define MARCRECORD_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <MarcRecordException.h>
#include <MarcField.h>

using namespace std;

//typedef std::pair<int, int> mapelement;

class MarcRecord
{
    public:
        MarcRecord();
        MarcRecord(std::string const& in);
        virtual ~MarcRecord();
        std::string const& print() const;
        void buildup();
        void loadfieldmap(std::string const& filename);
        bool const& hasfieldmap() const;
    protected:
    private:
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord& m);    // print object
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord* m);    // print pointer to object
        std::string csvline;
        std::vector<std::string> csvfields;

        static std::map<int, int> fieldmap;         // share this for all records!
        static bool fieldmaploaded;

        std::set<MarcField> marcfields;         // One records contains several fields
};



#endif // MARCRECORD_H
