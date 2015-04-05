#ifndef H37F8D484_D7F2_4FB6_A88F_0ED12D5C0CB2
#define H37F8D484_D7F2_4FB6_A88F_0ED12D5C0CB2

#include <l0-infra/base/Default.h>
#include <l0-infra/base/Config.h>

#define ABSTRACT(...) virtual __VA_ARGS__ = 0

#if __SUPPORT_VIRTUAL_OVERRIDE
# define OVERRIDE(...) virtual __VA_ARGS__ override
#else
# define OVERRIDE(...) virtual __VA_ARGS__
#endif

#define EXTENDS(...) , ##__VA_ARGS__
#define IMPLEMENTS(...) EXTENDS(__VA_ARGS__)

#endif

