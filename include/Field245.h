#ifndef FIELD245_H
#define FIELD245_H

#include <MarcField.h>
#include <Helper.h>

#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

class Field245 : public MarcField
{
    public:
        Field245(int nr);
        virtual ~Field245();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD245_H
