#ifndef FIELD382_H
#define FIELD382_H

#include <MarcField.h>

#include <algorithm>
#include <cstring>

class Field382 : public MarcField
{
    public:
        Field382(int nr);
        virtual ~Field382();
        virtual void update(char marcsubfield, std::string data);
        //virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD382_H
