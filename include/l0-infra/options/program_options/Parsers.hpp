#ifndef PARSERS_Thu_Apr_16_23_57_14_2015_H
#define PARSERS_Thu_Apr_16_23_57_14_2015_H

#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Option.hpp"
#include "l0-infra/dci/Role.h"

OPTIONS_NS_BEGIN

struct OptionsDescription;

DEFINE_ROLE(ParsedOptions)
{
    ABSTRACT(const std::vector<Option>& options() const);
    ABSTRACT(const OptionsDescription& description() const);
};

struct CommandLineParser : private Cmdline, ParsedOptions{

    CommandLineParser( int argc, const char** argv
                     , const OptionsDescription& desc);

    const ParsedOptions& run();

private:
    OVERRIDE(const std::vector<Option>& options() const);
    OVERRIDE(const OptionsDescription& description() const);

private:
    std::vector<Option> parsedOptions;
    const OptionsDescription& desc;
};

OPTIONS_NS_END

#endif
