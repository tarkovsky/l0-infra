#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

OPTIONS_NS_BEGIN

CommandLineParser::	CommandLineParser
    ( int argc, const char** argv, const OptionsDescription& desc)
    : Cmdline(argc, argv), desc(desc)
{
}

const ParsedOptions&  CommandLineParser::run()
{
    Cmdline::setDescription(desc);

    parsedOptions = Cmdline::run();
    return *this;
}
    
const std::vector<Option>& CommandLineParser::options() const
{
    return parsedOptions;
}
    
const OptionsDescription& CommandLineParser::description() const
{
    return desc;
}

OPTIONS_NS_END
