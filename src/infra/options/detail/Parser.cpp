#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

namespace options {
    CommandLineParser::	CommandLineParser(int argc, const char** argv
        , const OptionsDescription& desc)
        : detail::Cmdline(argc, argv)
        , desc(desc)
	{

	}

    const ParsedOptions&  CommandLineParser::run()
    {
        detail::Cmdline::setDescription(desc);

        parsedOptions = detail::Cmdline::run();
        return *this;
    }
    
    void CommandLineParser::parseArgs()
    {
        auto&  vm = VariablesMap::getInstance();
    	vm.store(run());
    }

    const std::vector<Option>& CommandLineParser::options() const
    {
        return parsedOptions;
    }
    
    const OptionsDescription& CommandLineParser::description() const
    {
        return desc;
    }

}

