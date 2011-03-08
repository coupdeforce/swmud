#include <security.h>
#include <classes.h>

inherit M_DAEMON_DATA;

/*
   Format of a skill (should be using a class, ergh)

   key of the map: internal skill name
   data of the map: array:
      0 : Proper Skill Name
      1 : advancement rate (x out of 10,000)
      2,3,4,5,6,7,8,9,10 : str, con, agi, dex, per, int, cha, luc, for
*/

void remove_skill_from_category(string category, string skill_name);
string* query_skills_in_category(string category);
void remove_category(string category);

private mapping skills = ([ ]);
private mapping skill_advance_guild_requirements = ([ ]);
private mapping skill_advance_learn_requirements = ([ ]);
private mapping skill_categories = ([ ]);

#define PRIV_REQUIRED   "Mudlib:daemons"

//:FUNCTION register_skill
// Adds a new skill into the skill daemon, takes the internal name of the skill as the first
// parameter, and the skill data as an array.
int register_skill(string skill, mixed skill_data)
{
   if (!check_privilege(PRIV_REQUIRED))
   {
      error("security: illegal attempt to add a skill\n");
   }

   if (skills[skill]) { return 0; }

   skills[skill] = skill_data;

   save_me();

   return 1;
}

//:FUNCTION remove_skill
// Delete a skill from the skill daemon
int remove_skill(string skill)
{
   if (!check_privilege(PRIV_REQUIRED))
   {
      error("security: illegal attempt to remove a skill\n");
   }

   if (!skills[skill]) { return 0; }

   // delete the skill
   map_delete(skills, skill);
   map_delete(skill_advance_guild_requirements, skill);
   map_delete(skill_advance_learn_requirements, skill);

   foreach (string category in SKILL_D->query_skill_categories())
   {
      if (member_array(skill, query_skills_in_category(category)) > -1)
      {
         remove_skill_from_category(category, skill);
      }
   }

   save_me();

   return 1;
}

int edit_skill(string skill, string capname, int advrate, int stat_str, int stat_con, int stat_agi, int stat_dex, int stat_per, int stat_int, int stat_cha, int stat_luc, int stat_for, string array learn_requirements)
{
   if (!check_privilege(PRIV_REQUIRED))
   {
      error("security: illegal attempt to remove a skill\n");
   }

   if (!skills[skill]) { return 0; }

   skills[skill] = ({ capname, advrate, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for });

   save_me();
}

int is_skill(string skill)
{
   return skills[skill];
}

//:FUNCTION query_skills
// Returns a list of all the skill names.
string array query_skills()
{
   return keys(skills);
}

//:FUNCTION query_skill
// Returns the skill data for a particular skill name
mixed query_skill(string skill_name)
{
   return skills[skill_name];
}

void set_skill_advance_guild_requirements(string skill, string array requirements)
{
   skill_advance_guild_requirements[skill] = requirements;
}

void set_skill_advance_learn_requirements(string skill, string array requirements)
{
   skill_advance_learn_requirements[skill] = requirements;
}

string array query_skill_advance_guild_requirements(string skill_name)
{
   if (skill_advance_guild_requirements[skill_name])
   {
      return skill_advance_guild_requirements[skill_name];
   }

   return ({ });
}

string array query_skill_advance_learn_requirements(string skill_name)
{
   if (skill_advance_learn_requirements[skill_name])
   {
      return skill_advance_learn_requirements[skill_name];
   }

   return ({ });
}

void add_skill_to_category(string category, string skill_name)
{
   if (!skill_categories[category]) { skill_categories[category] = ({ }); }

   skill_categories[category] += ({ skill_name });

   save_me();
}

void remove_skill_from_category(string category, string skill_name)
{
   if (arrayp(skill_categories[category]))
   {
      skill_categories[category] -= ({ skill_name });
   }

   save_me();
}

void remove_category(string category)
{
   map_delete(skill_categories, category);
}

string* query_skills_in_category(string category)
{
   return skill_categories[category];
}

string* query_skill_categories()
{
   return keys(skill_categories);
}

string list_skill_categories()
{
   string category;
   string* categories;
   string skill;
   string* skills;

   string list = "";

   categories = query_skill_categories();

   foreach (category in categories)
   {
      list += category + ":\n";
      skills = query_skills_in_category(category);
      skills = sort_array(skills, 1);
      list += "  " + implode(skills, "\n  ");
      list += "\n";
   }

   return list;
}

string list_uncategorized_skills()
{
   string category;
   string* categories;
   string skill;
   string* skills;
   string* all_skills;
   string list = "List of Uncategorized Skills\n";

   all_skills = query_skills();
   categories = query_skill_categories();

   foreach(category in categories)
   {
      skills = query_skills_in_category(category);

      foreach(skill in skills)
      {
         all_skills -= ({ skill });
      }
   }

   all_skills = sort_array(all_skills, 1);
   list += implode(all_skills, "\n");

   return list;
}