#ifndef FIELD952_H
#define FIELD952_H

#include <MarcField.h>

#include <algorithm>

// this field encodes internal circulation data. TODO: check whether necessary. Could be defined in MARC to KOHA mapping?
class Field952 : public MarcField
{
    public:
        Field952(int nr);
        virtual ~Field952();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD952_H
