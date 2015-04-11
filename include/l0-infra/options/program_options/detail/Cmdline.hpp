#ifndef CMDLINE_H
#define CMDLINE_H

#include <string>
#include <vector>

#include "../OptionsDescription.hpp"

namespace options {
	
	struct  Option;

	namespace detail {

    struct Cmdline
	{
        Cmdline(const std::vector<std::string>& args);
        Cmdline(int argc, const char** argv);

        void setDescription(const OptionsDescription& desc);
        std::vector<Option> run();

	private:
        void init(const std::vector<std::string>& args);
        std::vector<Option> parseLongOption(const std::string& arg);
        std::vector<Option> parseShortOption(const std::string& arg);
		bool tryAllParser(const std::string& arg, std::vector<Option>& result);

	private:
        std::vector<std::string> args;
        const OptionsDescription* desc;
    };
    
}}


#endif

