#include "l0-infra/options/program_options/detail/Cmdline.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

namespace options {

	void parseArgs(int argc, const char* const argv[],
                       const OptionsDescription& desc)
    {
    	auto&  vm = VariablesMap::getInstance();
    	vm.store(BasicCommandLineParser(argc, argv).options(desc).run());
    }
}
