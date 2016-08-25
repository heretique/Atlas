#ifndef DEBUG_H
#define DEBUG_H


#include <assert.h>

#if _DEBUG
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#endif //DEBUG_H
