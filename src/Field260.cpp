#include "Field260.h"

Field260::Field260(int nr) : MarcField(nr)
{
    //ctor
}

Field260::~Field260()
{
    //dtor
}


void Field260::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // set indicators

    if (marcsubfield == 'L')        // legacy data, requiring additional parsing
    {
        // we assume this is a string according to ISBD principes:
        // place ; place : editor ; editor , date ; date

        // first, get the location out for subfield a
        std::vector<std::string> segments = Helper::Segment(data, ':');
        if (segments.size() > 2)
            throw MarcRecordException("ERROR field 260 : too many semicolons, not ISBD.");

        std::string location = segments[0];
        // replace all ";" by "$a" and input as subfield $a
        Helper::ReplaceAll(location, ";", "$a");
        Helper::Trim(location);
        MarcField::update('a', location);

        // now, the editor(s). Comes after the : , but before any ,

        if (segments.size() == 1)     // if it does not exist, return
            return;

        // separate from date .
        std::vector<std::string> edsegments = Helper::Segment(segments[1], ',');
        if (edsegments.size() > 2)
            throw MarcRecordException("ERROR field 260 : too many commas, not ISBD.");

        std::string editor = edsegments[0];
        Helper::ReplaceAll(editor, ";", "$b");
        Helper::Trim(editor);
        MarcField::update('b', editor);

        //now, process the date : after the comma
        if (edsegments.size() == 1)     // if it does not exist, return
            return;

        std::string dates = edsegments[1];
        Helper::ReplaceAll(dates, ";", "$c");
        Helper::Trim(dates);
        MarcField::update('c', dates);

    }
    else        // anything else is just inputted. Can still add some spell checks later if needed
    {
        MarcField::update(marcsubfield, data);
    }
}


