#include "magellan/magellan.hpp"
#include "l0-infra/options/ProgramOptions.hpp"

USING_OPTIONS_NS

using namespace hamcrest;

FIXTURE(OptionTest)
{
    OptionsDescription desc {"Allowed options"};
    VariablesMap& varMap = VariablesMap::getInstance();

    void give_option_config(std::map<std::string, std::string>&& tbl)
    {
        desc.add(std::move(tbl));
        varMap.clear();
    }

    static const char** to_argv(std::vector<const char*>&& options)
    {
        const char** argv = new const char*[options.size()];

        for (auto i = 0; i < options.size(); i++)
        {
            argv[i] = options[i];
        }

        return argv;
    }

    void when_parse_program_options(std::vector<const char*>&& options)
    {
        auto argv = to_argv(std::move(options));

        SCOPE_EXIT([=] { delete [] argv; });

		varMap.parseArgs(options.size(), argv, desc);
    }

    template <typename T>
    void then_expect_has_option(const std::string& option, Matcher<T>* matcher)
    {
        ASSERT_THAT(varMap.has(option), matcher);
    }

    void then_expect_option_with_value(const std::string& option, const std::string& value)
    {
        ASSERT_THAT(varMap.has(option), be_true());
        ASSERT_THAT(varMap[option], is(value));
    }

    #define and_expect_has_option then_expect_has_option
    #define and_expect_option_with_value then_expect_option_with_value

    TEST("--help")
    {
        give_option_config({
            {"help",   "produce help message"},
            {"filter", "set filter"},
        });

        when_parse_program_options({"","--help"});
        then_expect_has_option("help", be_true());
    }

    TEST("--filter=1")
    {
        give_option_config({
            {"help",   "produce help message"},
            {"filter", "set filter"},
        });

        when_parse_program_options({"", "--filter=1"});
        then_expect_option_with_value("filter", "1");
    }

    TEST("invalid option")
    {
        give_option_config({
            {"help",   "produce help message"},
            {"filter", "set filter"},
        });

        when_parse_program_options({"", "--hello"});

        then_expect_has_option("filter", be_false());
        and_expect_has_option("hello", be_false());
    }

    TEST("short prefix with no value")
    {
        give_option_config({
            {"help,h", "produce help message"},
            {"filter,f", "set filter"},
            {"date", "set date"},
        });

        when_parse_program_options({"","-h", "-f", "-D"});

        then_expect_has_option("help", be_true());
        and_expect_has_option("help", be_true());
        and_expect_has_option("data", be_false());
    }

    TEST("'-hf' == '-h -f'")
    {
        give_option_config({
            {"help,h", "produce help message"},
            {"filter,f", "set filter"},
        });

        when_parse_program_options({"","-hf"});

        then_expect_has_option("help", be_true());
        and_expect_has_option("filter", be_true());
    }

    TEST("'-f=1' parse as 'filter' with value '1'")
    {
        give_option_config({
            {"help,h", "produce help message"},
            {"filter,f", "set filter"},
        });

        when_parse_program_options({"","-f=1"});
        then_expect_option_with_value("filter", "1");
    }

    TEST("'--f' is not valid")
    {
        give_option_config({
            {"help,h", "produce help message"},
            {"filter,f", "set filter"},
        });

        when_parse_program_options({"","--f"});
        then_expect_has_option("filter", be_false());
    }

    TEST("'-hf=1' parse as 'help' and 'filter' with value '1'")
    {
        give_option_config({
            {"help,h", "produce help message"},
            {"filter,f", "set filter"},
        });

        when_parse_program_options({"","-hf=1"});
        then_expect_has_option("help", be_true());
        and_expect_option_with_value("filter", "1");
    }
};
