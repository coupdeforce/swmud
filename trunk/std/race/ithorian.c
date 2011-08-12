inherit RACE;

string race_name = "ithorian";
int body_size = 4;
private nosave object array unjustified_ithorian_targets = ({ });

// This is the name of the race, used in the initial selection screen, finger info, etc.
string query_race()
{
   return race_name;
}

// This is the body size of the race, from 1 to 5
int query_body_size()
{
   return body_size;
}

// This is the help description
string short_description()
{
   return "Blah blah Ithorians blah blah.\n";
}

void add_unjustified_ithorian_target(object target)
{
   if (member_array(target, unjustified_ithorian_targets) == -1)
   {
      unjustified_ithorian_targets += ({ target });
   }
}

void remove_unjustified_ithorian_target(object target)
{
   if (member_array(target, unjustified_ithorian_targets) == -1)
   {
      unjustified_ithorian_targets -= ({ target });
   }
}

void reset_unjustified_ithorian_targets()
{
   unjustified_ithorian_targets = ({ });
}

object array query_unjustified_ithorian_targets()
{
   return unjustified_ithorian_targets;
}

int is_unjustified_ithorian_target(object target)
{
   if (member_array(target, unjustified_ithorian_targets) > -1)
   {
      return 1;
   }

   return 0;
}

void check_unjustified_ithorian_targets()
{
   foreach (object target in unjustified_ithorian_targets)
   {
      if (!target)
      {
         unjustified_ithorian_targets -= ({ target });
      }
   }
}

int query_race_str_min() { return 35; }
int query_race_str_max() { return 85; }
int query_race_con_min() { return 40; }
int query_race_con_max() { return 90; }
int query_race_agi_min() { return 5; }
int query_race_agi_max() { return 80; }
int query_race_dex_min() { return 5; }
int query_race_dex_max() { return 80; }
int query_race_per_min() { return 40; }
int query_race_per_max() { return 90; }
int query_race_int_min() { return 45; }
int query_race_int_max() { return 95; }
int query_race_cha_min() { return 40; }
int query_race_cha_max() { return 90; }
int query_race_luc_min() { return 40; }
int query_race_luc_max() { return 90; }
int query_race_for_min() { return 50; }
int query_race_for_max() { return 100; }