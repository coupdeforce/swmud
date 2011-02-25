/* The concept is a bit abstract, so the name isn't entirely correct.  This
 * is an abstraction of the concept of something that one attacks with;
 * either a weapon or one's self.  It controls the combat messages, and
 * modifies the result based on the target, etc.
 *
 * This means that when you fight with a weapon, the _weapon_ is really
 * attacking, not you.  Some of the names in this are a bit backwards compat, though.
 *
 * April, 1998: Iizuka made significant changes and updated for adversary. */
// On repair, clear the damaged flag and set new class/durability if necessary

inherit CLASS_EVENT_INFO;

int test_flag(int);
void clear_flag(int);
void assign_flag(int, int);

private int weapon_speed = 1;
private int to_hit_base = 0;
private int to_hit_bonus = 0;
private int stun_bonus = 0;
private int slow_bonus = 0;
private int tear_bonus = 0;
private int critical_chance_bonus = 0;
private int critical_multiplier_bonus = 0;
private int durability = 1;
private int max_durability = 1;
private int chance_to_be_damaged = 2;
private string skill_used = "";
private mapping damage_types = ([ ]);
private mapping damage_bonuses = ([ ]);
private nosave string backstab_message = "";
private nosave string death_message = "";
private nosave mapping def_combat_messages = ([]);
private mapping combat_messages = ([]);

void internal_setup()
{
   this_object()->add_save( ({ "damage_types", "damage_bonuses", "durability", "max_durability", "critical_chance_bonus", "critical_multiplier_bonus", "weapon_speed", "to_hit_base", "to_hit_bonus", "stun_bonus", "slow_bonus", "tear_bonus", "chance_to_be_damaged", "persist_flags" }) );
}

int can_swing()
{
   return 1;
}

mixed adjust_my_result(mixed result)
{
   return result;
}

void set_to_hit_base(int x)
{
   to_hit_base = x;
}

int query_to_hit_base(object target)
{
   return to_hit_base;
}

void set_to_hit_bonus(int x)
{
   to_hit_bonus = x;
}

void add_to_hit_bonus(int x)
{
   to_hit_bonus += x;
}

int query_to_hit_bonus(object target)
{
   return to_hit_bonus;
}

void reset_to_hit_bonus()
{
   to_hit_bonus = 0;
}

void add_stun_bonus(int x)
{
   stun_bonus += x;
}

int query_stun_bonus()
{
   return stun_bonus;
}

void reset_stun_bonus()
{
   stun_bonus = 0;
}

void add_slow_bonus(int x)
{
   slow_bonus += x;
}

int query_slow_bonus()
{
   return slow_bonus;
}

void reset_slow_bonus()
{
   slow_bonus = 0;
}

void add_tear_bonus(int x)
{
   tear_bonus += x;
}

int query_tear_bonus()
{
   return tear_bonus;
}

void reset_tear_bonus()
{
   tear_bonus = 0;
}

void add_critical_chance_bonus(int x)
{
   critical_chance_bonus += x;
}

int query_critical_chance_bonus()
{
   return critical_chance_bonus;
}

void reset_critical_chance_bonus()
{
   critical_chance_bonus = 0;
}

void add_critical_multiplier_bonus(int x)
{
   critical_multiplier_bonus += x;
}

int query_critical_multiplier_bonus()
{
   return critical_multiplier_bonus;
}

void reset_critical_multiplier_bonus()
{
   critical_multiplier_bonus = 0;
}

int is_weapon()
{
   return 1;
}

//:FUNCTION set_backstab_message
// Set the targetted_action displayed on a successful backstab with this weapon
void set_backstab_message(string message)
{
   if (strlen(message))
   {
      backstab_message = message;
   }
}

//:FUNCTION query_backstab_message
// Returns the targetted_action displayed on a successful backstab with this weapon
string query_backstab_message()
{
   if (strlen(backstab_message))
   {
      return backstab_message;
   }

   switch (skill_used)
   {
      case "unarmed": return "$N $vdeliver a surprising blow to the back of $p1 neck!";
      case "slash_1h":
      case "slash_2h": return "$N $vbury $p $o into $p1 back!";
      case "strike_1h":
      case "strike_2h": return "$N viciously $vclub $t in the back of the head with $p $o!";
   }

   return "$N $vbackstab $t with $p $o2!";
}

//:FUNCTION set_death_message
// Set the targetted_action for the death message when something is killed with this weapon
void set_death_message(string message)
{
   if (strlen(message))
   {
      death_message = message;
   }
}

//:FUNCTION query_death_message
// Returns the targetted_action for the death message when something is killed with this weapon
string query_death_message()
{
   if (strlen(death_message))
   {
      return death_message;
   }

   return "$N was killed by $N1 with $p1 $o at $o1.";
}

//:FUNCTION set_combat_messages
// Set the set of combat messages which are used by default
void set_combat_messages(string type)
{
   if (!(def_combat_messages = MESSAGES_D->get_messages(type)))
   {
      error("No messages of that type.\n");
   }
}

//:FUNCTION query_combat_message
// Returns the combat message for a given type
mixed query_combat_message(string type)
{
   return combat_messages[type] || def_combat_messages[type];
}

mixed query_combat_messages()
{
   if (sizeof(combat_messages))
   {
      return combat_messages;
   }

   if (sizeof(def_combat_messages))
   {
      return def_combat_messages;
   }

   return MESSAGES_D->get_messages("combat-blunt");
}

private mixed merge_lists(mixed list, mixed item, mixed def)
{
   if (arrayp(list)) { return list + ({ item }); }
   else if (list) { return ({ list, item }); }
   else if (def) { return merge_lists(def, item, 0); }
   else { return item; }
}

void add_combat_message(string type, string msg)
{
   combat_messages[type] = merge_lists(combat_messages[type], msg, def_combat_messages[type]);
}

//:FUNCTION set_damage_type
// Set a damage type for the weapon
void set_damage_type(string type, int amount)
{
   damage_types[type] = amount;
}

mapping query_damage_types()
{
   if (sizeof(damage_types))
   {
      mapping types = ([ ]);

      foreach (string type in keys(damage_types) + keys(damage_bonuses))
      {
         if (floor((damage_types[type] + damage_bonuses[type]) * durability / max_durability) > 0)
         {
            types[type] = to_int(floor((damage_types[type] + damage_bonuses[type]) * durability / max_durability));
         }
      }

      return types;
   }

   return ([ "striking" : 1 ]);
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

void reset_damage_bonuses()
{
   damage_bonuses = ([ ]);
}

mapping query_damage_bonuses()
{
   return damage_bonuses;
}

int query_weapon_class()
{
   if (sizeof(damage_types))
   {
      mapping types = ([ ]);
      int total = 0;

      foreach (string type in keys(damage_types) - keys(damage_bonuses) + keys(damage_bonuses))
      {
         if (floor((damage_types[type] + damage_bonuses[type]) * durability / max_durability) > 0)
         {
            total += to_int(floor((damage_types[type] + damage_bonuses[type]) * durability / max_durability));
         }
      }

      return total;
   }

   return 0;
}

void decrease_class(int x)
{
   object owner = this_object()->query_wielded_by();
   durability -= x;

   assign_flag(F_DAMAGED, 1);

   if (owner && (durability <= 0))
   {
      durability = 0;

      assign_flag(F_BROKEN, 1);

      tell(owner, "%^ITEM_DAMAGE%^You notice your %^RESET%^" + this_object()->short() + "%^ITEM_DAMAGE%^ breaks from damage.%^RESET%^\n");
      this_object()->unwield();
   }
   else if (owner)
   {
      tell(owner, "%^ITEM_DAMAGE%^You notice your %^RESET%^" + this_object()->short() + "%^ITEM_DAMAGE%^ becomes damaged.%^RESET%^\n");
   }
}

//:FUNCTION set_durability
void set_durability(int amount)
{
   durability = amount;

   if (max_durability < durability) { max_durability = durability; }
}

//:FUNCTION set_max_durability
void set_max_durability(int amount)
{
   max_durability = amount;
}

//:FUNCTION query_durability
// Query the current durability of an object.  Higher is better.
int query_durability()
{
   return durability;
}

//:FUNCTION query_max_durability
// Query the current maximum durability of an object.  Higher is better.
int query_max_durability()
{
   return max_durability;
}

//:FUNCTION set_chance_to_be_damaged
// Set the probability for this to be damaged, from 0 (never) to 100 (always)
void set_chance_to_be_damaged(int x)
{
   if ((x >= 0) && (x <= 100))
   {
      chance_to_be_damaged = x;
   }
}

//:FUNCTION query_chance_to_be_damaged
// Query the probability for this to be damaged
int query_chance_to_be_damaged()
{
   return chance_to_be_damaged;
}

void set_weapon_speed(int x)
{
   weapon_speed = x;
}

int query_weapon_speed()
{
   return weapon_speed;
}

string query_skill_used()
{
   return skill_used;
}

void set_skill_used(string new_skill_used)
{
   skill_used = new_skill_used;
}

void do_adjust(int amount)
{
   if (test_flag(F_ADJUSTED))
   {
      return;
   }

   assign_flag(F_ADJUSTED, 1);

   if (!test_flag(F_BROKEN))
   {
      max_durability += amount;

      durability = max_durability;

      clear_flag(F_DAMAGED);

      foreach (string type in keys(damage_types))
      {
         damage_types[type] += amount;

         if (damage_types[type] < 1) { map_delete(damage_types, type); }
      }
   }
}

void do_repair()
{
   object body = this_body();

   if (!test_flag(F_BROKEN))
   {
      if (durability < max_durability)
      {
         if (!body->has_skill_delay())
         {
            body->add_skill_delay(4);

            if (body->test_skill("melee_repair", 1))
            {
               int max_repair = (body->query_skill("melee_repair") / 100) + 1;

               if ((max_repair < 11) && ((max_durability - durability) > max_repair))
               {
                  int permanent_damage = max_durability - (durability + max_repair);

                  this_object()->set_value(this_object()->query_value_pure() * (durability + max_repair) / max_durability);

                  max_durability = durability + max_repair;

                  foreach (string type in keys(damage_types))
                  {
                     damage_types[type] -= permanent_damage;

                     if (damage_types[type] < 1) { map_delete(damage_types, type); }
                  }
               }

               durability = max_durability;

               clear_flag(F_DAMAGED);

               body->simple_action("$N $vrepair $o.", this_object());
            }
            else
            {
               body->simple_action("$N $vfail to repair $o.", this_object());
            }
         }
         else
         {
            tell(body, "You are too busy to repair something.\n");
         }
      }
      else
      {
         tell(body, capitalize(this_object()->the_short()) + " does not need to be repaired.\n");
      }
   }
   else
   {
      tell(body, capitalize(this_object()->the_short()) + " is broken and cannot be repaired.\n");
   }
}

class event_info source_modify_event(class event_info evt)
{
   return evt;
}