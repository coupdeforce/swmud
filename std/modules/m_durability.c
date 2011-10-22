#include <toolbox.h>

int test_flag(int);
void clear_flag(int);
void assign_flag(int, int);

private int durability = 1;
private int max_durability = 1;
private int chance_to_be_damaged = 2;
private nosave string repair_skill = "melee repair";
private nosave string learn_requirement = "";
private nosave string array repair_guilds = ({ "engineer" });
private nosave string parts_type = "weapon";

void mudlib_setup()
{
   this_object()->add_save(({ "durability", "max_durability", "chance_to_be_damaged" }));
}

void decrease_durability(int x)
{
   object owner = owner(this_object());
   durability -= x;

   assign_flag(F_DAMAGED, 1);

   if (durability <= 0)
   {
      durability = 0;

      assign_flag(F_BROKEN, 1);
   }

   if (owner)
   {
      if (durability <= 0)
      {
         tell(owner, "%^ITEM_DAMAGE%^You notice your " + this_object()->short() + "%^RESET%^%^ITEM_DAMAGE%^ breaks from damage.%^RESET%^\n");
         this_object()->do_remove();
         this_object()->do_unwield();
      }
      else
      {
         tell(owner, "%^ITEM_DAMAGE%^You notice your " + this_object()->short() + "%^RESET%^%^ITEM_DAMAGE%^ becomes damaged.%^RESET%^\n");
      }
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

string query_repair_skill()
{
   return repair_skill;
}

void set_repair_skill(string new_repair_skill)
{
   repair_skill = new_repair_skill;
}

string query_repair_learn_requirement()
{
   return learn_requirement;
}

void set_repair_learn_requirement(string new_learn_requirement)
{
   learn_requirement = new_learn_requirement;
}

string array query_repair_guilds()
{
   return repair_guilds;
}

void set_repair_guilds(string array new_repair_guilds)
{
   repair_guilds = new_repair_guilds;
}

string query_parts_type()
{
   return parts_type;
}

void set_parts_type(string new_parts_type)
{
   parts_type = new_parts_type;
}

void do_adjust_durability(int amount)
{
   if (test_flag(F_ADJUSTED) || (amount < 1))
   {
      return;
   }

   max_durability += ceil(max_durability * amount / 10.0);
}

void do_repair()
{
   object body = this_body();
   int required_parts = ((member_array(body->query_primary_guild(), repair_guilds) > -1) ? 1 : 3) * (max_durability - durability);
   int found_parts = check_toolboxes_for_parts(parts_type, required_parts);
   int guild_levels = 0;

   if (test_flag(F_BROKEN))
   {
      tell(body, capitalize(this_object()->the_short()) + " is broken and cannot be repaired.\n");

      return;
   }

   if (durability >= max_durability)
   {
      tell(body, capitalize(this_object()->the_short()) + " does not need to be repaired.\n");

      return;
   }

   if (body->has_skill_delay())
   {
      tell(body, "You are too busy to repair something.\n");

      return;
   }

   if (strlen(learn_requirement) && !body->has_learned_skill(learn_requirement))
   {
      tell(body, "You have not learned how to repair " + this_object()->the_short() + ".\n");

      return;
   }

   if (found_parts < required_parts)
   {
      write("You need " + (required_parts - found_parts) + " more " + parts_type + " parts to repair " + this_object()->the_short() + ".\n");

      return;
   }

   body->add_skill_delay(4);

   foreach (string guild in repair_guilds)
   {
      if (guild == body->query_primary_guild())
      {
         guild_levels += body->query_guild_level(guild) * 2;

         body->add_skill_training_points(repair_skill, 2);
      }
      else
      {
         guild_levels += body->query_guild_level(guild);
      }
   }

   if (body->test_skill(repair_skill, guild_levels * 10))
   {
      int max_repair = (body->query_skill(repair_skill) / 100) + 1;

      if ((max_repair < 11) && ((max_durability - durability) > max_repair))
      {
         int new_durability = durability + max_repair;
         this_object()->set_value(this_object()->query_value_pure() * new_durability / max_durability);

         if (this_object()->is_weapon())
         {
            mapping damage_types = this_object()->query_damage_types_pure();

            foreach (string type in keys(damage_types))
            {
               if (damage_types[type] >= max_durability)
               {
                  damage_types[type] = damage_types[type] * new_durability / max_durability;
               }
               else
               {
                  damage_types[type] = to_int(ceil(damage_types[type] * 1.0 * durability / max_durability));
               }

               if (damage_types[type] < 1)
               {
                  map_delete(damage_types, type);
               }
            }

            this_object()->set_damage_types(damage_types);
         }

         if (this_object()->is_armor())
         {
            int armor_class = this_object()->query_max_armor_class();

            if (armor_class >= new_durability)
            {
               armor_class = armor_class * new_durability / max_durability;
            }
            else
            {
               armor_class = to_int(ceil(armor_class * 1.0 * new_durability / max_durability));
            }

            this_object()->set_armor_class(armor_class);
         }

         max_durability = new_durability;
      }

      if (remove_parts_from_toolboxes(parts_type, required_parts) == required_parts)
      {
         durability = max_durability;

         clear_flag(F_DAMAGED);

         body->simple_action("$N $vrepair $o.", this_object());
      }
   }
   else
   {
      if ((100 - body->query_int()) > body->query_skill(repair_skill))
      {
         remove_parts_from_toolboxes(parts_type, random(required_parts) + 1);
      }

      body->simple_action("$N $vfail to repair $o.", this_object());
   }
}