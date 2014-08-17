// Last edited by deforce on 10-12-2009
#include <mudlib.h>
#include <config.h>

inherit CMD;

#define WIDTH this_user()->query_screen_width()

private void main(string arg)
{
   string array skills = sort_array(SKILL_D->query_skills(), 1);

   if (sizeof(skills))
   {
      string output = "";

      if (arg == "/c")
      {
         string array categories = SKILL_D->query_skill_categories();

         if (sizeof(categories))
         {
            string array uncategorized = skills;

            categories = sort_array(categories, 1);

            foreach (string category in categories)
            {
               output += capitalize(category) + ":\n";

               foreach (string name in sort_array(SKILL_D->query_skills_in_category(category), 1))
               {
                  mixed data = SKILL_D->query_skill(name);

                  output += sprintf("  %-14s  %s\n", name, data[0]);

                  uncategorized -= ({ name });
               }
            }

            if (sizeof(uncategorized))
            {
               output += "Uncategorized:\n";

               foreach (string name in uncategorized)
               {
                  mixed data = SKILL_D->query_skill(name);

                  output += sprintf("  %-14s  %s\n", name, data[0]);
               }
            }
         }
      }
      else if (arg == "/u")
      {
         string array categories = SKILL_D->query_skill_categories();

         if (sizeof(categories))
         {
            string array uncategorized = skills;

            foreach (string category in categories)
            {
               foreach (string name in sort_array(SKILL_D->query_skills_in_category(category), 1))
               {
                  uncategorized -= ({ name });
               }
            }

            if (sizeof(uncategorized))
            {
               output += "Uncategorized skills:\n";
               output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s\n",
                  "Skill", "Full Name", "Advnc", "Str", "Con", "Agi", "Dex", "Per", "Int", "Cha", "Luc", "For");
               output += "  " + repeat_string("-", (WIDTH - 3)) + "\n";

               foreach (string name in uncategorized)
               {
                  mixed data = SKILL_D->query_skill(name);

                  output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d\n",
                     name[0..13], data[0][0..(WIDTH - 62)], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10]);
               }
            }
            else
            {
               output += "There are no uncategorized skills.\n";
            }
         }
      }
      else if (strlen(arg))
      {
         skills = SKILL_D->query_skills_in_category(arg);

         if (sizeof(skills))
         {
            output += capitalize(arg) + " skills:\n";
            output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s\n",
               "Skill", "Full Name", "Advnc", "Str", "Con", "Agi", "Dex", "Per", "Int", "Cha", "Luc", "For");
            output += "  " + repeat_string("-", (WIDTH - 3)) + "\n";

            foreach (string skill in sort_array(skills, 1))
            {
               mixed data = SKILL_D->query_skill(skill);

               output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d\n",
                  skill[0..13], data[0][0..(WIDTH - 62)], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10]);
            }
         }
         else
         {
            mixed data = SKILL_D->query_skill(arg);

            if (sizeof(data))
            {
               output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s\n",
                  "Skill", "Full Name", "Advnc", "Str", "Con", "Agi", "Dex", "Per", "Int", "Cha", "Luc", "For");
               output += "  " + repeat_string("-", (WIDTH - 3)) + "\n";
               output += sprintf("  %-14s  %-" + (WIDTH - 71) + "s  %-5d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d\n",
                  arg[0..13], data[0][0..(WIDTH - 62)], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10]);
            }
            else
            {
               output += "There are no skills in \"" + arg + "\" category.\n";
            }
         }
      }
      else
      {
         output += sprintf("%-14s  %-" + (WIDTH - 71) + "s  %-7s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s\n",
            "Skill", "Full Name", "Advnc", "Str", "Con", "Agi", "Dex", "Per", "Int", "Cha", "Luc", "For");

         output += repeat_string("-", (WIDTH - 1)) + "\n";

         foreach (string name in skills)
         {
            mixed data = SKILL_D->query_skill(name);

            output += sprintf("%-14s  %-" + (WIDTH - 71) + "s  %-5d    %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d\n",
               name[0..13], data[0][0..(WIDTH - 62)], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10]);
         }
      }

      out(output);
   }
}