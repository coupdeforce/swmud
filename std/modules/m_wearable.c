// Mixed in to create a wearable item.  The client should also mix in M_GETTABLE.
// Created by Rust/Belboz on 09-07-1994
// Updated for "modules" approach by Deathblade on 11-12-1994
// Deathblade switched to flags to pick up attributes on 04-21-1997
// Iizuka made major changes to support the new adversary code on 03-25-1998

#include <flags.h>

void hook_state(string, mixed, int);
string worn_extra_short();
int test_flag(int);
void assign_flag(int, int);
void clear_flag(int);
void on_wear();
void on_remove();

private string array required_learned_skills = ({ });
private mapping required_skills = ([ ]);
private mapping required_guilds = ([ ]);
private string wear_message = "$N $vwear $p $o.";
private string remove_message = "$N $vremove $p $o.";
private string array slots;
private int body_size = 3;
private int resize_count = 0;
private mapping attribute_modifiers = ([ ]);
private mapping attribute_bonuses = ([ ]);
private nosave mapping attribute_hooks = ([ ]);
private mapping skill_bonuses = ([ ]);
private nosave mapping skill_hooks = ([ ]);
private int to_hit_bonus = 0;
private nosave function to_hit_hook;
private int heal_bonus = 0;
private nosave function heal_hook;
private int body_armor_bonus = 0;
private string wear_relation = "over";

void mudlib_setup()
{
   this_object()->add_save(({ "body_size", "slots", "required_learned_skills", "required_skills", "required_guilds", "wear_message", "remove_message", "resize_count", "attribute_modifiers", "attribute_bonuses", "skill_bonuses", "to_hit_bonus", "heal_bonus", "body_armor_bonus", "wear_relation" }));
}

mixed ob_state()
{
   if (!test_flag(F_WORN)) { return 0; }

   return slots;
}

int valid_wear() // Return 1 if they can wear this.
{
   return 1;
}

int valid_remove() // Return 1 if they can remove this.
{
   return 1;
}

//:FUNCTION set_wear_message
//Set the message used when an object is worn.
void set_wear_message(string message)
{
   wear_message = message;
}

//:FUNCTION query_wear_message
//Return the message given when an object is used.
string query_wear_message()
{
   return wear_message;
}

//:FUNCTION set_remove_message
//Set the message used when an object is removed.
void set_remove_message(string message)
{
   remove_message = message;
}

//:FUNCTION query_remove_message
//Return the message given when an object is removed.
string query_remove_message()
{
   return remove_message;
}

//:FUNCTION set_wear_relation
//Sets the relation of the object to the player, such as "over" or "on".
void set_wear_relation(string relation)
{
   wear_relation = relation;
}

//:FUNCTION query_wear_relation
//Return the wear relation.
string query_wear_relation()
{
   return wear_relation;
}

//:FUNCTION add_a_resize
//Increments the resize counter when an object is resized.
void add_a_resize()
{
   resize_count++;
}

//:FUNCTION query_resized()
//Returns the number of times an object has been resized.
int query_resized()
{
   return resize_count;
}

//:FUNCTION set_body_size
//Set the body size that can wear it.
void set_body_size(int value)
{
   if ((value >= 0) && (value <= 5))
   {
      body_size = value;
   }
   else
   {
      body_size = 3;
   }
}

//:FUNCTION query_body_size
//Returns the body size that can wear it.
int query_body_size()
{
   return body_size;
}

//:FUNCTION set_slot
//Sets the body slot that the wearable object takes up.
void set_slot(string which)
{
   slots = ({ which });
}

//:FUNCTION set_slots
//Sets the body slots that the wearable object takes up.
void set_slots(string array which ...)
{
   slots = which;
}

//:FUNCTION add_slot
//Adds a body slots that the wearable object takes up.
void add_slot(string which)
{
   if (member_array(which, slots) == -1)
   {
      slots += ({ which });
   }
}

//:FUNCTION remove_slot
//Removes a body slot that the wearable object takes up.
void remove_slot(string which)
{
   while (member_array(which, slots) > -1)
   {
      slots -= ({ which });
   }
}

//FUNCTION query_slots
//Returns the body slots that the wearable object takes up.
string array query_slots()
{
   return slots;
}

string worn_attributes()
{
   if (sizeof(slots) > 1)
   {
      string array limbs = ({ });

      if (member_array("head", slots) != -1) { limbs += ({ "head" }); }

      if (member_array("torso", slots) != -1) { limbs += ({ "torso" }); }

      if ((member_array("left arm", slots) != -1) && (member_array("right arm", slots) != -1))
      {
         limbs += ({ "arms" });
      }

      if ((member_array("left hand", slots) != -1) && (member_array("right hand", slots) != -1))
      {
         limbs += ({ "hands" });
      }

      if ((member_array("left leg", slots) != -1) && (member_array("right leg", slots) != -1))
      {
         limbs += ({ "legs" });
      }

      if ((member_array("left foot", slots) != -1) && (member_array("right foot", slots) != -1))
      {
         limbs += ({ "feet" });
      }

      if (sizeof(limbs) > 1)
      {
         return " worn " + wear_relation + " " + implode(limbs[0..(sizeof(limbs) - 2)], ", ") +
            " and " + limbs[sizeof(limbs) - 1];
      }
      else
      {
         return " worn " + wear_relation + " " + limbs[0];
      }
   }
   else { return " worn " + wear_relation + " " + environment()->query_slot_name(slots[0]); }
}

void set_attribute_modifiers(mapping modifiers)
{
   attribute_modifiers = modifiers;
}

mapping query_attribute_modifiers()
{
   return attribute_modifiers;
}

int get_attribute_bonus(string type)
{
   return this_object()->query_attribute_modifiers()[type] + attribute_bonuses[type];
}

void set_attribute_bonuses(mapping bonuses)
{
   attribute_bonuses = bonuses;
}

void set_attribute_bonus(string type, int amount)
{
   attribute_bonuses[type] = amount;
}

void add_attribute_bonus(string type, int amount)
{
   attribute_bonuses[type] = attribute_bonuses[type] + amount;
}

void reset_attribute_bonuses()
{
   attribute_bonuses = ([ ]);
}

mapping query_attribute_bonuses()
{
   return attribute_bonuses;
}

int get_skill_bonus(string skill)
{
   return skill_bonuses[skill];
}

void set_skill_bonuses(mapping skills)
{
   skill_bonuses = skills;
}

void set_skill_bonus(string skill, int amount)
{
   skill_bonuses[skill] = amount;
}

void add_skill_bonus(string skill, int amount)
{
   skill_bonuses[skill] = skill_bonuses[skill] + amount;
}

void reset_skill_bonuses()
{
   skill_bonuses = ([ ]);
}

mapping query_skill_bonuses()
{
   return skill_bonuses;
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

void set_heal_bonus(int amount)
{
   heal_bonus = amount;
}

void add_heal_bonus(int amount)
{
   heal_bonus += amount;
}

void reset_heal_bonus()
{
   heal_bonus = 0;
}

int query_heal_bonus()
{
   return heal_bonus;
}

int get_body_armor_bonus()
{
   return body_armor_bonus;
}

void set_body_armor_bonus(int amount)
{
   body_armor_bonus = amount;
}

void add_body_armor_bonus(int amount)
{
   body_armor_bonus += amount;
}

void reset_body_armor_bonus()
{
   body_armor_bonus = 0;
}

int query_body_armor_bonus()
{
   return body_armor_bonus;
}

//:FUNCTION set_worn
//set_worn(1) causes an object to become worn.  set_worn(0) removes it.
void set_worn(int g)
{
   string array affected_attributes = keys(this_object()->query_attribute_modifiers()) - keys(attribute_bonuses) + keys(attribute_bonuses);

   assign_flag(F_WORN, g);
//   hook_state("prevent_drop", "You'll have to take it off first.\n", g);

   if (g)
   {
      to_hit_hook = (: query_to_hit_bonus :);
      owner(this_object())->add_hook("to_hit_bonus", heal_hook);
      heal_hook = (: query_heal_bonus :);
      owner(this_object())->add_hook("heal_rate_bonus", heal_hook);
      owner(this_object())->add_armor_bonus(body_armor_bonus);
   }
   else
   {
      owner(this_object())->remove_hook("to_hit_bonus", heal_hook);
      owner(this_object())->remove_hook("heal_rate_bonus", heal_hook);
      owner(this_object())->add_armor_bonus(body_armor_bonus * -1);
   }

   foreach (string type in affected_attributes)
   {
      if (g)
      {
         attribute_hooks[type] = (: get_attribute_bonus, type :);
         owner(this_object())->add_hook(type + "_bonus", attribute_hooks[type]);
      }
      else
      {
         owner(this_object())->remove_hook(type + "_bonus", attribute_hooks[type]);
      }
   }

   foreach (string skill in keys(skill_bonuses))
   {
      if (g)
      {
         skill_hooks[skill] = (: get_skill_bonus, skill :);
         owner(this_object())->add_hook(skill + "_skill_bonus", skill_hooks[skill]);
      }
      else
      {
         owner(this_object())->remove_hook(skill + "_skill_bonus", skill_hooks[skill]);
      }
   }

   owner(this_object())->refresh_stats();

   if (g)
   {
      on_wear();
   }
   else
   {
      on_remove();
   }
}

void remove()
{
   object env = environment();

   if (!slots || !env || !env->is_living()) { return 0; }

   env->remove_item(this_object());

   set_worn(0);
}

void do_wear()
{
   if (!slots) { return; }

   if (test_flag(F_BROKEN))
   {
      environment()->simple_action("$N $vdiscover that the $o is too broken to wear.", this_object());

      return;
   }

   if (!environment()->wear_item(this_object(), slots))
   {
      if (test_flag(F_WORN))
      {
         environment()->simple_action("$N $vdiscover $ns $vare already wearing the $o.", this_object());
      }
      else
      {
         environment()->simple_action("$N $vdiscover $ns cannot wear the $o.", this_object());
      }

      return;
   }

   if (test_flag(F_CONCEALED))
   {
      clear_flag(F_CONCEALED);
      environment()->simple_action("$N $vpull out " + this_object()->a_short() + " from the folds of $p clothing.\n");
   }

   set_worn(1);
   environment()->simple_action(query_wear_message(), this_object());

   if (body_size < 1) { return; }

   if (environment()->query_body_size() < body_size)
   {
      if (this_object()->query_plural())
      {
         environment()->my_action("They're a little loose, but you manage to wear them.");
      }
      else
      {
         environment()->my_action("It's a little loose, but you manage to wear it.");
      }
   }
   else if (environment()->query_body_size() > body_size)
   {
      if (this_object()->query_plural())
      {
         environment()->my_action("They're a little tight, but you manage to fit them on.");
      }
      else
      {
         environment()->my_action("It's a little tight, but you manage to fit it on.");
      }
   }
}

void do_remove()
{
   if (!slots) { return 0; }

   if (test_flag(F_WORN))
   {
      environment()->remove_item(this_object());

      set_worn(0);
      environment()->simple_action(query_remove_message(), this_object());
   }
}

void add_required_learned_skill(string skill)
{
   if (strlen(skill))
   {
      required_learned_skills += ({ skill });
   }
}

void remove_required_learned_skill(string skill)
{
   if (member_array(skill, required_learned_skills) > -1)
   {
      required_learned_skills -= ({ skill });
   }
}

string array query_required_learned_skills()
{
   return required_learned_skills;
}

int check_required_learned_skills()
{
   object who = owner(this_object());

   foreach (string skill in required_learned_skills)
   {
      if (wizardp(who))
      {
         tell(who, "(Need to have learned " + skill + ")\n");
      }

      if (!who->has_learned_skill(skill))
      {
         return 0;
      }
   }

   return 1;
}

void add_required_skill(mixed skills ...)
{
   if (arrayp(skills) && (sizeof(skills) == 2))
   {
      required_skills[skills[0]] = skills[1];
   }
}

void remove_required_skill(string skill)
{
   if (required_skills[skill])
   {
      map_delete(required_skills, skill);
   }
}

mapping query_required_skills()
{
   return required_skills;
}

int check_required_skills()
{
   object who = owner(this_object());

   foreach (string skill in keys(required_skills))
   {
      if (wizardp(who))
      {
         tell(who, "(Skill in " + skill + " is " + who->query_skill(skill) + " and needs to be at least " + required_skills[skill] + ")\n");
      }

      if (who->query_skill(skill) < required_skills[skill])
      {
         return 0;
      }
   }

   return 1;
}

void add_required_guild(mixed guild ...)
{
   if (arrayp(guild) && (sizeof(guild) == 2))
   {
      required_guilds[guild[0]] = guild[1];
   }
}

void remove_required_guild(string guild)
{
   if (required_guilds[guild])
   {
      map_delete(required_guilds, guild);
   }
}

mapping query_required_guilds()
{
   return required_guilds;
}

int check_required_guilds()
{
   object who = owner(this_object());

   foreach (string guild in keys(required_guilds))
   {
      if (wizardp(who))
      {
         tell(who, "(Need to be at least level " + required_guilds[guild] + " in " + guild + " guild)\n");
      }

      if (who->query_guild_level(guild) < required_guilds[guild])
      {
         return 0;
      }
   }

   return 1;
}

mixed direct_wear_obj()
{
   object who = owner(this_object());

   if (who != this_body()) { return 0; }

   foreach (string body_slot in slots)
   {
      if (sizeof(this_body()->query_armor_slot(body_slot)) > 1)
      {
         write("You're already wearing something that covers your " + body_slot + ".\n");

         return 0;
      }
   }

   if (test_flag(F_WORN))
   {
      write("You're already wearing that.\n");

      return 0;
   }

   return 1;
}

mixed direct_remove_obj()
{
   object who = owner(this_object());

   if (who != this_body()) { return 0; }

   if (environment() != this_body() || !test_flag(F_WORN))
   {
      write("You're not wearing that.\n");

      return 0;
   }

   return 1;
}

void on_wear()
{
}

void on_remove()
{
}