#ifndef FIELD591_H
#define FIELD591_H

#include <MarcField.h>


class Field591 : public MarcField
{
    public:
        Field591(int nr);;
        virtual ~Field591();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD591_H
