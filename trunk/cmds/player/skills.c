// Last modified by deforce on 12-24-2007
#include <mudlib.h>
#include <config.h>
#include <hooks.h>

inherit CMD;

#define WIDTH this_user()->query_screen_width()

private void main(string arg)
{
   object body;
   mapping skills;
   string array lines = ({ });
   float columns = floor(WIDTH / 37);
   int line_count;

   if (columns > 1) { columns = floor((WIDTH - ((columns - 1) * 4)) / 37); }

   if (wizardp(this_body()) && find_body(arg)) { body = find_body(arg); }
   else { body = this_body(); }

   skills = body->get_skills();

   if (!sizeof(skills))
   {
      out("You have no skills.\n");

      return;
   }

   foreach (string name in keys(skills))
   {
      string capname;
      string output;
      int array my_skill = skills[name];
      int skill_value;
      int pure_skill_value;
      int rank;
      mixed data = SKILL_D->query_skill(name);

      if (!data) { capname = name + " (invalid)"; }
      else
      {
         capname = data[0];
      }

      if (sizeof(my_skill) != 2)
      {
         my_skill = ({ 0, 0 });
      }

      pure_skill_value = my_skill[0];
      skill_value = pure_skill_value + body->call_hooks("all_skill_bonus", HOOK_SUM) + body->call_hooks(name + "_skill_bonus", HOOK_SUM);

      if (skill_value < 0)
      {
         skill_value = 0;
      }
      else if (skill_value > 1000)
      {
         skill_value = 1000;
      }

      rank = skill_value / 100;
      output = sprintf("%2i", rank);

      if (rank < 10) { output += sprintf("  %2i%%", (skill_value - (rank * 100))); }
      else if (rank == 10) { output += "  ***"; }

      if (pure_skill_value > skill_value)
      {
         output = "%^BOLD%^%^RED%^" + output + "%^RESET%^";
      }
      else if (skill_value > pure_skill_value)
      {
         output = "%^BOLD%^%^GREEN%^" + output + "%^RESET%^";
      }

      lines += ({ sprintf("%-27s : ", capname[0..26]) + output });
   }

   lines = sort_array(lines, 1);
   line_count = ceil(sizeof(lines) / columns);

   if ((columns > 1) && (sizeof(lines) > 1))
   {
      out(sprintf("\n\n%%^YELLOW%%^%-29s %-7s%%^RESET%%^", "Skill", "Rank"));

      if (sizeof(lines) > 4)
      {
         out(repeat_string("    " + sprintf("%%^YELLOW%%^%-29s %-7s%%^RESET%%^", "Skill", "Rank"), columns - 1));
      }
      else
      {
         out("    " + sprintf("%%^YELLOW%%^%-29s %-7s%%^RESET%%^", "Skill", "Rank"));
      }

      out("\n");
      out(repeat_string("-", 37));

      if (sizeof(lines) > 4)
      {
         out(repeat_string("    " + repeat_string("-", 37), columns - 1));
      }
      else
      {
         out("    " + repeat_string("-", 37));
      }

      out("\n");
   }
   else
   {
      outf("\n\n%%^YELLOW%%^%-29s %-7s%%^RESET%%^\n", "Skill", "Rank");
      out(repeat_string("-", 37) + "\n");
   }

   for (int row_count = 0; row_count < line_count; row_count++)
   {
      out(lines[row_count]);

      for (int column_count = 1; column_count < columns; column_count++)
      {
         if (((column_count * line_count) + row_count) < sizeof(lines))
         {
            out("    " + lines[(column_count * line_count) + row_count]);
         }
      }

      out("\n");
   }
}