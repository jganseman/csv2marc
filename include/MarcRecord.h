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

typedef std::multiset<MarcField*, MarcFieldPtrComparator> t_fieldset;
typedef t_fieldset::const_iterator t_fieldsetIterator;

#define ORGCODE "BE-BxLRC"

// define a list of unique subfields. These can only contain one nr, any multiples need another field
static const int UniqueFields[] = {20, 22, 24, 382, 384, 500, 533, 650};
static const char UniqueSubfields[] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
static const int NrOfUniqueFields = 8;
// Added 24 and 382. Not strictly necessary, but adds to clarity.
//TODO add 260 here? not strictly unique, but for clarity it might be better to have it repeatable

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
        std::set<MarcField*> getMultipleFields(std::vector<int>& multiplenrs) const;
        void addField(MarcField* val){ marcfields.insert(val); }
        void deleteField(MarcField* val){ marcfields.erase(val); }
        bool isCRB() const;
        void setAuthority(bool value) {isAuthority=value;}
    protected:
    private:
        void AddFixedValues();
        void CheckTitle();
        void CheckNotForLoan();
        void MakeKeywordsUnique();
        void ProcessNonRepeatableFields();
        void ProcessParts();
        void AddKohaData();
        void MakeBarcode();

        friend std::ostream& operator<< (std::ostream& os, const MarcRecord& m);    // print object
        friend std::ostream& operator<< (std::ostream& os, const MarcRecord* m);    // print pointer to object

        std::string csvline;                     // a full line (1 record) of the csv file
        std::vector<std::string> csvfields;         // the same csv line parsed in a series of strings that correspond to the fields

        static std::multimap<int, t_marcfield > fieldmap;         // mapping of csv columns to marc fields (int-char pairs), shared for all records!
        static bool fieldmaploaded;                          // indicate whether a fieldmap is loaded

        t_fieldset marcfields;         // One records contains several fields
        //bool valid;
        bool isAuthority = false;
};



#endif // MARCRECORD_H
