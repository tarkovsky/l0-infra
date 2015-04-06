#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include <iterator>

namespace options {

    namespace detail {
        template< class Iterator>
        std::vector<std::string>
        makeVector(Iterator i, Iterator e)
        {
            std::vector<std::string > result;
            for(; i != e; ++i)
                result.push_back(*i);
            return result;
        }
    }

    BasicCommandLineParser::
    BasicCommandLineParser(const std::vector<std::string>& xargs)
       : detail::Cmdline(xargs), desc(0)
    {}

    BasicCommandLineParser::
	BasicCommandLineParser(int argc, const char* const argv[])
        : detail::Cmdline(detail::makeVector<const char* const*>(argv+1, argv+argc+!argc))
    	, desc(0)
	{

	}

	BasicCommandLineParser&
	BasicCommandLineParser::options(const OptionsDescription& desc)
	{
		detail::Cmdline::setDescription(desc);
		this->desc = &desc;
        
		return *this;
	}
    
    ParsedOptions
    BasicCommandLineParser::run()
    {
        ParsedOptions result(desc);
        result.options = detail::Cmdline::run();

        return ParsedOptions(result);
    }
}

