#ifndef CMDLINE_H
#define CMDLINE_H

#include <string>
#include <vector>

#include "../Option.hpp"
#include "../OptionsDescription.hpp"

namespace options { namespace detail {

    struct Cmdline
	{
        Cmdline(const std::vector<std::string>& args);
        Cmdline(int argc, const char** argv);

        void setDescription(const OptionsDescription& desc);
        std::vector<Option> run();

	private:
        void init(const std::vector<std::string>& args);
        std::vector<Option> parseLongOption(const string& arg);
        std::vector<Option> parseShortOption(const string& args);


	private:
        std::vector<std::string> args;
        const OptionsDescription* desc;
    };
    
}}


#endif

