// Last edited by deforce on 09-30-2009
#include <mudlib.h>

inherit CMD;
inherit M_INPUT;

private nomask void get_input(int index, string input);

string array titles = ({ "Full Name", "Advance Rate", "Strength", "Constitution", "Agility", "Dexterity", "Perception", "Intelligence", "Charisma", "Luck", "Force" });
mixed data;
string skill;

private void main(string arg)
{
   if (!check_privilege(1) && (SECURE_D->query_is_wizard(this_body()->query_userid()) < 3))
   {
      out("You need to be a moff in order to add skills.\n");

      return;
   }

   if (!arg)
   {
      out("Usage: skadd <skill name>\n");

      return;
   }

   skill = arg;

   if (!SKILL_D->query_skill(skill))
   {
      SKILL_D->register_skill(skill, ({ title_capitalize(skill), 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
      data = SKILL_D->query_skill(skill);

      out("\n");

      for (int index = sizeof(titles) - 1; index >= 0; index--)
      {
         modal_simple((: get_input, index :), titles[index] + " [" + data[index] + "]: ");
      }

      out("\n");
   }
   else
   {
      out("There is already a \'" + arg + "\' skill.\n");
   }
}

private nomask void get_input(int index, string input)
{
   if (!strlen(input))
   {
   }
   else if (index == 0)
   {
      data[index] = input;

      SKILL_D->register_skill(skill, ({ data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10] }));
   }
   else
   {
      data[index] = to_int(input);

      SKILL_D->register_skill(skill, ({ data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10] }));
   }
}