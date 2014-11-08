#ifndef FIELDLDR_H
#define FIELDLDR_H

#include <MarcField.h>

#include<cstring>

class FieldLDR : public MarcField
{
    public:
        FieldLDR(int nr);
        virtual ~FieldLDR();
        void Setfixedstring(int index, char val) { if (index>=0 && index < 25) fixedstring[index] = val; }

        virtual std::string const print() const;

        virtual void update(char marcsubfield, std::string data);
        virtual bool isempty() const;

    protected:
    private:
        char fixedstring[25];       // add null character
};

#endif // FIELDLDR_H
