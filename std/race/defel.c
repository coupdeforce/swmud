inherit RACE;

string race_name = "defel";
int body_size = 3;

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

int can_see_in_uv_light() { return 1; }

// This is the help description
string short_description()
{
   return "Blah blah Defels blah blah.\n";
}

int query_race_str_min() { return 40; }
int query_race_str_max() { return 90; }
int query_race_con_min() { return 50; }
int query_race_con_max() { return 100; }
int query_race_agi_min() { return 50; }
int query_race_agi_max() { return 100; }
int query_race_dex_min() { return 40; }
int query_race_dex_max() { return 90; }
int query_race_per_min() { return 35; }
int query_race_per_max() { return 85; }
int query_race_int_min() { return 35; }
int query_race_int_max() { return 85; }
int query_race_cha_min() { return 5; }
int query_race_cha_max() { return 80; }
int query_race_luc_min() { return 40; }
int query_race_luc_max() { return 90; }
int query_race_for_min() { return 5; }
int query_race_for_max() { return 80; }