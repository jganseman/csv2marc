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
#include <FieldFactory.h>
#include <MarcField.h>

using namespace std;

typedef std::pair<int, char> t_marcfield;
typedef std::pair<int, t_marcfield> t_fieldmapelement;

typedef std::set<MarcField*, MarcFieldPtrComparator> t_fieldset;
typedef t_fieldset::const_iterator t_fieldsetIterator;

#define ORGCODE "BE-BxLRC"

// define a list of unique fields
static const int UniqueFields[] = {20, 22, 384};
static const char UniqueSubfields[] = {'a', 'a', 'a'};
static const int NrOfUniqueFields = 3;


class MarcRecord
{
    public:
        MarcRecord();
        MarcRecord(std::string const& in);
        virtual ~MarcRecord();
        std::string const print() const;
        void buildup();
        void loadfieldmap(std::string const& filename);
        bool const& hasfieldmap() const;
        bool isvalid() const;
        //void setValidity(bool value) { valid=value; }
        MarcField* getField(int nr) const;
        void addField(MarcField* val){ marcfields.insert(val); }
    protected:
    private:
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord& m);    // print object
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord* m);    // print pointer to object

        std::string csvline;                     // a full line (1 record) of the csv file
        std::vector<std::string> csvfields;         // the same csv line parsed in a series of strings that correspond to the fields

        static std::multimap<int, t_marcfield > fieldmap;         // mapping of csv columns to marc fields (int-char pairs), shared for all records!
        static bool fieldmaploaded;                          // indicate whether a fieldmap is loaded

        t_fieldset marcfields;         // One records contains several fields
        //bool valid;
};



#endif // MARCRECORD_H
