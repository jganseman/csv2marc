#ifndef FIELD264_H
#define FIELD264_H

#include <MarcField.h>


class Field264 : public MarcField
{
    public:
        Field264(int nr);
        virtual ~Field264();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD264_H
