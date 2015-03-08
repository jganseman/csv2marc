#ifndef FIELD022_H
#define FIELD022_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>

#include <string>
#include <algorithm>
#include <vector>


class Field022 : public MarcField
{
    public:
        Field022(int nr);
        virtual ~Field022();

        virtual void update(char marcsubfield, std::string data);
        virtual std::string const print() const;
    protected:
    private:
        std::string issn;
};

#endif // FIELD022_H
