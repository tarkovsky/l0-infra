#ifndef KLSAFJLKAEFI_NXCN_SDFKWEJLK_FSDYUIWERIUYXC_NMCXHS
#define KLSAFJLKAEFI_NXCN_SDFKWEJLK_FSDYUIWERIUYXC_NMCXHS

template<typename T>
struct Singleton
{
    static T& getInstance()
    {
        static T instance;
        return instance;
    }

protected:
    Singleton() {}
};

#define DEF_SINGLETON(object) struct object : Singleton<object>

#endif

