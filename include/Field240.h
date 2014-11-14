#ifndef FIELD240_H
#define FIELD240_H

#include <MarcField.h>

#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

class Field240 : public MarcField
{
    public:
        Field240(int nr);
        virtual ~Field240();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD240_H
