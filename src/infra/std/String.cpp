#include "l0-infra/std/String.h"
#include "l0-infra/std/Algorithm.h"

STDEXT_NS_BEGIN

std::string to_upper(const std::string& str)
{
    return stdext::reduce(str, std::string(),
            [](auto &result, auto c) { return result += toupper(c); });
}

std::string to_lower(const std::string& str)
{
    return stdext::reduce(str, std::string(),
            [](auto &result, auto c) { return result += tolower(c); });
}

std::string trim(const std::string& str)
{
    return rtrim(ltrim(str));
}

namespace
{
    const std::string space = " \n\r\t";
}

std::string ltrim(const std::string& str)
{
    auto begin = str.find_first_not_of(space);
    return begin == std::string::npos ? str : str.substr(begin);
}

std::string rtrim(const std::string& str)
{
    auto end = str.find_last_not_of(space);
    return end == std::string::npos ? str : str.substr(0, end + 1);
}

STDEXT_NS_END
