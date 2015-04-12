#ifndef PARSERS_H
#define PARSERS_H

#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Option.hpp"

namespace options {

    struct OptionsDescription;
    struct VariablesMap;

    struct ParsedOptions
	{
        explicit ParsedOptions(const OptionsDescription* xdescription)
        : description(xdescription) {}

        std::vector<Option> options;
        const OptionsDescription* description;
    };

    struct BasicCommandLineParser : private detail::Cmdline{

        BasicCommandLineParser(const std::vector<std::string>& args);
        BasicCommandLineParser(int argc, const char* const argv[]);

        BasicCommandLineParser& options(const OptionsDescription& desc);

        ParsedOptions run();

    private:
        const OptionsDescription* desc;
    };

    void parseArgs(int argc, const char* const argv[],
                        const OptionsDescription& desc);
}
#endif
