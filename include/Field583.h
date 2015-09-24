#ifndef FIELD583_H
#define FIELD583_H

#include <MarcField.h>


class Field583 : public MarcField
{
    public:
        Field583(int nr);
        virtual ~Field583();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD583_H
