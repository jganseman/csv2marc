#include "Field952.h"

Field952::Field952(int nr) : MarcField(nr)
{
    //ctor
}

Field952::~Field952()
{
    //dtor
}

//shelf number and barcode are copied  from field001 at end of record.cpp buildup()
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


        found = data.find("VOORWERP");              // 3D objects
        if (found != std::string::npos) code="RE";

        found = data.find("KIT");
        if (found != std::string::npos) code="RE";

        found = data.find("CD-ROM");                // machine data
        if (found != std::string::npos) code="CD";

        found = data.find("MACHINE");
        if (found != std::string::npos) code="CF";

        found = data.find("AFBEELDING");            // image
        if (found != std::string::npos) code="RE";

        found = data.find("FOTO");
        if (found != std::string::npos) code="RE";

        found = data.find("MICROVORM");             // projectables (dvd etc)
        if (found != std::string::npos) code="CF";

        found = data.find("DVD");
        if (found != std::string::npos) code="CF";

        found = data.find("FILM");
        if (found != std::string::npos) code="CF";

        found = data.find("VIDEO");
        if (found != std::string::npos) code="CF";

        found = data.find("CD");                    // music (cd, dvd)
        if (found != std::string::npos) code="CD";

        found = data.find("LP");
        if (found != std::string::npos) code="VI";

        found = data.find("CASSETTE");
        if (found != std::string::npos) code="CD";

        found = data.find("TAPE");
        if (found != std::string::npos) code="CD";

        found = data.find("GELUID");
        if (found != std::string::npos) code="CD";

        found = data.find("TEKST");
        if (found != std::string::npos) code="BK";

        found = data.find("MUZIEK");            // music is more important than text
        if (found != std::string::npos) code="MU";

        // recognize a manuscript . This is from the Material field
        found = data.find("MANUSC");
        if (found != std::string::npos) code="BK";

        MarcField::update(marcsubfield, code);
    }
    else        // default: just copy the data in the subfield
    {
        MarcField::update(marcsubfield, data);
    }

}
