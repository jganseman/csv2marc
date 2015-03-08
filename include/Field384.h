#ifndef FIELD384_H
#define FIELD384_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>

#include <algorithm>



class Field384 : public MarcField
{
    public:
        Field384(int nr);
        virtual ~Field384();
        virtual void update(char marcsubfield, std::string data);
        //virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD384_H
