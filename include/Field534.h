#ifndef FIELD534_H
#define FIELD534_H

#include <MarcField.h>


class Field534 : public MarcField
{
    public:
        Field534(int nr);
        virtual ~Field534();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD534_H
