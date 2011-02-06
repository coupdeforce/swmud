// Last modified by deforce on 03-13-2010
#include <mudlib.h>
#include <config.h>
#include <classes.h>

inherit CMD;

object body;

private void main(string arg)
{
   int skill_value;
   int pure_skill_value;
   string array skills = ({ });
   string array all_skills = ({ });

   if (!arg)
   {
      out("Usage: skill <skill name>\n");

      return;
   }

   arg = lower_case(arg);
   body = this_body();
   skill_value = body->query_skill(arg);
   pure_skill_value = body->query_skill_pure(arg);

   if (pure_skill_value > 0)
   {
      int rank = to_int(floor(skill_value / 100));
      int to_next = skill_value - (rank * 100);
      mixed data = SKILL_D->query_skill(arg);
      string capname;
      string output = sprintf("Rank %i", rank);

      if (rank < 10) { output += sprintf(", %i%% to next rank", to_next); }

      if (data) { capname = data[0]; }

      if (pure_skill_value > skill_value)
      {
         output = "%^BOLD%^%^RED%^" + output + "%^RESET%^";
      }
      else if (skill_value > pure_skill_value)
      {
         output = "%^BOLD%^%^GREEN%^" + output + "%^RESET%^";
      }

      output = capname + " : " + output;

      out(output);

      return;
   }

   skills = body->get_skills();

   if (!sizeof(skills))
   {
      out("You have no skills.\n");

      return;
   }

   all_skills = SKILL_D->query_skills();

   foreach (string name in all_skills)
   {
      string capname;
      mixed data = SKILL_D->query_skill(name);

      if (data) { capname = lower_case(data[0]); }

      if (capname == arg)
      {
         main(name);

         return;
      }
   }

   out("You have no skill in " + arg + ".\n");
}