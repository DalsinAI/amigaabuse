/* Abuse finds its data at PROGDIR:data/ and keeps its saves, settings
 * and caches in PROGDIR:Saves/ (src/sdlport/setup.cpp); PROGDIR: is
 * also made the current directory while it runs, for the few names it
 * opens as they are, and the one it was is given back at exit.
 * Abuse_amigaExists() stands in for fopen() on a drawer, which opens
 * nothing on AmigaOS.
 * The AmiCradle Team. Public domain, like Abuse's own sources. */
#include <stdlib.h>
#include <proto/dos.h>

static BPTR progdir_lock;
static BPTR old_dir;

static void Abuse_amigaDone(void)
{
    if (progdir_lock) {
        CurrentDir(old_dir);
        UnLock(progdir_lock);
        progdir_lock = 0;
    }
}

void Abuse_amigaInit(void)
{
    if (progdir_lock)
        return;
    progdir_lock = Lock((CONST_STRPTR)"PROGDIR:", ACCESS_READ);
    if (progdir_lock) {
        old_dir = CurrentDir(progdir_lock);
        atexit(Abuse_amigaDone);
    }
}

int Abuse_amigaExists(const char *path)
{
    BPTR lock = Lock((CONST_STRPTR)path, ACCESS_READ);

    if (!lock)
        return 0;
    UnLock(lock);
    return 1;
}
