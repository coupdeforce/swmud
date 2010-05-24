// Last edited by deforce on 03-23-2010
// Must be cloned with new() instead of it using the master object, for multiple hooks to work correctly
inherit "/std/buff";

int query_to_hit();
int query_skill();

function to_hit_hook;
function skill_hook;
int to_hit_adjustment;
int skill_adjustment;
string array weapon_skill_names;
int damage_adjustment;

void create(int to_hit, int skill, string array weapon_skills, int damage, int duration)
{
   buff::create();

   to_hit_hook = (: query_to_hit() :);
   skill_hook = (: query_skill() :);

   to_hit_adjustment = to_hit;
   skill_adjustment = skill;
   weapon_skill_names = weapon_skills;
   damage_adjustment = damage;

   set_buff_duration(duration);
}

int query_to_hit()
{
   return to_hit_adjustment;
}

int query_skill()
{
   return skill_adjustment;
}

void apply_buff(object target)
{
   target->add_hook("to_hit_bonus", to_hit_hook);
   target->add_hook("all_skill_bonus", skill_hook);
   target->add_damage_bonus(damage_adjustment);

   foreach (string name in weapon_skill_names)
   {
      target->add_hook(name + "_skill_bonus", skill_hook);
   }

   tell(target, "%^BOLD%^" + (damage_adjustment < 0 ? "%^RED%^" : "%^GREEN%^") + "You feel " + (damage_adjustment < 0 ? "negatively" : "positively") + " affected by battle meditation.%^RESET%^\n");
}

void remove_buff(object target)
{
   target->remove_hook("to_hit_bonus", to_hit_hook);
   target->remove_hook("all_skill_bonus", skill_hook);
   target->add_damage_bonus(damage_adjustment * -1);

   foreach (string name in weapon_skill_names)
   {
      target->remove_hook(name + "_skill_bonus", skill_hook);
   }

   tell(target, "%^BOLD%^You no longer feel " + (damage_adjustment < 0 ? "negatively" : "positively") + " affected by battle meditation.%^RESET%^\n");
}