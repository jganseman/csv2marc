#ifndef FIELD028_H
#define FIELD028_H

#include <MarcField.h>


class Field028 : public MarcField
{
    public:
        Field028(int nr);
        virtual ~Field028();

        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD028_H
