#include "Field952.h"

Field952::Field952(int nr) : MarcField(nr)
{
    //ctor
}

Field952::~Field952()
{
    //dtor
}


void Field952::update(char marcsubfield, std::string data)
{
    // see http://wiki.koha-community.org/wiki/Holdings_data_fields_%289xx%29
    // for all details!
    if (data.empty() || data == "")
        return;

    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    std::size_t found;

    if (marcsubfield == '1')        // LOST status
    {
        found = data.find("LOST");
        if (found != std::string::npos)             // item is lost
            MarcField::update(marcsubfield, "1");
    }
    else if (marcsubfield == '7')        // NOT FOR LOAN status
    {
        found = data.find("*");
        if (found != std::string::npos)             // item is not for loan
            MarcField::update(marcsubfield, "1");
    }
    else if (marcsubfield == 'y')        // internal status
    {
        std::string code = "MU";        // default: music

        // NOTE: mostly copied from LDR update routine
        found = data.find("TEKST");
        if (found != std::string::npos) code="BK";      //books

        found = data.find("MACHINE");
        if (found != std::string::npos) code="CF";          // computer files

        found = data.find("AFBEELDING");
        if (found != std::string::npos) code="RE";        // realia

        found = data.find("CD");
        if (found != std::string::npos) code="CD";

        found = data.find("LP");
        if (found != std::string::npos) code="VI";      //vinyl

        found = data.find("VOORWERP");
        if (found != std::string::npos) code="RE";        // realia

        MarcField::update(marcsubfield, code);
    }
    else        // default: just copy the data in the subfield
    {
        MarcField::update(marcsubfield, data);
    }

}
