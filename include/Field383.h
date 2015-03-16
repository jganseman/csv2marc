#ifndef FIELD383_H
#define FIELD383_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>

#include <cstring>

class Field383 : public MarcField
{
    public:
        Field383(int nr);
        virtual ~Field383();
        virtual void update(char marcsubfield, std::string data);
        //virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD383_H
