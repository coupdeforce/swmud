// Last edited by deforce on 10-19-2009
// Must be cloned with new() instead of it using the master object, for multiple hooks to work correctly
inherit "/std/buff";

function my_hook;
string stat_name;
int stat_bonus;

void create(string stat, int bonus, int duration)
{
   buff::create();

   stat_name = stat;
   stat_bonus = bonus;

   set_buff_duration(duration);
}

int query_stat_bonus()
{
   return stat_bonus;
}

void apply_buff(object target)
{
   my_hook = (: query_stat_bonus :);
   target->add_hook(stat_name + "_bonus", my_hook);
   target->refresh_stats();
}

void remove_buff(object target)
{
   target->remove_hook(stat_name + "_bonus", my_hook);
   target->refresh_stats();
}