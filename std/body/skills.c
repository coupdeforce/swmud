#include <classes.h>
#include <hooks.h>

int get_stat_bonus(string skill);
int get_stat_weight(string skill);
int query_skill_evaluation();
int query_int();
int get_skill_point_minimum(int adv_rate);
mixed call_hooks(string, int);

private mapping skills = ([ ]);
private string array learned_skills = ({ });
private string array blocked_skills = ({ });
private string array learned_constructions = ({ });
private string array blocked_constructions = ({ });
private string array learned_manufactures = ({ });
private string array blocked_manufactures = ({ });

int set_skill(string skill, int skill_points, int training_points)
{
   if (member_array(skill, SKILL_D->query_skills()) == -1)
   {
      error("Illegal skill; cannot set new skill value for " + skill + " for " + this_object()->short() + ".\n");
   }

   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s skill in " + skill + " to " + skill_points, previous_object(-1)); }

   if (skill_points < 1)
   {
      map_delete(skills, skill);
   }
   else
   {
      skills[skill] = ({ skill_points, training_points });
   }

   return 1;
}

void remove_skill(string skill)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + this_object()->short() + "'s skill in " + skill, previous_object(-1)); }

   if (skills[skill])
   {
      map_delete(skills, skill);
   }
}

void reset_skills()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s skills", previous_object(-1)); }

   skills = ([ ]);
}

mapping get_skills()
{
   return skills;
}

int array get_skill(string skill)
{
   return skills[skill];
}

int array query_skill_pure(string skill)
{
   if (skills[skill] && (sizeof(skills[skill]) == 2))
   {
      return skills[skill][0];
   }

   return 0;
}

int query_skill(string skill)
{
   if (skills[skill] && (sizeof(skills[skill]) == 2))
   {
      int skill_value = skills[skill][0] + call_hooks("all_skill_bonus", HOOK_SUM) + call_hooks(skill + "_skill_bonus", HOOK_SUM);

      if (skill_value < 0)
      {
         skill_value = 0;
      }
      else if (skill_value > 1000)
      {
         skill_value = 1000;
      }

      return skill_value;
   }

   return 0;
}

//:FUNCTION learn_skill
// Add some number of skill points to the given skill, propogating skill
// values up through the parent skills.  Training points are also assigned as appropriate.
void learn_skill(string skill, int value)
{
   int array my_skill;
   int skill_value;
   mixed skill_data;
   float chance;
   string array guilds = this_body()->query_guild_names();
   string array guild_requirements = SKILL_D->query_skill_advance_guild_requirements(skill);
   string array learn_requirements = SKILL_D->query_skill_advance_learn_requirements(skill);

   if (this_object()->is_body()) { this_object()->check_wizard_set("use learn_skill on  " + this_object()->short(), previous_object(-1)); }

   skill_data = SKILL_D->query_skill(skill);

   if (!skill_data)
   {
      error("No skill data for " + skill + ".\n");
   }

   my_skill = skills[skill];

   if (sizeof(my_skill) != 2)
   {
      my_skill = ({ 0, 0 });
   }

   skill_value = my_skill[0];

   if (sizeof(guild_requirements))
   {
      int found_one = 0;

      foreach (string requirement in guild_requirements)
      {
         if (member_array(requirement, guilds) > -1)
         {
            found_one = 1;
         }
      }

      if (!found_one)
      {
         skill_value = 1000;
      }
   }

   foreach (string requirement in learn_requirements)
   {
      if (member_array(requirement, learned_skills) == -1)
      {
         skill_value = 1000;
         break;
      }
   }

   if (skill_value < 1000)
   {
      int advrate = skill_data[1];
      int roll = random(advrate + ((skill_value + (advrate * 10)) / 100));

      // Debug code for advance roll
//      tell(filter_array(all_inventory(environment(this_object())), (: wizardp($1) :)), this_object()->short() + " advance roll for " + skill + ": Is " + roll + " (" + (skill_value * 10) + ") less than " + to_int(ceil(chance)) + "?\n");
//      tell(filter_array(all_inventory(environment(this_object())), (: wizardp($1) :)), this_object()->short() + " skill points for " + skill + ": Is " + my_skill[1] + " >= " + get_skill_point_minimum(advrate) + "?\n");

      if ((roll == 0) || (my_skill[1] >= advrate) || (my_skill[1] >= get_skill_point_minimum(advrate)))
      {
         // Advance
         my_skill[0] += 1;
         my_skill[1] = 0;

         if (my_skill[0] % 100)
         {
            tell(this_object(), "%^SKILL_IMPROVE%^You feel better at " + skill_data[0] + "." + (my_skill[0] < 1000 ? "  You are now " + (my_skill[0] % 100) + "% towards Rank " + ((my_skill[0] / 100) + 1) + "." : "") + "%^RESET%^\n");
         }
         else
         {
            tell(this_object(), "%^SKILL_IMPROVE%^You feel better at " + skill_data[0] + ".  You have acheived Rank " +  (my_skill[0] / 100) + ".%^RESET%^\n");
         }
      }

      skills[skill] = my_skill;
   }
}

//:FUNCTION test_skill
// Test whether this users' skill succeeds against a given opposing skill
// level.  Returns 1 for success of this user.
int test_skill(string skill, int adjustment)
{
   int roll = random(1000);
   int array my_skill = skills[skill];
   int skill_value;
   int destination_value;
   int stat_bonus = get_stat_bonus(skill);
   float stat_weight = get_stat_weight(skill);
   string array guilds = this_body()->query_guild_names();
   string array guild_requirements = SKILL_D->query_skill_advance_guild_requirements(skill);
   string array learn_requirements = SKILL_D->query_skill_advance_learn_requirements(skill);

   if (this_object()->is_body()) { this_object()->check_wizard_set("use test_skill on  " + this_object()->short(), previous_object(-1)); }

   if (sizeof(my_skill) != 2)
   {
      my_skill = ({ 0, 0 });
   }

   skill_value = my_skill[0] + call_hooks("all_skill_bonus", HOOK_SUM) + call_hooks(skill + "_skill_bonus", HOOK_SUM);

//   tell(this_object(), skill + " - bonus: " + stat_bonus + " weight: " + to_int(stat_weight));
   if (stat_weight > 0)
   {
      destination_value = (stat_bonus / stat_weight * 500) + (skill_value / 2) + adjustment;
   }
   else
   {
      destination_value = skill_value + adjustment;
   }

   if (destination_value > 950) { destination_value = 950; }
   else if (destination_value < 50) { destination_value = 50; }

   // Debug code for skillrolls
//   tell(filter_array(all_inventory(environment(this_object())), (: wizardp($1) :)), this_object()->short() + " skill roll for " + skill + ": Is " + destination_value + " more than " + roll + "?\n");

   // semi-hack.. return 1 if roll == 0, 1 in 1000 chance of success
   if (roll == 0) { return 1; }

   if (sizeof(guild_requirements))
   {
      int found_one = 0;

      foreach (string requirement in guild_requirements)
      {
         if (member_array(requirement, guilds) > -1)
         {
            found_one = 1;
         }
      }

      if (!found_one)
      {
         skill_value = 1000;
      }
   }

   foreach (string requirement in learn_requirements)
   {
      if (member_array(requirement, learned_skills) == -1)
      {
         skill_value = 1000;
         break;
      }
   }

   if (skill_value < 1000)
   {
      if (destination_value > roll)
      {
         my_skill[1] += 1;
      }
      else
      {
         int evaluation = query_skill_evaluation();

         if (evaluation < 50)
         {
            my_skill[1] += 2;
         }
         else if (evaluation < 75)
         {
            my_skill[1] += 1 + random(2);
         }
         else
         {
            my_skill[1] += 1;
         }
      }

      skills[skill] = my_skill;

      learn_skill(skill, stat_weight > 0 ? (stat_bonus / stat_weight * adjustment) : adjustment);
   }

   return (destination_value > roll);
}

void add_skill_training_points(string skill, int amount)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + amount + " to skill training points to " + this_object()->short() + "'s " + skill + " skill", previous_object(-1)); }

   if (skills[skill])
   {
      skills[skill][1] += amount;
   }
}

//:FUNCTION query_skill_evaluation
//Returns the player's overall evaluation (0 to 100 percent) of their skill point total.
//This evaluation corresponds to how they are doing with respect to the maximum possible skill level.
int query_skill_evaluation()
{
   int total = 0;

   foreach (string skill in keys(skills))
   {
      int array my_skill = skills[skill];

      total += my_skill[0];
   }

   return sizeof(skills) ? to_int(floor(total / (10 * sizeof(skills)))) : 0;
}

void add_learned_skill(string skill)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + skill + " to " + this_object()->short() + "'s learned skills", previous_object(-1)); }

   if (member_array(skill, learned_skills) == -1)
   {
      learned_skills += ({ skill });
   }
}

void remove_learned_skill(string skill)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + skill + " from " + this_object()->short() + "'s learned skills", previous_object(-1)); }

   if (member_array(skill, learned_skills) > -1)
   {
      learned_skills -= ({ skill });
   }
}

void reset_learned_skills()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s learned skills", previous_object(-1)); }

   learned_skills = ({ });
}

int has_learned_skill(string skill)
{
   if (member_array(skill, learned_skills) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_learned_skills()
{
   return sort_array(learned_skills, 1);
}

void add_learned_construction(string construction)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + construction + " to " + this_object()->short() + "'s learned constructions", previous_object(-1)); }

   if (member_array(construction, learned_constructions) == -1)
   {
      learned_constructions += ({ construction });
   }
}

void remove_learned_construction(string construction)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + construction + " from " + this_object()->short() + "'s learned constructions", previous_object(-1)); }

   if (member_array(construction, learned_constructions) > -1)
   {
      learned_constructions -= ({ construction });
   }
}

void reset_learned_constructions()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s learned constructions", previous_object(-1)); }

   learned_constructions = ({ });
}

int has_learned_construction(string construction)
{
   if (member_array(construction, learned_constructions) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_learned_constructions()
{
   return sort_array(learned_constructions, 1);
}

void add_learned_manufacture(string manufacture)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + manufacture + " to " + this_object()->short() + "'s learned manufactures", previous_object(-1)); }

   if (member_array(manufacture, learned_manufactures) == -1)
   {
      learned_manufactures += ({ manufacture });
   }
}

void remove_learned_manufacture(string manufacture)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + manufacture + " from " + this_object()->short() + "'s learned manufactures", previous_object(-1)); }

   if (member_array(manufacture, learned_manufactures) > -1)
   {
      learned_manufactures -= ({ manufacture });
   }
}

void reset_learned_manufactures()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s learned manufactures", previous_object(-1)); }

   learned_manufactures = ({ });
}

int has_learned_manufacture(string manufacture)
{
   if (member_array(manufacture, learned_manufactures) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_learned_manufactures()
{
   return sort_array(learned_manufactures, 1);
}

void add_blocked_skill(string skill)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + skill + " to " + this_object()->short() + "'s blocked skills", previous_object(-1)); }

   if (member_array(skill, blocked_skills) == -1)
   {
      blocked_skills += ({ skill });
   }
}

void remove_blocked_skill(string skill)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + skill + " from " + this_object()->short() + "'s blocked skills", previous_object(-1)); }

   if (member_array(skill, blocked_skills) > -1)
   {
      blocked_skills -= ({ skill });
   }
}

int has_blocked_skill(string skill)
{
   if (member_array(skill, blocked_skills) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_blocked_skills()
{
   return sort_array(blocked_skills, 1);
}

void add_blocked_construction(string construction)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + construction + " to " + this_object()->short() + "'s blocked constructions", previous_object(-1)); }

   if (member_array(construction, blocked_constructions) == -1)
   {
      blocked_constructions += ({ construction });
   }
}

void remove_blocked_construction(string construction)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + construction + " from " + this_object()->short() + "'s blocked constructions", previous_object(-1)); }

   if (member_array(construction, blocked_constructions) > -1)
   {
      blocked_constructions -= ({ construction });
   }
}

int has_blocked_construction(string construction)
{
   if (member_array(construction, blocked_constructions) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_blocked_constructions()
{
   return sort_array(blocked_constructions, 1);
}

void add_blocked_manufacture(string manufacture)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + manufacture + " to " + this_object()->short() + "'s blocked manufactures", previous_object(-1)); }

   if (member_array(manufacture, blocked_manufactures) == -1)
   {
      blocked_manufactures += ({ manufacture });
   }
}

void remove_blocked_manufacture(string manufacture)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + manufacture + " from " + this_object()->short() + "'s blocked manufactures", previous_object(-1)); }

   if (member_array(manufacture, blocked_manufactures) > -1)
   {
      blocked_manufactures -= ({ manufacture });
   }
}

int has_blocked_manufacture(string manufacture)
{
   if (member_array(manufacture, blocked_manufactures) > -1)
   {
      return 1;
   }

   return 0;
}

string array query_blocked_manufactures()
{
   return sort_array(blocked_manufactures, 1);
}

int get_skill_point_minimum(int adv_rate)
{
   float min = adv_rate;

   switch (query_int())
   {
      case 0..9: min *= 1.00; break;
      case 10..14: min *= 0.99; break;
      case 15..19: min *= 0.98; break;
      case 20..24: min *= 0.97; break;
      case 25..29: min *= 0.96; break;
      case 30..34: min *= 0.95; break;
      case 35..39: min *= 0.94; break;
      case 40..44: min *= 0.92; break;
      case 45..49: min *= 0.90; break;
      case 50..54: min *= 0.88; break;
      case 55..59: min *= 0.86; break;
      case 60..64: min *= 0.84; break;
      case 65..69: min *= 0.82; break;
      case 70..74: min *= 0.80; break;
      case 75..79: min *= 0.78; break;
      case 80..84: min *= 0.75; break;
      case 85..89: min *= 0.72; break;
      case 90..94: min *= 0.68; break;
      case 95..99: min *= 0.64; break;
      case 100..999999: min *= 0.60; break;
   }

   return to_int(ceil(min));
}
