#ifndef FIELD020_H
#define FIELD020_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <algorithm>
#include <vector>


class Field020 : public MarcField
{
    public:
        Field020(int nr);
        virtual ~Field020();

        virtual void update(char marcsubfield, std::string data);
        virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD020_H
