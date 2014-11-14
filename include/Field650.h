#ifndef FIELD650_H
#define FIELD650_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <algorithm>
#include <vector>

class Field650 : public MarcField
{
    public:
        Field650(int nr);
        virtual ~Field650();
        virtual void update(char marcsubfield, std::string data);
        virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD650_H
