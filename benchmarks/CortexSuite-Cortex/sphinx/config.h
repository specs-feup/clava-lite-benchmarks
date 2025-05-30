/* helper/config.h.  Generated from config.h.in by configure.  */
/* helper/config.h.in.  Generated from configure.in by autoheader.  */

/* Use ALSA library for sound I/O */
/* #undef AD_BACKEND_ALSA */

/* Use Jack library for sound I/O */
/* #undef AD_BACKEND_JACK */

/* No interface for sound I/O */
#define AD_BACKEND_NONE

/* Use OSS for sound I/O */
/* #undef AD_BACKEND_OSS */

/* Use PulseAudio library for sound I/O */
/* #undef AD_BACKEND_PULSEAUDIO */

/* Use WinMM interface for sound I/O */
/* #undef AD_BACKEND_WIN32 */

/* Default radix point for fixed-point */
/* #undef DEFAULT_RADIX */

/* Enable thread safety */
#define ENABLE_THREADS

/* Use Q15 fixed-point computation */
/* #undef FIXED16 */

/* Use fixed-point computation */
/* #undef FIXED_POINT */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if you have the iconv() function. */
/* #define HAVE_ICONV 1 */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `asound' library (-lasound). */
/* #undef HAVE_LIBASOUND */

/* Define to 1 if you have the `blas' library (-lblas). */
/* #undef HAVE_LIBBLAS */

/* Define to 1 if you have the `jack' library (-ljack). */
/* #undef HAVE_LIBJACK */

/* Define to 1 if you have the `lapack' library (-llapack). */
/* #undef HAVE_LIBLAPACK */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #define HAVE_LIBPTHREAD 1 */

/* Define to 1 if you have the `pulse' library (-lpulse). */
/* #undef HAVE_LIBPULSE */

/* Define to 1 if you have the `pulse-simple' library (-lpulse-simple). */
/* #undef HAVE_LIBPULSE_SIMPLE */

/* Define to 1 if the system has the type `long long'. */
/*#define HAVE_LONG_LONG 0*/

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `perror' function. */
#define HAVE_PERROR 1

/* Define to 1 if you have the `popen' function. */
#define HAVE_POPEN 1

/* Define to 1 if you have the <pthread.h> header file. */
/* #define HAVE_PTHREAD_H 1 */

/* Define to 1 if you have the <samplerate.h> header file. */
/* #undef HAVE_SAMPLERATE_H */

/* Define to 1 if you have the <sndfile.h> header file. */
/* #undef HAVE_SNDFILE_H */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST

/* Define to the sub-directory in which libtool stores uninstalled libraries.
 */
#define LT_OBJDIR ".libs/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "sphinxbase"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "sphinxbase 0.8"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "sphinxbase"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.8"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
/*#define SIZEOF_LONG_LONG 4*/

/* Enable debugging output */
/* #undef SPHINX_DEBUG */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable matrix algebra with LAPACK */
#define WITH_LAPACK

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */
