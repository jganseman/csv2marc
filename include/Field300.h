#ifndef FIELD300_H
#define FIELD300_H

#include <MarcField.h>


class Field300 : public MarcField
{
    public:
        Field300(int nr);
        virtual ~Field300();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD300_H
