#ifndef FIELD490_H
#define FIELD490_H

#include <MarcField.h>


class Field490 : public MarcField
{
    public:
        Field490(int nr);
        virtual ~Field490();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD490_H
