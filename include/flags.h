#ifndef __FLAGS_H__
#define __FLAGS_H__

/* Defines the macros and constants used for flag handling.  All flags in
** the system are identified with a single, unique flag indicator (usually
** represented with the 'which' variable).  These unique values are created
** with macros in this header.  The defined sets of flags are adminstrated
** in this file to prevent conflicts (although many of the actual flags in
** a set are defined in other headers).
**
** The functions to manipulate flags are defined in /std/base/flags.c:
**
**  int test_flag(int which)    test a flag -- returns 0 or 1
**  void set_flag(int which)    set a flag
**  void clear_flag(int which)  clear a flag
**
**  void assign_flag(int which, int state)      set or clear a flag
**  int get_flags(int set_key)                  return a whole flag set
*/


/* Flag keys are composed of two parts: an "administered" flag
** set identifier and the flag index.
**
** The set identifiers are administered here to prevent conflict
** among sets.  Many of the flag keys are defined in other
** headers.
**
** Keys are composed with the MakeFlag() macro.
*/
#define MakeFlag(s,i)   (((s) << 5) | ((i) & 0x1F))

#define FlagSet(w)      ((w) >> 5)
#define FlagIndex(w)    ((w) & 0x1F)


// Administered sets
#define STD_FLAGS       0       /* standard flags (setbit.h) */
#define MODULE_FLAGS    1       /* module-specific flags */
#define PLAYER_FLAGS    2       /* player.c flags (playerflags.h) */
#define PLAYER_NP_FLAGS 3       /* player.c non-persist (playerflags.h) */
#define MAILBASE_FLAGS  5       /* mailbase.c flags */

// STD_FLAGS
#define F_INVIS                 MakeFlag(STD_FLAGS, 2)
#define F_TOUCHED               MakeFlag(STD_FLAGS, 4)
#define F_DESTROYABLE           MakeFlag(STD_FLAGS, 5)
#define F_ATTACHED              MakeFlag(STD_FLAGS, 14)
#define F_TRANSPARENT           MakeFlag(STD_FLAGS, 16)
#define F_AUTOLOAD              MakeFlag(STD_FLAGS, 17)

/// MODULE_FLAGS
#define F_OPEN                  MakeFlag(MODULE_FLAGS, 0)
#define F_LIGHTED               MakeFlag(MODULE_FLAGS, 1)
#define F_WIELDED               MakeFlag(MODULE_FLAGS, 2)
#define F_WORN                  MakeFlag(MODULE_FLAGS, 3)
#define F_DAMAGED               MakeFlag(MODULE_FLAGS, 4)
#define F_BROKEN                MakeFlag(MODULE_FLAGS, 5)
#define F_HELD                  MakeFlag(MODULE_FLAGS, 6)
#define F_HIDDEN                MakeFlag(MODULE_FLAGS, 7)
#define F_CONCEALED             MakeFlag(MODULE_FLAGS, 8)
#define F_ADJUSTED              MakeFlag(MODULE_FLAGS, 9)
#define F_FORCE_SENSITIVE       MakeFlag(MODULE_FLAGS, 10)
#define F_HOLSTERED             MakeFlag(MODULE_FLAGS, 11)
#define F_ALTERED               MakeFlag(MODULE_FLAGS, 12)
#define F_ALTERED_WEIGHT        MakeFlag(MODULE_FLAGS, 13)
#define F_ALTERED_VALUE         MakeFlag(MODULE_FLAGS, 14)
#define F_ALTERED_CAPACITY      MakeFlag(MODULE_FLAGS, 15)
#define F_RESIZED               MakeFlag(MODULE_FLAGS, 16)
#define F_STOLEN                MakeFlag(MODULE_FLAGS, 17)
#define F_ACID_EXPOSURE         MakeFlag(MODULE_FLAGS, 18)

// MAILBASE_FLAGS
#define F_BIFF          MakeFlag(MAILBASE_FLAGS, 0)


#endif /* __FLAGS_H__ */