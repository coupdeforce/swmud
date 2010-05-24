// Last edited by deforce on 04-06-2010
#include <hooks.h>

mixed call_hooks(string, int);

private nosave mapping skills = ([ ]);
private nosave int skill_delay = 0;
private mapping skill_delays = ([ ]);
private nosave string array combat_actions = ({ });

int has_skill_delay();

int set_skill(string skill, int skill_points)
{
   skills[skill] = skill_points;

   return 1;
}

int get_stat_weight(string skill)
{
   int weight = 0;
   mixed skill_data;

   skill_data = SKILL_D->query_skill(skill);

   if (!skill_data)
   {
      error("can't find skill " + skill);
      return 0;
   }

   foreach (int value in skill_data[2..10])
   {
      weight += value;
   }

   return weight;
}

int get_stat_bonus(string skill)
{
   int bonus = 0;
   int player_stat;
   mixed skill_data;

   skill_data = SKILL_D->query_skill(skill);

   if (!skill_data)
   {
      error("can't find skill " + skill);
      return 0;
   }

   // Strength
   if (skill_data[2] != 0)
   {
      bonus += this_body()->query_str() * skill_data[2] / 100;
   }

   // Constitution
   if (skill_data[3] != 0)
   {
      bonus += this_body()->query_con() * skill_data[3] / 100;
   }

   // Agility
   if (skill_data[4] != 0)
   {
      bonus += this_body()->query_agi() * skill_data[4] / 100;
   }

   // Dexterity
   if (skill_data[5] != 0)
   {
      bonus += this_body()->query_dex() * skill_data[5] / 100;
   }

   // Perception
   if (skill_data[6] != 0)
   {
      bonus += this_body()->query_per() * skill_data[6] / 100;
   }

   // Intelligence
   if (skill_data[7] != 0)
   {
      bonus += this_body()->query_int() * skill_data[7] / 100;
   }

   // Charisma
   if (skill_data[8] != 0)
   {
      bonus += this_body()->query_cha() * skill_data[8] / 100;
   }

   // Luck
   if (skill_data[9] != 0)
   {
      bonus += this_body()->query_luc() * skill_data[9] / 100;
   }

   // Force
   if (skill_data[10] != 0)
   {
      bonus += this_body()->query_for() * skill_data[10] / 100;
   }

   return bonus;
}

//:FUNCTION test_skill
// Test whether the adversary's skill succeeds against a given opposing skill
// level.  Returns 1 for success of this adversary.
int test_skill(string skill, int adjustment)
{
   int roll = random(1000);
   int skill_value = 0;
   int destination_value;
   int stat_bonus = get_stat_bonus(skill);
   float stat_weight = get_stat_weight(skill);

   if (skills[skill])
   {
      skill_value = skills[skill] + call_hooks("all_skill_bonus", HOOK_SUM) + call_hooks(skill + "_skill_bonus", HOOK_SUM);
   }

   if (stat_weight > 0)
   {
      destination_value = skill_value + adjustment + (stat_bonus / stat_weight * (1000.0 - skill_value));
   }
   else
   {
      destination_value = skill_value + adjustment;
   }

   if (destination_value > 950) { destination_value = 950; }
   else if (destination_value < 50) { destination_value = 50; }

   // semi-hack.. return 1 if roll == 0, 1 in 1000 chance of success
   if (roll == 0) { return 1; }

   return (destination_value > roll);
}

mapping get_skills()
{
   return skills;
}

int query_skill(string skill)
{
   return skills[skill] + call_hooks("all_skill_bonus", HOOK_SUM) + call_hooks(skill + "_skill_bonus", HOOK_SUM);
}

void add_combat_action(string value)
{
   if (strlen(value)) { combat_actions += ({ value }); }
}

string array query_combat_actions()
{
   return combat_actions;
}

void perform_combat_action()
{
   if (sizeof(combat_actions))
   {
      string action = choice(combat_actions);

      if (action[0..0] == "&")
      {
         call_other(this_object(), action[1..]);

         return;
      }

      this_object()->do_game_command(action);
   }
}

//:FUNCTION add_skill_delay
// Adds a specificed skill delay in seconds
void add_skill_delay(int delay)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add skill delay of " + delay + " to " + this_object()->short(), previous_object(-1)); }

   skill_delay = time() + has_skill_delay() + delay;
}

//:FUNCTION has_skill_delay
// Returns 0 if the player can use skills again, the remaing time if not
int has_skill_delay()
{
   if (time() < skill_delay)
   {
      return skill_delay - time();
   }

   skill_delay = 0;

   return 0;
}

//:FUNCTION query_skill_delays
// Returns a mapping of any special skill delays
mapping query_skill_delays()
{
   foreach (string skill in keys(skill_delays))
   {
      if (skill_delays[skill] < time())
      {
         map_delete(skill_delays, skill);
      }
   }

   return skill_delays;
}

//:FUNCTION add_special_skill_delay
// Adds a specificed skill delay in seconds to the specified skill
void add_special_skill_delay(string skill, int delay)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add special skill delay of " + delay + " to " + this_object()->short() + " for " + skill, previous_object(-1)); }

   skill_delays[skill] = time() + delay;
}

//:FUNCTION has_special_skill_delay
// Returns 0 if the player can use the specified skill again, the remaing time if not
int has_special_skill_delay(string skill)
{
   if (skill_delays[skill] && (time() < skill_delays[skill]))
   {
      return skill_delays[skill] - time();
   }

   map_delete(skill_delays, skill);

   return 0;
}