#ifndef MACROS_H
#define MACROS_H

#include <assert.h>
#include <stdlib.h>

/*
===============================================================================================

 Macros: for everyday use and abuse

===============================================================================================
*/

#define MEMCMP(b1, l1, b2, l2)      ((l1) == (l2) && memcmp((b1), (b2), l1) == 0)

/*
===============================================================================================
 */

#define BUFCMP(b1, b2)  \
 ((b1)->GetLength() == (b2)->GetLength() && \
 memcmp((b1)->GetBuffer(), (b2)->GetBuffer(), (b1)->GetLength()) == 0)

/*
===============================================================================================
 */

#define SIZE(a)                     (sizeof((a)) / sizeof((a[0])))

/*
===============================================================================================
 */

#ifdef DEBUG
#define IFDEBUG(expr)   expr
#else
#define IFDEBUG(expr)
#endif

/*
===============================================================================================
 */

#ifdef DEBUG
#define ASSERT(expr)            \
    do {                        \
        if (!(expr))            \
        {                       \
            PrintStackTrace();  \
            assert(expr);       \
        }                       \
    } while (0)
#else
#define ASSERT(expr)                        \
    do {                                    \
        if (!(expr))                        \
        {                                   \
            PrintStackTrace();              \
            bool prev = Log_SetTrace(true); \
            Log_Trace("Failed: " #expr);    \
            Log_Flush();                    \
            if (prev) Log_SetTrace(false);  \
            _exit(1);                       \
        }                                   \
    } while (0)
#endif

#define ASSERT_FAIL()               ASSERT(false)

/*
===============================================================================================
 */

#define STOP_FAIL(code, ...)                                                    \
do {                                                                            \
    Log_SetTarget(LOG_TARGET_STDERR|LOG_TARGET_FILE);                           \
    const char* msg = StaticPrint("" __VA_ARGS__);                              \
    Log_Message("Exiting (%d)%s%s", code, msg ? ": " : "...", msg ? msg : "");  \
    _exit(code);                                                                \
} while (0)

/*
===============================================================================================
 */

#define RESTART(msg)    \
{                       \
    Log_Message(msg);   \
    _exit(2);           \
}

/*
===============================================================================================
 */

#define CS_INT_SIZE(int_type) \
 ((size_t)(0.30103 * sizeof(int_type) * 8) + 2 + 1)

/*
===============================================================================================
 */

#define ABS(a)      ((a) > 0 ? (a) : (-1*(a)))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

/*
===============================================================================================
 */
 
#define STR_AND_LEN(s) s, strlen(s)

/*
===============================================================================================

  It is  a common idiom to use const char string literals (CSL) in the code, 
  and the length of const literals are known at compile time.
 
===============================================================================================
 */

#define CSLLEN(s)           (sizeof(s "") - 1)
#define STR_AND_CSLLEN(s)   s, CSLLEN(s)

/*
===============================================================================================

  All the containers use the same convention for iterating through elements,
  so these macros can be used for the common case.
 
===============================================================================================
 */

#define FOREACH(it, cont)           for (it = (cont).First(); it != NULL; it = (cont).Next(it))
#define FOREACH_BACK(it, cont)      for (it = (cont).Last(); it != NULL; it = (cont).Prev(it))
#define FOREACH_FIRST(it, cont)     for (it = (cont).First(); it != NULL; it = (cont).First())
#define FOREACH_LAST(it, cont)      for (it = (cont).Last(); it != NULL; it = (cont).Last())
#define FOREACH_REMOVE(it, cont)    for (it = (cont).First(); it != NULL; it = (cont).Remove(it))
#define FOREACH_POP(it, cont)       while((it = (cont).Pop()) != NULL)

/*
===============================================================================================

  Compile time assert macro and helpers. Useful when defining structures that contain opaque
  structures to check at compile time that there is enough space for it.
 
===============================================================================================
 */

#define CONCAT2_(a, b)              a##b
#define CONCAT2(a, b)               CONCAT2_(a, b)
#define STATIC_ASSERT(expr, msg)    typedef char CONCAT2(STATIC_ASSERT_,__LINE__) [(expr)?1:-1]

#define STRINGIFY2(x)               #x
#define STRINGIFY(x)                STRINGIFY2(x)

#define TRY_YIELD_RETURN(yieldTimer, start)                                     \
    if (NowClock() - start >= YIELD_TIME)                                       \
    {                                                                           \
        ASSERT(!yieldTimer.IsActive());                                         \
        EventLoop::Add(&yieldTimer);                                            \
        return;                                                                 \
    }

#define TRY_YIELD_BREAK(yieldTimer, start)                                      \
    if (NowClock() - start >= YIELD_TIME)                                       \
    {                                                                           \
        ASSERT(!yieldTimer.IsActive());                                         \
        EventLoop::Add(&yieldTimer);                                            \
        break;                                                                  \
    }

/*
===============================================================================================

  This can be used to mark variables that are not used in order to prevent compiler warning

===============================================================================================
 */

#define UNUSED(var)     (void) var

#endif
