#ifndef FIELD336_H
#define FIELD336_H

#include <MarcField.h>
#include <set>


class Field336 : public MarcField
{
    public:
        Field336(int nr);
        virtual ~Field336();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
        char extracttype(std::string data);

        // the following data defines the sets of terms admitted in the B-Bc library
        bool isValidTerm(std::string& data);
        static std::set<std::string> validterms;
        static std::set<std::string> init();
        void fixTerminology(std::string& data);
};

#endif // FIELD336_H
