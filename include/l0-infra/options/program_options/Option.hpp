#ifndef OPTION_Fri_Apr_10_22_35_31_2015_H
#define OPTION_Fri_Apr_10_22_35_31_2015_H

#include "program_options.h"
#include <string>

OPTIONS_NS_BEGIN

struct Option 
{
    Option(const std::string& key,
           const std::string& value);

    void dump() const;
    const std::string& value() const;
    const std::string& key() const;
        
private:
    std::string _key;
    std::string _value;
};

OPTIONS_NS_END

#endif
