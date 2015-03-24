#ifndef _QHDGA6BWYYSJVRGHQ4OVYNFAJPUKWNH9188DUQXLQHRJF6
#define _QHDGA6BWYYSJVRGHQ4OVYNFAJPUKWNH9188DUQXLQHRJF6

#define __DO_JOIN_AGAIN(symbol1, symbol2) symbol1##symbol2
#define __DO_JOIN(symbol1, symbol2) __DO_JOIN_AGAIN(symbol1, symbol2)

#define JOIN(symbol1, symbol2) __DO_JOIN(symbol1, symbol2)

#ifdef __COUNTER__
# define UNIQUE_ID __COUNTER__
#else
# define UNIQUE_ID __LINE__
#endif

# define UNIQUE_NAME(prefix) JOIN(prefix, UNIQUE_ID)

#endif
