#ifndef MARCFIELD_H
#define MARCFIELD_H

#include <string>

class MarcField
{
    public:
        MarcField(int nr, char defchar=' ');
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

        void update(int csvcol, std::string data);

    protected:
    private:
        int fieldnr;
        char indicator1;
        char indicator2;
};

#endif // MARCFIELD_H
