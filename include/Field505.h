#ifndef FIELD505_H
#define FIELD505_H

#include <MarcField.h>


class Field505 : public MarcField
{
    public:
        Field505(int nr);
        virtual ~Field505();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD505_H
