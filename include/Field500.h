#ifndef FIELD500_H
#define FIELD500_H

#include <MarcField.h>


class Field500 : public MarcField
{
    public:
        Field500(int nr);
        virtual ~Field500();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD500_H
