// 04-XX-1998: Iizuka updated to conform to the new adversary code.
// 05-XX-1998: Iizuka added support for multiple resistances.
// On repair, clear the damaged flag and set new class/durability if necessary
#include <hooks.h>

inherit CLASS_EVENT_INFO;

int test_flag(int);
void clear_flag(int);
void assign_flag(int, int);
mixed call_hooks(string, int);

private mapping resistances = ([ ]);
private mapping resistance_bonuses = ([ ]);
private mapping weaknesses = ([ ]);
private mapping weakness_bonuses = ([ ]);
private int armor_class = 1;
private int max_armor_class = 1;
private int armor_bonus = 0;
private int durability = 1;
private int max_durability = 1;
private int chance_to_be_damaged = 5;

void internal_setup()
{
   this_object()->add_save( ({ "armor_class", "max_armor_class", "armor_bonus", "durability", "max_durability", "chance_to_be_damaged", "resistances", "resistance_bonuses", "weaknesses", "weakness_bonuses", "persist_flags" }) );
}

//:FUNCTION set_armor_class
// Set the protection of the particular damage sink, preventing random(class) points of damage
void set_armor_class(int x)
{
   if (x > 0)
   {
      armor_class = x;
      max_armor_class = x;
      durability = x;
      max_durability = x;
   }
}

void decrease_class(int x)
{
   if (!this_object()->is_living())
   {
      armor_class -= x;
      durability -= x;

      assign_flag(F_DAMAGED, 1);

      if (test_flag(F_WORN))
      {
         tell(environment(), "%^ITEM_DAMAGE%^You notice your " + terminal_colour(this_object()->short(), ANSI_D->query_translations()[1]) + " becomes damaged.%^RESET%^\n");
      }

      if ((armor_class <= 0) || (durability <= 0))
      {
         armor_class = 0;
         durability = 0;

         assign_flag(F_BROKEN, 1);

         environment()->my_action("Your $o breaks from damage.", this_object());
         this_object()->do_remove();
      }
   }
}

//:FUNCTION query_armor_class
// Query the current armor class of an object.  Higher is better.
int query_armor_class()
{
   return armor_class;
}

//:FUNCTION set_durability
// Set the durability equal to or lower than armor class, which is independent
// Must be used after set_armor_class()
void set_durability(int x)
{
   if (x <= armor_class)
   {
      durability = x;
      max_durability = x;
   }
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

//:FUNCTION query_max_armor_class
// Query the maximum armor class of an object.  Higher is better.
int query_max_armor_class()
{
   return max_armor_class;
}

//:FUNCTION add_armor_bonus
// Adds an overall modifier of armor class on the object.
void add_armor_bonus(int amount)
{
   armor_bonus += amount;
}

void reset_armor_bonus()
{
   armor_bonus = 0;
}

int query_armor_bonus()
{
   return armor_bonus;
}

class event_info sink_modify_event(class event_info evt)
{
   object wearer = evt->target;
   int damage_before_sink = event_damage(evt);
   int total_armor_class = wearer->query_armor_class() + wearer->query_armor_bonus();
   mapping total_resistances = wearer->query_resistances();
   mapping total_weaknesses = wearer->query_weaknesses();

   if (!mapp(evt->data) || evt->weapon->id("force_reflect")) { return evt; }

   if (this_object()->is_jedi_armor() && wearer->query_guild_level("jedi") && wearer->has_learned_skill("jedi defense"))
   {
      int jedi_alignment = wearer->query_jedi_alignment();

      total_armor_class += wearer->query_jedi_armor_class();

      wearer->test_skill("jedi_defense", jedi_alignment * (jedi_alignment < 0 ? -10 : 20));
   }

   if ((armor_class + armor_bonus) > total_armor_class)
   {
      total_armor_class = armor_class + armor_bonus;
   }

   foreach (string type in keys(resistances) + keys(resistance_bonuses) + keys(total_resistances))
   {
      total_resistances[type] = total_resistances[type] + resistances[type] + resistance_bonuses[type];
   }

   foreach (string type in keys(weaknesses) + keys(weakness_bonuses) + keys(total_weaknesses))
   {
      total_weaknesses[type] = total_weaknesses[type] + weaknesses[type] + weakness_bonuses[type];
   }

   foreach (string type in keys(evt->data))
   {
      int effective_armor_class = total_armor_class;

      if (effective_armor_class > evt->data[type])
      {
         effective_armor_class = evt->data[type];
      }

      effective_armor_class -= total_weaknesses[type];
      effective_armor_class += total_resistances[type];

      if (effective_armor_class > evt->data[type])
      {
         effective_armor_class = evt->data[type];
      }
      else if (effective_armor_class < 0)
      {
         effective_armor_class = 0;
      }

      evt->data[type] -= random(effective_armor_class + 1);
   }

   if (chance_to_be_damaged)
   {
      int damage_chance = chance_to_be_damaged;

      if (((damage_before_sink - event_damage(evt)) / 2) > damage_chance)
      {
         damage_chance = (damage_before_sink - event_damage(evt)) / 2;
      }

      if (damage_chance > random(100))
      {
         // X in 100 chance of the armor getting damaged
         decrease_class(1);
      }
   }
/*
//   if (wizardp(owner(this_object())) && (damage_before_sink > 0) && (damage_before_sink > event_damage(evt)))
   if (damage_before_sink > evt->data[1])
   {
      tell(wearer, "(Your " + this_object()->short() + " adjusted damage from " + damage_before_sink + " to " + event_damage(evt) + ")\n");
   }
*/
   return evt;
}

//:FUNCTION set_resist
// Specifies that for damage of type 'type', the armor class should be increased by 'amt'
void set_resist(string type, int amt)
{
   resistances[type] = amt;

   if (resistances[type] < 1)
   {
      map_delete(resistances, type);
   }
}

//:FUNCTION set_resistances
// Set all the resistances an armor possesses.
void set_resistances(mapping x)
{
   foreach (string type, int amount in x)
   {
      set_resist(type, amount);
   }
}

//:FUNCTION add_resistance_bonus
// Allows for setting temporary resistances
void add_resistance_bonus(string type, int amt)
{
   resistance_bonuses[type] += amt;

   if (resistance_bonuses[type] < 1)
   {
      map_delete(resistance_bonuses, type);
   }
}

void reset_resistance_bonuses()
{
   resistance_bonuses = ([ ]);
}

//:FUNCTION query_resistances
// Returns all the resistances an armor possesses.
mapping query_resistances()
{
   mapping resistances = ([ ]);

   foreach (string type in keys(resistances) + keys(resistance_bonuses))
   {
      resistances[type] = resistances[type] + resistance_bonuses[type];
   }

   return resistances;
}

//:FUNCTION set_weakness
// Specifies that for damage of type 'type', the armor class should be decreased by 'amt'
void set_weakness(string type, int amt)
{
   weaknesses[type] = amt;

   if (weaknesses[type] < 1)
   {
      map_delete(weaknesses, type);
   }
}

void set_weaknesses(mapping weak)
{
   foreach (string type, int amount in weak)
   {
      set_weakness(type, amount);
   }
}

//:FUNCTION add_weakness_bonus
// Allows for setting temporary weaknesses
void add_weakness_bonus(string type, int amt)
{
   weakness_bonuses[type] += amt;

   if (weakness_bonuses[type] < 1)
   {
      map_delete(weakness_bonuses, type);
   }
}

void reset_weakness_bonuses()
{
   weakness_bonuses = ([ ]);
}

//:FUNCTION query_weaknesses
// Returns all the weaknesses an armor possesses.
mapping query_weaknesses()
{
   mapping weaknesses = ([ ]);

   foreach (string type in keys(weaknesses) + keys(weakness_bonuses))
   {
      weaknesses[type] = weaknesses[type] + weakness_bonuses[type];
   }

   return weaknesses;
}

//:FUNCTION is_armor
// Returns 1 if the object is a damage_sink.
int is_armor()
{
   return 1;
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
      max_armor_class += amount;

      if (max_armor_class < 1)
      {
         max_armor_class = 1;
      }

      if (max_durability > max_armor_class)
      {
         max_durability = max_armor_class;
      }

      armor_class = max_armor_class;
      durability = max_durability;

      clear_flag(F_DAMAGED);
   }
}

void do_repair()
{
   object body = this_body();

   if (!test_flag(F_BROKEN))
   {
      int skill_delay = body->has_skill_delay();

      if ((armor_class < max_armor_class) || (durability < max_durability))
      {
         if (!skill_delay)
         {
            int skill_test = body->test_skill("armor_repair", 1);
            int max_repair = (body->query_skill("armor_repair") / 100) + 1;

            body->add_skill_delay(4);

            if (skill_test > 0)
            {
               if ((max_durability - durability) > max_repair)
               {
                  max_durability = durability + max_repair;
                  max_armor_class = armor_class + max_repair;
               }

               armor_class = max_armor_class;
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
            tell(body, "You must wait at least " + skill_delay + " more second" + (skill_delay > 1 ? "s" : "") + " before attempting to repair.\n");
         }
      }
      else
      {
         tell(body, this_object()->short() + " does not need to be repaired.\n");
      }
   }
   else
   {
      tell(body, this_object()->short() + " is broken and cannot be repaired.\n");
   }
}