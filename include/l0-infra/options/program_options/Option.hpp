#ifndef OPTION_Fri_Apr_10_22_35_31_2015_H
#define OPTION_Fri_Apr_10_22_35_31_2015_H

#include <string>
#include <vector>

namespace options {

    struct Option 
    {
        Option();
        Option(const std::string& xstring_key, 
               const std::vector< std::string> &xvalue);

        std::string string_key;
        std::vector< std::string > value;
        std::vector< std::string> original_tokens;
        bool unregistered;
        bool hasValue;

        void dump() const;
    };
}

#endif
