#include "Field300.h"

Field300::Field300(int nr): MarcField(nr)
{
    verbose=false;
    //ctor
}

Field300::~Field300()
{
    //dtor
}


void Field300::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    // make a separate field 300 for the total number of things in this callnumber
    // this is recorded as a single number in 77b, so test for a number
    std::string datacopy2 = data;
    Helper::EraseWhitespace(datacopy2);
    long converted = atol(datacopy2.c_str());
    if (converted)
    {
       // put this number in, and add "units" (not using items as that's used in the interface for available copies)
       (converted > 1) ? MarcField::update('a', datacopy2 + " units") : MarcField::update('a', datacopy2 + " unit");
        return;
    }

    // goal: to lift out the dimension information from other extent information
    // and put the dimension information in subfield $c

    //first make  a copy of the data to work on
    std::string datacopy = data;
    std::string dimensioninfo = "";
    Helper::Trim(datacopy);
    Helper::MakeLowercase(datacopy);
    Helper::ReplaceAll(datacopy, "+", ";");     // to deal with separately recorded parties extent info

    std::vector<std::string> datasegments = Helper::Segment(datacopy, ';');
    int counter=0;

    for (std::vector<std::string>::iterator it = datasegments.begin(); it != datasegments.end(); ++it)
    {
        if (((*it).find("cm") != std::string::npos))
        {
            if (counter>1) dimensioninfo += " ; ";
            dimensioninfo += (*it);
            counter++;
            Helper::Remove(data, (*it));        // lift out of extent
        }
    }

    Helper::Trim(dimensioninfo);
    Helper::Trim(data);

    MarcField::update('c', dimensioninfo);
    MarcField::update('a', data);

    // generate error on potential millimeter info
    if ((data.find(" mm") != std::string::npos) || (data.find("mm ") != std::string::npos) || (data.find("mm.") != std::string::npos))
    {
        if (verbose) throw MarcRecordException("WARNING Field 300: dimensions in mm? : " + data);
    }

}
