#ifndef ANY_H
#define ANY_H

#include <sstream>

using namespace std;

namespace options{

    struct Any
    {
        Any()
        {}

        Any(bool)
        {}

        bool empty() const {return true;}

        template<typename T>
        void operator=(T& x)
        {
        	stringstream ss;
        	ss<<x;
        	value = ss.str();
        }

        std::string str() const
        {
        	return value;
        }

    private:
        std::string value;
    };
}

#endif /* ANY_H */
