/* ??-Aug-94 Created.  By Beek.  Originally was /std/weapon.
** 12-Dec-94 Converted to modules approach.  Deathblade.
** 22-Oct-95 Beek separated out the combat stuff into M_DAMAGE_SOURCE, since
**           we don't want living objects to have the verb interaction
** 961216, Deathblade: added related skill for the weapon.
** ??-Apr-98 Iizuka updated to support the new adversary code. Limb
**           wielding is now available, and inventory description code has
**           been updated in respect to this.
*/

#include <flags.h>

void unwield_me();
void hook_state(string, mixed, int);
void add_save(string array);
void assign_flag(int which, int state);
void test_flag(int);
void clear_flag(int);

private nosave object wielded_by;
private nosave function move_hook = (: unwield_me :);
private string array wielding_limbs = ({ });
private string array required_learned_skills = ({ });
private mapping required_skills = ([ ]);
private mapping required_guilds = ([ ]);
private string wield_message = "$N $vwield a $o.";
private string unwield_message = "$N $vunwield a $o.";
private mapping attribute_modifiers = ([ ]);
private mapping attribute_bonuses = ([ ]);
private nosave mapping attribute_hooks = ([ ]);
private mapping skill_bonuses = ([ ]);
private nosave mapping skill_hooks = ([ ]);
private int heal_bonus = 0;
private nosave function heal_hook;
private int armor_bonus = 0;
private int parry_bonus = 0;
private nosave function parry_hook;

void mudlib_setup()
{
   add_save(({ "wielding_limbs", "required_learned_skills", "required_skills", "required_guilds", "wield_message", "unwield_message", "attribute_modifiers", "attribute_bonuses", "skill_bonuses", "heal_bonus", "armor_bonus", "parry_bonus", "persist_flags" }));
}

int valid_wield() // Return 1 if they can wield this.
{
   return 1;
}

int valid_unwield() // Return 1 if they can unwield this.
{
   return 1;
}

void set_wield_message(string message)
{
   wield_message = message;
}

void set_unwield_message(string message)
{
   unwield_message = message;
}

string query_wield_message()
{
   return wield_message + "\n";
}

string query_unwield_message()
{
   return unwield_message + "\n";
}

string array query_wielding()
{
   return wielding_limbs;
}

object query_wielded_by()
{
   return wielded_by;
}

string wielded_attributes()
{
   return this_body()->get_wield_attributes();
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

void set_parry_bonus(int amount)
{
   parry_bonus = amount;
}

void add_parry_bonus(int amount)
{
   parry_bonus += amount;
}

void reset_parry_bonus()
{
   parry_bonus = 0;
}

int query_parry_bonus()
{
   return parry_bonus;
}

int get_armor_bonus()
{
   return armor_bonus;
}

void set_armor_bonus(int amount)
{
   armor_bonus = amount;
}

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

varargs void mark_wielded_by(object which, string array limbs...)
{
   string array affected_attributes = keys(this_object()->query_attribute_modifiers()) - keys(attribute_bonuses) + keys(attribute_bonuses);

   wielded_by = which;
   wielding_limbs = limbs;

   if (which)
   {
      wielded_by->mark_wielding_limbs(limbs);

      heal_hook = (: query_heal_bonus :);
      owner(this_object())->add_hook("heal_rate_bonus", heal_hook);
      parry_hook = (: query_parry_bonus :);
      owner(this_object())->add_hook("parry_bonus", parry_hook);
      owner(this_object())->add_armor_bonus(armor_bonus);
   }
   else
   {
      owner(this_object())->remove_hook("heal_rate_bonus", heal_hook);
      owner(this_object())->remove_hook("parry_bonus", parry_hook);
      owner(this_object())->add_armor_bonus(armor_bonus * -1);
   }

   assign_flag(F_WIELDED, which && (which != this_object()));
   hook_state("move", move_hook, which && (which != this_object()));

   foreach (string type in affected_attributes)
   {
      if (which)
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
      if (which)
      {
         skill_hooks[skill] = (: get_skill_bonus, skill :);
         owner(this_object())->add_hook(skill + "_bonus", skill_hooks[skill]);
      }
      else
      {
         owner(this_object())->remove_hook(skill + "_bonus", skill_hooks[skill]);
      }
   }

   owner(this_object())->refresh_stats();
}

mixed ob_state()
{
   return wielded_by ? wielded_attributes() : 0;
}

void unwield_me()
{
   if (wielded_by)
   {
      wielded_by->do_unwield(this_object());
      wielded_by = 0;
      clear_flag(F_WIELDED);
   }
}

void do_unwield()
{
   unwield_me();
}

void unwield()
{
   unwield_me();
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

mixed direct_wield_obj()
{
   object who = owner(this_object());

   if (who && who != this_body()) { return 0; }

   if (test_flag(F_BROKEN))
   {
      tell(who, "It is too broken to wield.\n");
      return 0;
   }

   if (wielded_by != 0)
   {
      tell(who, "You're already wielding that!\n");
      return 0;
   }

   return 1; /* Fall through */
}

mixed direct_wield_obj_in_str(object ob, string limb)
{
   object who = owner(this_object());

   if (who && who != this_body()) { return 0; }

   if (member_array(limb, who->query_wielding_limbs()) == -1) { return 0; }

   if (test_flag(F_BROKEN))
   {
      tell(who, "It is too broken to wield.\n");
      return 0;
   }

   if (!who->query_health(limb))
   {
      tell(who, sprintf("Your %s is in no condition to wield that!\n", limb));
      return 0;
   }

   return 1;
}

mixed direct_unwield_obj()
{
   object who = owner(this_object());

   if (!query_wielded_by()) { return "You are not wielding that.\n"; }

   if(who && who != this_body()) { return 0; }

   return 1;
}