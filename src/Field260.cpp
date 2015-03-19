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
        // we assume this is a string according to ISBD principles:
        // place : editor , date ( place printer : name printer , date printer )

        // possibly multiple editors are present. the fields are repeatable, but for clarity,
        // TODO do the same trick as with other unique fields: put as 1 string in subfield a


        std::vector<std::string> segments = Helper::Segment(data, ';');

        for (std::vector<std::string>::iterator it = segments.begin(); it != segments.end(); ++it)
        {
            std::string isbdstring = (*it);
            Helper::Trim(isbdstring);

            std::string stringtillnow = "";         // save everything here


            // find the first doublepoint: everything before that is the place
            Helper::Trim(isbdstring);
            if (isbdstring.empty() || isbdstring == "")
                continue;

            // find location: everything up to the first :
            size_t found = isbdstring.find(":");
            std::string location = isbdstring.substr(0, found);
            Helper::Trim(location);
            stringtillnow += location;
            if (found == string::npos)
            {
                MarcField::update('a', stringtillnow);
                throw MarcRecordException("WARNING field 260 : no location or editor name : "+isbdstring);
                continue;
            }

            // editor: everything else up to the first ,
            std::string rest = isbdstring.substr(found+1);
            found = rest.find(",");
            std::string editor = rest.substr(0, found);
            Helper::Trim(editor);
            stringtillnow += "$b";
            stringtillnow += editor;
            if (found == string::npos)
            {
                MarcField::update('a', stringtillnow);
                continue;
            }


            // date: everything else up to the first (
            rest = rest.substr(found+1);
            found = rest.find("(");
            std::string date = rest.substr(0, found);
            Helper::Trim(date);
                // if date contains more than numbers, x, cop., dep., s.a. : warn

                size_t found2 = date.find_first_not_of("cop.dep[]0123456789xsa- ");
                if (found2 != string::npos)
                    throw MarcRecordException("WARNING field 260 : unknown character in date: " + isbdstring);

            stringtillnow += "$c";
            stringtillnow += date;
            if (found == string::npos)
            {
                MarcField::update('a', stringtillnow);
                continue;
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
                MarcField::update('a', stringtillnow);
                throw MarcRecordException("WARNING field 260 : cannot distinguish printer name/place : "+isbdstring);
                continue;
            } else
            {
                stringtillnow += "$e";
                stringtillnow += printplace;
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
                MarcField::update('a', stringtillnow);
                continue;
            }

            rest = rest.substr(found+1);
            found = rest.find(")");
            std::string printdate = rest.substr(0, found);
            Helper::Trim(printdate);
            stringtillnow += "$g";
            stringtillnow += printdate;
            if (found == string::npos)  // this is probably the printer, not the place
            {
                MarcField::update('a', stringtillnow);
                continue;
            }

            rest = rest.substr(found+1);
            if (rest != "")
            {
                // write already what we have anyway
                MarcField::update('a', stringtillnow);
                throw MarcRecordException("ERROR field 260 : found data after brackets : " + rest);
            }

        }  // move on to next ISBD string



        /*
        std::vector<std::string> segments = Helper::Segment(data, ';');

        for (std::vector<std::string>::iterator it = segments.begin(); it != segments.end(); ++it)
        {
            std::string isbdstring = (*it);
            Helper::Trim(isbdstring);

            if (isbdstring.empty() || isbdstring == "")
                continue;

            // get the location out for subfield a
            std::vector<std::string> locsegments = Helper::Segment(isbdstring, ':');

            // first update location in MARC
            std::string location = locsegments[0];
            Helper::Trim(location);
            MarcField::update('a', location);

            // then check whether we have too much info
            if (locsegments.size() > 2)
                throw MarcRecordException("ERROR field 260 : too many semicolons, not ISBD : " + isbdstring);

            if (locsegments.size() == 1)       // if no other info, continue on to next string
                continue;

            // else, the rest of the string should contain editor and date separated by comma
            std::string reststring = locsegments[1];
            Helper::Trim(reststring);
            std::vector<std::string> datesegments = Helper::Segment(reststring, ',');

            // here too, first update editor string
            std::string editor = datesegments[0];
            Helper::Trim(editor);
            MarcField::update('b', editor);

            // then check if we have too many of them
            if (datesegments.size() > 2)
                throw MarcRecordException("ERROR field 260 : too many commas, not ISBD : " + isbdstring);

            // if we have only one comma, after the comma there are the dates
            if (datesegments.size() == 2)
            {
                std::string dates = datesegments[1];
                Helper::Trim(dates);
                MarcField::update('c', dates);
            }
        }
        */


        /*
            // DELETED OLDER VERSION: mixed up ; and other characters

        // first, get the location out for subfield a
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
        */

    }
    else        // anything else is just inputted. Can still add some spell checks later if needed
    {
        MarcField::update(marcsubfield, data);
    }
}


