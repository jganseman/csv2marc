#include "Field300.h"

Field300::Field300(int nr): MarcField(nr)
{
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

    // goal: to lift out the dimension information from other extent information
    // and put the dimension information in subfield $c

    //first make  a copy of the data to work on
    std::string datacopy = data;
    std::string dimensioninfo = "";

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
