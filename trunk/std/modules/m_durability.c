// On repair, clear the damaged flag and set new class/durability if necessary
int test_flag(int);
void clear_flag(int);
void assign_flag(int, int);

private int durability = 1;
private int max_durability = 1;
private int chance_to_be_damaged = 2;
private nosave string repair_skill = "melee repair";
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

   if (!test_flag(F_BROKEN))
   {
      if (durability < max_durability)
      {
         if (!body->has_skill_delay())
         {
            int guild_levels = 0;

            body->add_skill_delay(4);

            foreach (string guild in repair_guilds)
            {
               if (guild == body->query_primary_guild())
               {
                  guild_levels += body->query_guild_level(guild) * 2;

                  body->test_skill(repair_skill, 1000);
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
                  this_object()->set_value(this_object()->query_value_pure() * (durability + max_repair) / max_durability);

                  max_durability = durability + max_repair;
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