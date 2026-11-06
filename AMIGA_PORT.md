# Abuse for AmigaOS 3.x

Abuse is Crack dot Com's 1995 side-scrolling shooter. This is Sam Hocevar's
SDL 1.2 release, abuse 0.8 (2011), on AmigaOS 3.x, 68040 + FPU, built on
acsdl12 (SDL 1.2 and SDL_mixer on ACGame and AHI).

Source: http://abuse.zoy.org/raw-attachment/wiki/download/abuse-0.8.tar.gz,
sha256 0104db5fd2695c9518583783f7aaa7e5c0355e27c5a803840a05aef97f9d3488,
imported unchanged as the first commit.

## Licences

As abuse 0.8's COPYING says (all three texts are in COPYING in the drawer):

- `src/*` (the game, its Lisp engine and imlib): Crack dot Com, public domain.
- `src/sdlport/*`: Anthony Kruize, GNU GPL 2 or later. It is linked in, so
  the program as a whole is under the GPL 2 or later.
- `src/lol/*`, `src/tool/*`: Sam Hocevar, WTFPL.
- `data/*`: Crack dot Com, Justin Cassidy and others, public domain. This
  is the free data set (fRaBs-based), not the original registered data.
- `data/sfx/*`, `data/music/*`: Bobby Prince; he gave Debian permission to
  redistribute them. Free to pass on, but not public domain.
- `amiga/*`: the AmiCradle Team, public domain like Abuse's own sources.

## Build

    make -f amiga/Makefile.amigaos \
        STOVE=/path/to/openamigagcc-16.2.0-open4-x86_64-linux \
        NDK=/path/to/gfopensdk/OpenNDK/Include_H \
        ACSDL=/path/to/amicradle-guest/gameports/acsdl12 \
        ACSDLLIB=/path/to/acsdl12/build \
        ACGAME=/path/to/acgame ACGAMELIB=/path/to/acgame/build-amiga

acsdl12 is built first with its own `Makefile.amigaos`, without its own
copy of ACGame (`ACG=`): the game links acgame.library's stubs
(`libacgame.a`). `package/build-amiga.sh` (STOVE and OUT set) makes the
release archive with its installer, fetching and building acgame,
acsdl12 and zlib at pinned commits. This gives
`build/amigaos/Abuse/` (and `Abuse.info` beside it): the program `Abuse`,
the game data in `data/`, an empty `Saves/` drawer, this ReadMe and the
licences, with GlowIcons on everything, every drawer inside `data/` too.
The icons are made by `port_icons.py` from `amiga/icons/icons.json`; the
art is Abuse's own program icon (`doc/abuse.png`).

## Installing

Unpack the archive anywhere (on AmiCradle: `SYS:Utilities/OpenCompress x
abuse-...lha RAM:`), then double-click `Install-Abuse` beside the `Abuse`
drawer: it asks where to put the game (`Work:Games` by default) and keeps
the `Saves` drawer (saved games and settings) of an earlier install. The
drawer's `Libs` holds the acgame.library the game was built against, and
the game opens that copy first.

## Playing

Run `Abuse` from its drawer, from Workbench or a Shell. 320x200 in 256
colours, AGA or RTG (`ENV:ACGame/Display`). It needs a good deal of memory
(Fast RAM) and a 512 KB stack, which the program sets itself.

The mouse aims and fires (left button) and uses specials (right); the
cursor keys run, jump and climb; Ctrl and left Alt change weapons (the
Amiga has no Insert and one Ctrl). Saved games and
settings (`abuserc`, `gamma.lsp`, the light and data caches) go in
`Saves/`. Sound effects play; the music does not (see below).

## Changes for the Amiga

All under `#ifdef __amigaos__`, beside the upstream code:

- `amiga/config.h` stands in for configure's: no OpenGL (`HAVE_OPENGL` is
  not defined, so the 8-bit software path is used), no TCP/IP
  (`src/net/tcpip.cpp` is left out and `game.cpp` does not include it, so
  there is no network game; the rest of the network code stays and finds
  no protocol).
- Paths: the data is `PROGDIR:data/`, saves `PROGDIR:Saves/`. A name with
  a `:` in it counts as whole, as one starting with `/` does on Unix
  (`specs.cpp`, `game.cpp`, `fileman.cpp`); no `/` is put after a `:` or
  doubled (`PROGDIR:/` and `a//b` mean the parent drawer on AmigaOS);
  `open_FILE` puts no space between prefix and name (an upstream slip).
- `setup.cpp`: no `$HOME/.abuse`; `Saves/` is made if missing. Defaults
  are 320x200 unscaled, double-buffered (one `SDL_Flip` per frame: acsdl12
  shows the whole frame on every `SDL_UpdateRect`, and Abuse calls that for
  each dirty rectangle), and the Amiga's weapon keys. A new `abuserc` says
  the same.
- `sound.cpp`: the `sfx` drawer is looked for with a lock (`fopen` opens
  no drawer here); 11025 Hz mono, the rate of every Abuse sample;
  `Mix_FadeOutChannel` for `Mix_FadeOutGroup`, which acsdl12 lacks; an
  unset chunk pointer is cleared. The music is HMI, made into MIDI for
  SDL_mixer: acsdl12 plays MODs only and has no `Mix_LoadMUS_RW`, so songs
  are not loaded and the game is silent but for its effects.
- `view.cpp`: no `getlogin()`; the player is "Player".
- `amiga/progdir.c`: PROGDIR: is the current directory while it runs, and
  the drawer lock check. `amiga/stack.c`: a 512 KB stack for the Lisp
  engine's recursion. `amiga/pthread_stubs.c`: what libstdc++ asks of
  POSIX threads, for a program with one thread.

## Byte order

Abuse's files (SPE archives, levels, saves, the Lisp caches, demos, the
light table) are little-endian. Abuse 0.8 is big-endian clean: every
16 and 32-bit value goes through `bFILE::read_uint16/32` and
`write_uint16/32` (or `lstl`/`lltl` for packets and level tile maps),
which swap when `BigEndian()` in `src/common.h` finds the machine is
big-endian at run time; on the 68040 they swap. The audit found no raw
read of a multi-byte value: the rest are byte strings, palettes (3-byte
`color`, also 3 bytes on m68k), image rows and 8-bit lengths. SPE entry
sizes and offsets (`specs.cpp`), level tile maps (swapped after reading
and around writing), object counts and Lisp strings all pass through the
swapping calls. The HMI-to-MIDI converter reads bytes explicitly, and is
not used here. WAVs are read by acsdl12, which swaps them.
`WORDS_BIGENDIAN` is set in `amiga/config.h`, though no code reads it.

## Warnings

The build is warning-free with `-Wall`; the warnings Abuse's own code gives
on every machine are turned off by name in `amiga/Makefile.amigaos`, each
with its reason.

State (4 November 2026): builds and links; not yet run in a lab or on a
real Amiga.
