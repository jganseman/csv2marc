#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

// this class includes static helper functions , e.g. for string handling, that are used all over the code


class Helper
{
    public:
        Helper();
        virtual ~Helper();

        static void MakeUppercase(std::string& data);
        static void MakeLowercase(std::string& data);
        static void EraseWhitespace(std::string& data);     // erases all whitespace
        static void Trim(std::string& data);        // erases beginning and trailing whitespace

        static std::vector<std::string> Segment(std::string& data, char delim);

        static void Remove(std::string& data, std::string toBeRemoved);
        static void ReplaceAll(std::string& data, std::string toFind, std::string toSubstitute);

    protected:
    private:
};

#endif // HELPER_H
