#include "../Any.hpp"

namespace options { 

    extern std::string arg;
    
    template<class T>
    std::string typed_value<T>::name() const
    {
        std::string const& var = (m_value_name.empty() ? arg : m_value_name);
        std::string msg = "[=" + var  + ")]";
        if (!m_default_value.empty() && !m_default_value_as_text.empty())
            {
                msg += " (=" + m_default_value_as_text + ")";
                return msg;
            }
        return var;
            
    }

    namespace validators {
        const std::string& get_single_string(
            const std::vector<std::string>& v, 
            bool allow_empty = false);
                
    }

    template<class T>
    void validate(Any& v, 
                  const std::vector< std::string>& xs, 
                  T*, long)
    {
        std::string s(validators::get_single_string(xs));
        v = s;
    }

    void validate(Any& v, 
                  const std::vector<std::string>& xs, 
                  bool*,
                  int);

    template<class T>
    void validate(Any& v, 
                  const std::vector<std::string >& s, 
                  std::vector<T>*,
                  int)
    {
    }

    template<class T>
    void typed_value<T>::parse(Any& value_store, 
           const std::vector<std::string>& new_tokens) const
    {
        validate(value_store, new_tokens, (T*)0, 0);
    }

    template<class T>
    typed_value<T>* value()
    {
        return value<T>(0);
    }

    template<class T>
    typed_value<T>* value(T* v)
    {
        typed_value<T>* r = new typed_value<T>(v);

        return r;        
    }

}
