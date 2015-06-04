#ifndef FIELD382_H
#define FIELD382_H

#include <MarcField.h>
#include <Helper.h>

#include <algorithm>
#include <cstring>
#include <set>

class Field382 : public MarcField
{
    public:
        Field382(int nr);
        virtual ~Field382();
        virtual void update(char marcsubfield, std::string data);
        //virtual std::string const print() const;
    protected:
    private:
        // the following data defines the sets of instruments admitted in the B-Bc library
        bool isValidTerm(std::string& data);
        static std::set<std::string> validterms;
        static std::set<std::string> init();
        void fixTerminology(std::string& data);
};

#endif // FIELD382_H
