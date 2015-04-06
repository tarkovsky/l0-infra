#include "magellan/magellan.hpp"

#include "l0-infra/options/ProgramOptions.hpp"

using namespace std;
using namespace options;
using namespace hamcrest;

FIXTURE(OptionTest)
{
	SETUP()
	{

	}

	TEARDOWN()
	{

	}

	TEST("can parse 'help' right when input like '--help'")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()("help", "produce help message")
						("filter", "set filter");

		const char* argv[] = {"","--help"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);

		ASSERT_THAT(varMap.has("help"), is(true));
	}

	TEST("can parse 'filter' value = 1 when input like '--filter=1'")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()("filter", "set filter");

		const char* argv[] = {"", "--filter=1"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);

		ASSERT_THAT(varMap.has("filter"), is(true));
		ASSERT_THAT(varMap["filter"].value().str(), is(string("1")));
	}

	TEST("should parse not when given wrong input ")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()("filter", "set filter");

		const char* argv[] = {"", "--hello"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);

		ASSERT_THAT(varMap.has("filter"), is(false));
		ASSERT_THAT(varMap.has("hello"), is(false));
	}

	TEST("should parse variables right when input has short prefix with no value")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()
		        		("help,h", "produce help message")
						("filter,f", "set filter")
						("date", "set date");

		const char* argv[] = {"","-h", "-f", "-D"};
		const int argc = 3;

		VariablesMap varMap = parseArgs(argc, argv, desc);
		ASSERT_THAT(varMap.has("help"), is(true));
		ASSERT_THAT(varMap.has("filter"), is(true));
		ASSERT_THAT(varMap.has("data"), is(false));
	}

	TEST("can parse '-ab' variables as -a and -b ")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()
		        		("help,h", "produce help message")
						("filter,f", "set filter");

		const char* argv[] = {"","-hf"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);

		ASSERT_THAT(varMap.has("help"), is(true));
		ASSERT_THAT(varMap.has("filter"), is(true));
	}

	TEST("can parse '-f=1' as 'filter' with value '1'")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()
			        				("help,h", "produce help message")
									("filter,f", "set filter");

		const char* argv[] = {"","-f=1"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);
		ASSERT_THAT(varMap.has("filter"), is(true));
		ASSERT_THAT(varMap["filter"].value().str(), is(string("1")));
	}

	TEST("should not parse '--f' as 'filter'")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()
			        				("help,h", "produce help message")
									("filter,f", "set filter");

		const char* argv[] = {"","--f"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);
		ASSERT_THAT(varMap.has("filter"), is(false));
	}

	TEST("can parse '-hf=1' as 'filter' with value '1' and 'help'")
	{
		OptionsDescription desc("Allowed options");
		desc.add_options()
			        				("help,h", "produce help message")
									("filter,f", "set filter");

		const char* argv[] = {"","-hf=1"};
		const int argc = 2;

		VariablesMap varMap = parseArgs(argc, argv, desc);

		ASSERT_THAT(varMap.has("help"), is(true));
		ASSERT_THAT(varMap.has("filter"), is(true));
		ASSERT_THAT(varMap["filter"].value().str(), is(string("1")));
	}

};
