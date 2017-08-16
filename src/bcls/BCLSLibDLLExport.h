#ifndef _bclslibdllexport_h_
#define _bclslibdllexport_h_


#if defined (_MSC_VER)
#pragma warning(disable: 4267)
#pragma warning(disable: 4800) /*  warning C4800: 'double' : forcing value to bool 'true' or 'false' */
#endif


/* Cmake will define bcls_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define bcls_EXPORTS when
building the BCLS DLL on windows.
*/

#if defined (bcls_BUILT_AS_DYNAMIC_LIB)

#if defined (bcls_EXPORTS)  /* Compiling the MXA DLL/Dylib */
#if defined (_MSC_VER)  /* MSVC Compiler Case */
#define  bcls_EXPORT __declspec(dllexport)
#define EXPIMP_TEMPLATE
#elif (__GNUC__ >= 4)  /* GCC 4.x has support for visibility options */
#define bcls_EXPORT __attribute__ ((visibility("default")))
#endif
#else  /* Importing the DLL into another project */
#if defined (_MSC_VER)  /* MSVC Compiler Case */
#define  bcls_EXPORT __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#elif (__GNUC__ >= 4)  /* GCC 4.x has support for visibility options */
#define bcls_EXPORT __attribute__ ((visibility("default")))
#endif
#endif
#endif

/* If bcls_EXPORT was never defined, define it here */
#ifndef bcls_EXPORT
#define bcls_EXPORT
#define EXPIMP_TEMPLATE
#endif


#endif /* _bcls_DLL_EXPORT_H_ */

