#include "Helper.h"

Helper::Helper()
{
    //ctor
}

Helper::~Helper()
{
    //dtor
}



void Helper::MakeUppercase(std::string& data)
{
    if (data.empty())
        return;
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
}

void Helper::MakeLowercase(std::string& data)
{
    if (data.empty())
        return;
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
}

void Helper::EraseWhitespace(std::string& data)
{
    if (data.empty())
        return;
    data.erase(remove_if(data.begin(), data.end(), ::isspace), data.end());
}

void Helper::Trim(std::string& data)
{
    if (data.empty())
        return;

    data = data.erase(data.find_last_not_of(" \n\r\t")+1).substr(data.find_first_not_of(" \n\r\t"));
}

std::vector<std::string> Helper::Segment(std::string& data, char delim)
{
    std::vector<std::string> datasegments;
    std::stringstream datastream(data);
    std::string segment;
    while(std::getline(datastream, segment, delim))
    {
        datasegments.push_back(segment);
    }
    return datasegments;
}

void Helper::Remove(std::string& data, std::string toBeRemoved)
{
    if (data.empty() || toBeRemoved.empty())
        return;
    while (data.find(toBeRemoved) != std::string::npos)
        data.replace(data.find(toBeRemoved), toBeRemoved.length(), "");
}

// remove all characters from string
void Helper::RemoveAllOf(std::string& data, std::string toBeRemoved)
{
    if (data.empty() || toBeRemoved.empty())
        return;
    for(unsigned int i=0; i<toBeRemoved.size(); i++)
    {
        char toFind = toBeRemoved[i];
        size_t found = data.find(toFind);
        while (found != std::string::npos)
        {
            data.replace(found, 1, "");
            found = data.find(toFind);
        }
    }
}

void Helper::ReplaceAll(std::string& data, std::string toFind, std::string toSubstitute)
{
    if (data.empty() || toFind.empty())
        return;
    while (data.find(toFind) != std::string::npos)
        data.replace(data.find(toFind), toFind.length(), toSubstitute);
}


