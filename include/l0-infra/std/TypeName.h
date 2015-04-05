#ifndef MAMSDLFKE_UIETUOM_3746_JFOIEWNNVJ_38457_NVHDYW
#define MAMSDLFKE_UIETUOM_3746_JFOIEWNNVJ_38457_NVHDYW

#include <l0-infra/std/stdext.h>
#include <string>
#include <typeinfo>

STDEXT_NS_BEGIN

///////////////////////////////////////////////////////////////
std::string getDemangledName(const std::type_info& typeInfo);

///////////////////////////////////////////////////////////////
template<typename T>
struct TypeName
{
    static std::string value()
    {
        return getDemangledName(typeid(T));
    }
};

///////////////////////////////////////////////////////////////
template<>
struct TypeName<std::string>
{
    static std::string value() { return "std::string"; }
};

///////////////////////////////////////////////////////////////
template<>
struct TypeName<std::nullptr_t>
{
    static std::string value() { return "std::nullptr_t"; }
};

STDEXT_NS_END

#endif
