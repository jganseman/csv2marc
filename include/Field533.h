#ifndef FIELD533_H
#define FIELD533_H

#include <MarcField.h>


class Field533 : public MarcField
{
    public:
        Field533(int nr);
        virtual ~Field533();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD533_H
