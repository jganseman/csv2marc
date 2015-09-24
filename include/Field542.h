#ifndef FIELD542_H
#define FIELD542_H

#include <MarcField.h>


class Field542 : public MarcField
{
    public:
        Field542(int nr);
        virtual ~Field542();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD542_H
