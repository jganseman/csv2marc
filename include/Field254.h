#ifndef FIELD254_H
#define FIELD254_H

#include <MarcField.h>


class Field254 : public MarcField
{
    public:
        Field254(int nr);
        virtual ~Field254();
        //virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD254_H
