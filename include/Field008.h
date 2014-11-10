#ifndef FIELD008_H
#define FIELD008_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include<cstring>
#include<cstdlib>

#include <string>
#include <algorithm>
#include <vector>

class Field008 : public MarcField
{
    public:
        Field008(int nr);
        virtual ~Field008();
        void Setfixedstring(int index, char val) { if (index>=0 && index < 41) fixedstring[index] = val; }

        virtual std::string const print() const;
        virtual void update(char marcsubfield, std::string data);
        virtual bool isempty() const;
    protected:
    private:
        char fixedstring[41];       // add null character
};

#endif // FIELD008_H
