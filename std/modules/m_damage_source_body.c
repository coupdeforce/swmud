// Adversary's unarmed weapon

inherit CLASS_EVENT_INFO;

int query_primary_level();
int query_agi();
int query_str();
int query_skill(string skill);

private nosave mapping damage_bonuses = ([ ]);

int can_swing()
{
   return 1;
}

mixed adjust_my_result(mixed result)
{
   return result;
}

varargs int query_to_hit_base(object target)
{
   // Unarmed to-hit penalty or bonus

   if (target)
   {
      return (query_agi() * 100 / target->query_agi()) - 100;
   }

   return 0;
}

mapping query_damage_types()
{
   int level = query_primary_level();
   int strength = query_str();
   int rank = query_skill("unarmed") / 100;
   int unarmed_damage = level * strength * rank / 1000;

   if (unarmed_damage < ((level / 5) + (strength / 5) + rank)) { unarmed_damage = (level / 5) + (strength / 5) + rank; }

   return ([ "striking" : unarmed_damage + damage_bonuses["striking"] ]) + filter(damage_bonuses, (: $1 != "striking" :));
}

mixed query_combat_messages()
{
   return MESSAGES_D->get_messages("combat-unarmed");
}

int is_weapon()
{
   return 1;
}

//:FUNCTION query_backstab_message
// Returns the targetted_action displayed on a successful backstab with this weapon
string query_backstab_message()
{
   return "$N $vdeliver a surprising blow to the back of $p1 neck!";
}

//:FUNCTION query_death_message
// Returns the targetted_action for the death message when something is killed with this weapon
string query_death_message()
{
   return "$N was killed by $N1 with $p1 bare hands at $o1.";
}

//:FUNCTION query_durability
// Query the current durability of an object.  Higher is better.
int query_durability()
{
   return 100;
}

//:FUNCTION query_max_durability
// Query the current max durability of an object.  Higher is better.
int query_max_durability()
{
   return 100;
}

//:FUNCTION query_chance_to_be_damaged
// Query the probability for this to be damaged
int query_chance_to_be_damaged()
{
   return 0;
}

//:FUNCTION set_damage_bonus
// Set an overall modifier of weapon class on the object.  The bonus
// does not get saved, so should only be used for temporary bonuses. (i.e. buffs)
void set_damage_bonus(string type, int bonus)
{
   damage_bonuses[type] = bonus;
}

void add_damage_bonus(string type, int value)
{
   damage_bonuses[type] += value;
}

mapping query_damage_bonuses()
{
   return damage_bonuses;
}

int query_weapon_class()
{
   mapping damage_types = query_damage_types();

   if (sizeof(damage_types))
   {
      int total = 0;

      foreach (string type in keys(damage_types))
      {
         total += damage_types[type];
      }

      return total;
   }

   return 0;
}

int query_weapon_speed()
{
   return 1;
}

string query_skill_used()
{
   return "unarmed";
}

class event_info source_modify_event(class event_info evt)
{
   return evt;
}