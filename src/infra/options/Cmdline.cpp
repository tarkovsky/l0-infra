#include <l0-infra/dci/Role.h>
#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Option.hpp"

using namespace std;

OPTIONS_NS_BEGIN

Cmdline::Cmdline(const vector<string>& args)
{
    init(args);
}

Cmdline::Cmdline(int argc, const char** argv)
{
    init(vector<string>(argv+1, argv+argc+!argc));
}

void Cmdline::init(const vector<string>& args)
{
    this->args = args;        
    desc= 0;
}
    
void Cmdline::setDescription(const OptionsDescription& desc)
{
    this->desc= &desc;
}

typedef std::vector<Option> (Cmdline::* style_parser)(const std::string&);

bool Cmdline::tryAllParser(const std::string& arg, vector<Option>& result)
{
    static style_parser style_parsers[] = {&Cmdline::parseLongOption
                                           , &Cmdline::parseShortOption};
    for(auto& parser : style_parsers)
    {
        const auto& next  = (this->*parser)(arg);

        if(next.empty()) continue;

        result.insert(result.begin(), next.begin(), next.end());
        return true;
    }

    return false;
}
		
vector<Option> Cmdline::run()
{
    vector<Option> result;
    for(auto& arg : args)
    {
        if (!tryAllParser(arg, result))
        {
            result.push_back(Option(arg, ""));
        }
    }
    return result;
}

namespace
{
    inline bool isLongOption(const string& tok)
    {
        return tok.size() >= 3 && tok[0] == '-' && tok[1] == '-';
    }

    DEFINE_ROLE(Tok)
    {
        ABSTRACT(void parse(string& name, string& adjacent) const);
    };

    struct LongTok : Tok
    {
        LongTok(const string& tok):tok(tok){}
    private:
        OVERRIDE(void parse( string& name, string& adjacent) const)
            {
                auto  p = tok.find('=');
                if (p != tok.npos)
                {
                    name = tok.substr(2, p-2);
                    adjacent = tok.substr(p+1);
                }
                else
                {
                    name = tok.substr(2);
                }
            }
    private:
        const string& tok;
    };

    struct BaseOptionParser
    {
        BaseOptionParser(const Tok& init): foundOption(0)
            {
                init.parse(name, adjacent);
            }

        bool tryFind(const OptionsDescription& desc)
            {
                return foundOption = desc.find(name);
            }

        void fillOption(vector<Option>& result)
            {
                result.push_back(Option(foundOption->getLongName(), adjacent));
            }

        const OptionDescription* foundOption;		
        string name;
        string adjacent;		
    };


    struct LongOptionParser : BaseOptionParser
    {
        LongOptionParser(const string& tok):BaseOptionParser(LongTok(tok))
            {

            }
    };
}

vector<Option> Cmdline::parseLongOption(const string& tok)
{
    vector<Option> result;

    if (!isLongOption(tok)) return result;

    LongOptionParser parser(tok);

    if(parser.tryFind(*desc))
    {
        parser.fillOption(result);
    }

    return result;
}

namespace
{
    bool isShortOption(const string& tok)
    {
        return tok.size() >= 2 && tok[0] == '-' && tok[1] != '-';
    }

    struct ShortTok : Tok
    {
        ShortTok(const string& tok):tok(tok){}
    private:
        OVERRIDE(void parse(string& name, string& adjacent) const)
            {
                name = tok.substr(0,2);
                adjacent = tok.substr(2);
            }
    private:
        const string& tok;
    };

    struct ShortOptionParser : BaseOptionParser
    {
        ShortOptionParser(const string& tok)
            : BaseOptionParser(ShortTok(tok))
            {
            }

        bool finished() const
            {
                return adjacent.empty();
            }

        void addOption(vector<Option>& result)
            {
                bool hasValue = (adjacent[0] == '=');
                auto value = [&]{adjacent.erase(adjacent.begin());
                                 return adjacent;
                };
                result.push_back(Option(foundOption? foundOption->getLongName(): name
                                        , hasValue? value() : ""));
                if(hasValue) adjacent.clear();
            }

        void goNext()
            {
                name = string("-") + adjacent[0];
                adjacent.erase(adjacent.begin());
            }
    };
}

vector<Option> Cmdline::parseShortOption(const string& tok)
{
    vector<Option> result;
    if (!isShortOption(tok)) return result;

    ShortOptionParser parser(tok);

    while(true)
    {
        if(!parser.tryFind(*desc))
        {
            if(parser.finished())
            {
                parser.addOption(result);
                break;
            }
            parser.goNext();
        }
        else
        {
            parser.addOption(result);
            if(parser.finished()) return result;
            parser.goNext();
        }
    }
    return result;
}

OPTIONS_NS_END
