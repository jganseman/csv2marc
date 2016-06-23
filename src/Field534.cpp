#include "Field534.h"

Field534::Field534(int nr) : MarcField(nr)
{
    //ctor
}

Field534::~Field534()
{
    //dtor
}


void Field534::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // correct a few often occurring mistakes
    // work with statement "Facs. van" as starting point
    Helper::RemoveAllOf(data, "[]");
    data[0] = toupper(data[0]);
    Helper::ReplaceAll(data, "van :", "van");
    Helper::ReplaceAll(data, "van:", "van");
    Helper::ReplaceAll(data, "Facs van", "Facs. van");

    // the data in here should start with: Facs. van
    if (data.find("Facs") != 0)
        throw MarcRecordException("ERROR field 534: facsimile statement should start with 'Facs. van'");

    // remove the phrase "Facs van"
    Helper::ReplaceAll(data, "Facs. van", "");
    Helper::ReplaceAll(data, "Facs.", "");
    Helper::ReplaceAll(data, "Facs", "");
    //subfield p is obligatory and says its a facsimile
    MarcField::update('p', "Facsimile of:");

    // rest of the data gets trimmed and added as a note
    Helper::Trim(data);
    // could be nothing left, then return
    if (data.empty() || data == "")
        return;

    //else put in trimmed data as note
    MarcField::update(marcsubfield, data);
}
