#include <classes.h>
#include <mudlib.h>
#include <config.h>

inherit CMD;
inherit CLASS_SKILL_DATA;

#define WIDTH this_user()->query_screen_width()

string format_line(string internal_name, mixed data);

private void main(string arg)
{
   string array skills = sort_array(SKILL_D->query_skills(), 1);
   string header = sprintf("%-14s  %-" + (WIDTH - 71) + "s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s  %-3s\n",
      "Skill", "Full Name", "AdR", "AdA", "Str", "Con", "Agi", "Dex", "Per", "Int", "Cha", "Luc", "For")
      + repeat_string("-", (WIDTH - 1)) + "\n";

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
                  mixed data = SKILL_D->get_skill_data(name);

                  output += sprintf("  %-14s  %s\n", name, data->proper_name);

                  uncategorized -= ({ name });
               }
            }

            if (sizeof(uncategorized))
            {
               output += "Uncategorized:\n";

               foreach (string name in uncategorized)
               {
                  mixed data = SKILL_D->get_skill_data(name);

                  output += sprintf("  %-14s  %s\n", name, data->proper_name);
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
               output += "Uncategorized skills:\n" + header;

               foreach (string name in uncategorized)
               {
                  output += format_line(name, SKILL_D->get_skill_data(name));
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
            output += title_capitalize(arg) + " skills:\n" + header;

            foreach (string name in sort_array(skills, 1))
            {
               output += format_line(name, SKILL_D->get_skill_data(name));
            }
         }
         else
         {
            if (SKILL_D->is_skill(arg))
            {
               output += header + format_line(arg, SKILL_D->get_skill_data(arg));
            }
            else
            {
               out("There is no \"" + arg + "\" skill registered.\n");
            }
         }
      }
      else
      {
         output += header;

         foreach (string name in skills)
         {
            output += format_line(name, SKILL_D->get_skill_data(name));
         }
      }

      out(output);
   }
   else
   {
      out("There are no skills registered.\n");
   }
}

string format_line(string internal_name, mixed data)
{
   return sprintf("%-14s  %-" + (WIDTH - 71) + "s  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d\n",
      internal_name[0..13], data->proper_name, data->advance_rate, data->advance_amount, data->weight_str, data->weight_con, data->weight_agi, data->weight_dex, data->weight_per, data->weight_int, data->weight_cha, data->weight_luc, data->weight_for);
}