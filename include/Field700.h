#ifndef FIELD700_H
#define FIELD700_H

#include <MarcField.h>

#include <string>
#include <algorithm>
#include <vector>

class Field700 : public MarcField
{
    public:
        Field700(int nr);
        virtual ~Field700();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD700_H
