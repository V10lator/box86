#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "librarian/library_private.h"
#include "x86emu.h"
#include "emu/x86emu_private.h"
#include "callback.h"
#include "librarian.h"
#include "box86context.h"
#include "emu/x86emu_private.h"
#include "callback.h"

const char* gmpName = "libgmp.so.10";
#define LIBNAME gmp

#include "generated/wrappedgmptypes.h"

#include "wrappercallback.h"

// utility functions
#define SUPER() \
GO(0)   \
GO(1)   \
GO(2)   \
GO(3)   \
GO(4)

// alloc_func
#define GO(A)   \
static uintptr_t my_alloc_func_fct_##A = 0;                         \
static void* my_alloc_func_##A(size_t a)                            \
{                                                                   \
    return (void*)RunFunctionFmt(my_alloc_func_fct_##A, "L", a);    \
}
SUPER()
#undef GO
static void* find_alloc_func_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_alloc_func_fct_##A == (uintptr_t)fct) return my_alloc_func_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_alloc_func_fct_##A == 0) {my_alloc_func_fct_##A = (uintptr_t)fct; return my_alloc_func_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgmp.so.10 alloc_func callback\n");
    return NULL;
}
// realloc_func
#define GO(A)   \
static uintptr_t my_realloc_func_fct_##A = 0;                           \
static void* my_realloc_func_##A(void* a, size_t b)                     \
{                                                                       \
    return (void*)RunFunctionFmt(my_realloc_func_fct_##A, "pL", a, b);  \
}
SUPER()
#undef GO
static void* find_realloc_func_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_realloc_func_fct_##A == (uintptr_t)fct) return my_realloc_func_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_realloc_func_fct_##A == 0) {my_realloc_func_fct_##A = (uintptr_t)fct; return my_realloc_func_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgmp.so.10 realloc_func callback\n");
    return NULL;
}
// free_func
#define GO(A)   \
static uintptr_t my_free_func_fct_##A = 0;          \
static void my_free_func_##A(void* a)               \
{                                                   \
    RunFunctionFmt(my_free_func_fct_##A, "p", a);   \
}
SUPER()
#undef GO
static void* find_free_func_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_free_func_fct_##A == (uintptr_t)fct) return my_free_func_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_free_func_fct_##A == 0) {my_free_func_fct_##A = (uintptr_t)fct; return my_free_func_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgmp.so.10 free_func callback\n");
    return NULL;
}

#undef SUPER

EXPORT void my___gmp_get_memory_functions(x86emu_t* emu, void* f_alloc, void* f_realloc, void* f_free)
{
    my->__gmp_get_memory_functions(find_alloc_func_Fct(f_alloc), find_realloc_func_Fct(f_realloc), find_free_func_Fct(f_free));
}

#define CUSTOM_INIT \
    getMy(lib);

#define CUSTOM_FINI \
    freeMy();

#include "wrappedlib_init.h"
