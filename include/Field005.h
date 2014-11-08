#ifndef FIELD005_H
#define FIELD005_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <cstdlib>
#include <cstring>

class Field005 : public MarcField
{
    public:
        Field005(int nr);
        virtual ~Field005();

        virtual std::string const print() const;
        virtual void update(char marcsubfield, std::string data);
        virtual bool isempty() const;
    protected:
    private:
        char fixedstring[17];
};

#endif // FIELD005_H
