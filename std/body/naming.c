// Last edited by deforce on 03-22-2010
// naming.c -- functions for naming and description of the player body
// Created by Deathblade 06-03-1996
#include <commands.h>           // for CMD_OB_xxx

private string describe;

int is_visible();
int test_flag(int which);
int query_ghost();
object query_link();
string number_of(int num, string what);
void save_me();
string query_reflexive();
void add_id_no_plural(string array id...);
string in_room_desc();
string living_query_name();
string query_name();
string query_title();

string query_long_name()
{
   if (query_ghost()) { return "The ghost of " + capitalize(living_query_name()); }

   return query_title();
}

nomask string query_userid()
{
   if (!query_link()) { return 0; }

   return query_link()->query_userid();
}

string query_name()
{
   return query_userid();
}

nomask string query_invis_name()
{
   return "Someone";
}

string query_idle_string()
{
   object link = query_link();
   int idle_time;
   string result = "";

   if (interactive(link)) { idle_time = query_idle(link); }

   if (idle_time < 600) { return ""; }

   result += " (" + convert_time(idle_time, 3) + ")";

   return result;
}

// This is used by in_room_desc and by who, one of which truncates,
// one of which doesnt.  Both want an idle time.
string base_in_room_desc()
{
   object link = query_link();

   // If they are link-dead, then prepend something...
   if (!link || !interactive(link)) { return "The lifeless body of " + link->query_userid(); }

   return query_long_name();
}

string query_formatted_desc(int num_chars)
{
   string idle_string;
   int i;

   idle_string = query_idle_string();

   if (i = strlen(idle_string))
   {
      num_chars -= (i + 1);
      idle_string = " " + idle_string;
   }

   return M_ANSI->colour_truncate(base_in_room_desc(), num_chars) + idle_string;
}

void set_description(string str)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s description to " + str, previous_object(-1)); }

   if (base_name(previous_object()) == CMD_OB_DESCRIBE) { describe = str; }

   if (describe == "\n") { describe = ""; }

   save_me();
}

string our_description()
{
   if (describe) { return describe + "\n"; }

   return capitalize(query_name()) + " is boring and hasn't described " + query_reflexive() + ".\n";
}