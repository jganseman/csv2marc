#ifndef FIELD100_H
#define FIELD100_H

#include <MarcField.h>

#include <string>
#include <algorithm>
#include <vector>

class Field100 : public MarcField
{
    public:
        Field100(int nr);
        virtual ~Field100();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD100_H
