// Last edited by deforce on 11-25-2007
#include <mudlib.h>
#include <security.h>
#include <config.h>

inherit CMD;

#define WIDTH this_user()->query_screen_width()

private void main(string arg)
{
   string array result;
   string name, capname;
   int adv, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for;

   if (!arg)
   {
      out("Usage: addskill <name> [[title] adv str con agi dex per int cha luc for]\n");

      return;
   }

   if (check_privilege("Mudlib:daemons"))
   {
      out("You need to have mudlib daemons privilege.\n");

      return;
   }

   if (sscanf(arg, "%s:%s:%d %d %d %d %d %d %d %d %d %d", name, capname, adv,
      stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for))
   {
      result = SKILL_D->register_skill(lower_case(trim_spaces(name)), ({ capname, adv, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for }));
   }
   else if (sscanf(arg, "%s:%s", name, capname))
   {
      result = SKILL_D->register_skill(lower_case(trim_spaces(name)), ({ capname, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
   }
   else if (sscanf(arg, "%s", name))
   {
      result = SKILL_D->register_skill(lower_case(trim_spaces(name)), ({ title_capitalize(name), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
   }
   else
   {
      out("Bad skill data format.\n");

      return;
   }

   if (!result) { write("ERROR: skill already exists.\n"); }
   else { printf("Added: %s.\n", result[0]); }
}