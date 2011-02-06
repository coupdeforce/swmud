// Last edited by deforce on 10-27-2009
// Player flags (PLAYER_FLAGS and PLAYER_NP_FLAGS flag sets).
// To be used with the new flags functions.
// See also: flags.h, userbits.h

#ifndef __PLAYERFLAGS_H__
#define __PLAYERFLAGS_H__

// NOTE: use absolute cuz of simuls usage
#include "/include/flags.h"

#define F_SNOOPABLE          MakeFlag(PLAYER_FLAGS, 3)
#define F_BRIEF              MakeFlag(PLAYER_FLAGS, 6)
#define F_SILENCED           MakeFlag(PLAYER_FLAGS, 9)

// No longer used
//#define F_INVIS              MakeFlag(PLAYER_FLAGS, 0)
//#define F_HIDDEN             MakeFlag(PLAYER_FLAGS, 1)
//#define F_HISTORY            MakeFlag(PLAYER_FLAGS, 2)
//#define F_ANNOUNCE           MakeFlag(PLAYER_FLAGS, 4)
//#define F_ANSI               MakeFlag(PLAYER_FLAGS, 5)
//#define F_DEAD               MakeFlag(PLAYER_FLAGS, 7)
//#define F_DISPLAY_PATH       MakeFlag(PLAYER_FLAGS, 8)

// Non-persistent flags (they clear when you log off)
#define F_IN_EDIT            MakeFlag(PLAYER_NP_FLAGS, 0)
#define F_INACTIVE           MakeFlag(PLAYER_NP_FLAGS, 1)
#define F_FORCE_REFLECT      MakeFlag(PLAYER_NP_FLAGS, 2)
#define F_FORCE_FOCUS        MakeFlag(PLAYER_NP_FLAGS, 3)

#endif /* __PLAYERFLAGS_H__ */