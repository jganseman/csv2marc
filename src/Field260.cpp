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
        updateLegacy(marcsubfield, data);
    }
    else        // anything else is just inputted. Can still add some spell checks later if needed
    {
        MarcField::update(marcsubfield, data);
    }
}


void Field260::updateLegacy(char marcsubfield, std::string data)
{
        // we assume this is a string according to ISBD principles:
        // place : editor , date ( place printer : name printer , date printer )

        // possibly multiple editors are present. In that case, we throw an error,
        // and the field is better split up in different subfields in the original db

        // from a previous version of code, stringtillnow concatenates everything (was meant for decoupling later)

        std::string isbdstring = data;
        Helper::Trim(isbdstring);

        std::string stringtillnow = "";         // save everything here

        // find the first doublepoint: everything before that is the place
        Helper::Trim(isbdstring);
        if (isbdstring.empty() || isbdstring == "")
            return;

        // find location: everything up to the first :
        size_t found = isbdstring.find(":");
        std::string location = isbdstring.substr(0, found);
        Helper::Trim(location);
        stringtillnow += location;
        if (found == string::npos)
        {
            MarcField::update('a', location);
            //throw MarcRecordException("WARNING field 260 : no location or editor name : "+isbdstring);
            return;
        }

        // editor: everything else up to the first ,
        std::string rest = isbdstring.substr(found+1);

            // first do a check on the amount of commas still present.
            std::vector<std::string> commacount = Helper::Segment(rest, ',');
            if (commacount.size() > 2)
                if (verbose) throw MarcRecordException("WARNING field 260 : too many commas : "+data);

        found = rest.find(",");
        std::string editor = rest.substr(0, found);
        Helper::Trim(editor);
        stringtillnow += "$b";
        stringtillnow += editor;
        if (found == string::npos)
        {
            //MarcField::update('a', stringtillnow);
            MarcField::update('b', editor);
            return;
        }

        // date: everything else up to the first (
        rest = rest.substr(found+1);

            // now do a check on semicolons. useful to find excess data
            commacount = Helper::Segment(rest, ';');
            if (commacount.size() > 1)
                if (verbose) throw MarcRecordException("WARNING field 260 : too many semicolons : "+data);

        found = rest.find("(");
        std::string date = rest.substr(0, found);
        Helper::Trim(date);
            // if date contains more than numbers, x, cop., dep., s.a. : warn

            size_t found2 = date.find_first_not_of("cop.dep[]0123456789xsa- ");
            if (found2 != string::npos)
                if (verbose) throw MarcRecordException("WARNING field 260 : unknown character in date: " + data);

        stringtillnow += "$c";
        stringtillnow += date;
        if (found == string::npos)
        {
            //MarcField::update('a', stringtillnow);
            MarcField::update('c', date);
            return;
        }


        rest = rest.substr(found+1);
        found = rest.find(":");
        std::string printplace = rest.substr(0, found);
        Helper::ReplaceAll(printplace, ")", "");    // if it is the only string: remove closing bracket
        Helper::Trim(printplace);
        if (found == string::npos)  // this is probably the printer, not the place
        {
            stringtillnow += "$f";
            stringtillnow += printplace;
            //MarcField::update('a', stringtillnow);
            MarcField::update('f', printplace);
            //throw MarcRecordException("WARNING field 260 : cannot distinguish printer name/place : "+isbdstring);
            return;
        } else
        {
            stringtillnow += "$e";
            stringtillnow += printplace;
            MarcField::update('e', printplace);
        }

        rest = rest.substr(found+1);
        found = rest.find(",");
        std::string printer = rest.substr(0, found);
        Helper::ReplaceAll(printer, ")", "");    // if it is the only string: remove closing bracket
        Helper::Trim(printer);
        stringtillnow += "$f";
        stringtillnow += printer;
        if (found == string::npos)  // this is probably the printer, not the place
        {
            //MarcField::update('a', stringtillnow);
            MarcField::update('f', printer);
            return;
        }

        rest = rest.substr(found+1);
        found = rest.find(")");
        std::string printdate = rest.substr(0, found);
        Helper::Trim(printdate);
        stringtillnow += "$g";
        stringtillnow += printdate;
        if (found == string::npos)  // this is probably the printer, not the place
        {
            //MarcField::update('a', stringtillnow);
            MarcField::update('g', printdate);
            return;
        }

        rest = rest.substr(found+1);
        if (rest != "")
        {
            // write already what we have anyway
            MarcField::update('a', stringtillnow);
            if (verbose) throw MarcRecordException("ERROR field 260 : found data after closing brackets : " + data);
        }
}
