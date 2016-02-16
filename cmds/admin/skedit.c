#include <classes.h>
#include <mudlib.h>

inherit CMD;
inherit M_INPUT;
inherit CLASS_SKILL_DATA;

private nomask void get_input(int index, string input);

string array titles = ({ "Full Name", "Advance Rate", "Advance Amount", "Success Minimum", "Success Maximum", "Strength Weight", "Constitution Weight", "Agility Weight", "Dexterity Weight", "Perception Weight", "Intelligence Weight", "Charisma Weight", "Luck Weight", "Force Weight" });
mixed data;
string skill;

private void main(string arg)
{
   if (!check_privilege(1) && (SECURE_D->query_is_wizard(this_body()->query_userid()) < 3))
   {
      out("You need to be an admin in order to edit skills.\n");

      return;
   }

   if (!arg)
   {
      out("Usage: skedit <skill name>\n");

      return;
   }

   skill = arg;

   if (SKILL_D->query_skill(skill))
   {
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
      out("There is no \'" + arg + "\' skill.\n");
   }
}

private nomask void get_input(int index, string input)
{
   if (!strlen(input))
   {
      return;
   }

   if (index == 0)
   {
      data[index] = input;

      SKILL_D->edit_skill(skill, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13]);
   }
   else
   {
      data[index] = to_int(input);

      SKILL_D->edit_skill(skill, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13]);
   }
}