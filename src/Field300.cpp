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

    //NOTE: total nr of items has been removed from this location.
    //Total number of items will now be recorded in 591

    // goal: everything after a + sign will appear in subfield e
    // goal: to lift out the dimension information from other extent information
    // and put the dimension information in subfield $c

    std::string dimensioninfo = "";
    Helper::Trim(data);
    Helper::MakeLowercase(data);

    // First: everything after a + should come in subfield e.
    // Find the first plus sign, cut out everything after it, and put it in subfield e
    std::size_t pluspos = data.find('+');
    if ((pluspos != data.npos) && (pluspos != data.length()-1))      // also must not be last character, since we need to be able to do +1
    {
        std::string edata = data.substr(pluspos+1);        // everything from AFTER + to the end
        data = data.substr(0,pluspos+1);       // replace the rest of the string
        Helper::Trim(edata);
        MarcField::update('e', edata);
    }

    // now move on to the main contents in subfield a. Cut out any cm info for subfield c.
    std::vector<std::string> datasegments = Helper::Segment(data, ';');
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

    Helper::ReplaceAll(dimensioninfo, "cm.", "cm");     // do not abbreviate
    MarcField::update('c', dimensioninfo);
    MarcField::update('a', data);

    // generate error on potential millimeter info
    if ((data.find(" mm") != std::string::npos) || (data.find("mm ") != std::string::npos) || (data.find("mm.") != std::string::npos))
    {
        if (verbose) throw MarcRecordException("WARNING Field 300: dimensions in mm? : " + data);
    }

}
