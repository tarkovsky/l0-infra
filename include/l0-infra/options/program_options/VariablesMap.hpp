#ifndef VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H
#define VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H

#include "program_options.h"
#include <string>
#include <map>

OPTIONS_NS_BEGIN

struct OptionsDescription;
struct ParsedOptions;

struct  VariablesMap : std::map<std::string, std::string>
{
    static VariablesMap&  getInstance();

    void parseArgs(int argc, const char** const argv
                   , const OptionsDescription& desc);

    const std::string& operator[](const std::string& name) const;

    void clear(); 
    bool has(const std::string& name) const;

private:
    using super = std::map<std::string, std::string>;
    void store(const ParsedOptions& options);
};

OPTIONS_NS_END

#endif
