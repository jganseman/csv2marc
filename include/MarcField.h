#ifndef MARCFIELD_H
#define MARCFIELD_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

typedef std::pair<char, std::string> t_subfield;

class MarcField
{
    public:
        MarcField(int nr, char defchar='\\');
        virtual ~MarcField();
        int Getfieldnr() { return fieldnr; }
        void Setfieldnr(int val) { fieldnr = val; }
        char Getindicator1() { return indicator1; }
        void Setindicator1(char val) { indicator1 = val; }
        char Getindicator2() { return indicator2; }
        void Setindicator2(char val) { indicator2 = val; }

        // operators that are overloaded such that elements can be looked up more easily
        inline bool operator< (const MarcField& rhs) const { return fieldnr < rhs.fieldnr; }
        inline bool operator== (const MarcField& rhs) const { return fieldnr == rhs.fieldnr; }
        inline bool operator== (int key) const { return fieldnr == key; }      // enable usage of find algorithm with integers

        virtual std::string const print() const;
        virtual void update(char marcsubfield, std::string data);
        virtual bool isempty() const;

    protected:
    private:
        int fieldnr;
        char indicator1;
        char indicator2;

        std::multimap<char, std::string> subfields;

};


// MarcFieldPtr Comparator Functor. Define this as parameter in template definition
struct MarcFieldPtrComparator
{
  bool operator()(MarcField* f, MarcField* s) const
  {
    return f->Getfieldnr() < s->Getfieldnr();
  }

    bool operator()(MarcField* f, int key) const
  {
    return f->Getfieldnr() < key;
  }

};

#endif // MARCFIELD_H
