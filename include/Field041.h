#ifndef FIELD041_H
#define FIELD041_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <algorithm>
#include <vector>

class Field041 : public MarcField
{
    public:
        Field041(int nr);
        virtual ~Field041();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD041_H
