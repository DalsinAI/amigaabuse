/* config.h for AmigaOS 3.x (68040 + FPU) on acsdl12, in place of the one
 * configure makes. No OpenGL (HAVE_OPENGL is left out), no TCP/IP.
 * Byte order: Abuse finds it at run time (BigEndian() in src/common.h);
 * WORDS_BIGENDIAN is set here as well for the m68k, though no code reads it. */
#define PACKAGE "abuse"
#define PACKAGE_NAME "Abuse"
#define PACKAGE_TARNAME "abuse"
#define PACKAGE_VERSION "0.8"
#define PACKAGE_STRING "Abuse 0.8"
#define PACKAGE_BUGREPORT "sam@hocevar.net"
#define PACKAGE_URL ""
#define VERSION "0.8"

#define HAVE_ATEXIT 1
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRSTR 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define HAVE_SDL_SDL_MIXER_H 1
#define STDC_HEADERS 1

#define WORDS_BIGENDIAN 1

/* The data drawer: PROGDIR:data/ (see amiga/progdir.c). */
#define ASSETDIR "PROGDIR:data"
