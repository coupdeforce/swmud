#include <mudlib.h>
#include <setbit.h>

inherit M_GRAMMAR;

string get_extra_description();

// Our descriptions:
// long:   should be a complete sentence or a closure.
// in_room_desc: A longer string for rooms.  Titles for players.
// plural_in_room_desc: Message for more than 1.
// If these are zero, sane defaults are used based on 'names'.
private mixed long;
private mapping customize_record = ([ ]);
private mapping customize_times = ([ ]);
private int object_creation_time = 0;
private nosave mixed in_room_desc;
private nosave string plural_in_room_desc;
private nosave string listen_desc;
private nosave string smell_desc;
private nosave string feel_desc;
private nosave string force_desc;

// these are somewhere else...
int test_flag(int);
int is_visible();
int is_attached();
string the_short();
string short();
string plural_short();
string a_short();
string query_title();
string sex_race();
string get_health_description();
string team_description(int alignment);
string health_description(int percent);
string size_description(int size);
string customize_description(string type, string name);
varargs mixed call_hooks();

// This one is here, but this is a forward declaration.
string query_in_room_desc();

//:FUNCTION set_long
// Set the long description of an object
void set_long(mixed str)
{
   long = str;
   if (functionp(long)) { return; }
//   if ((long == "") || (long[<1] != '\n')) { long += "\n"; }
}

//:FUNCTION get_base_long
// Get the variable long, not the full description...
string get_base_long()
{
   string res;

   //if(!is_visible())
   //  return "Funny, you don't see anything at all.";

   res = evaluate(long);
   if (!res) { return "You see nothing special about " + the_short() + ".\n"; }

   if (strlen(get_extra_description()))
   {
      if (strlen(res))
      {
         res += "  ";
      }

      res += get_extra_description();
   }

   if (strlen(res))
   {
      return res + "\n";
   }

   return res;
}

string get_extra_long()
{
//:HOOK extra_long
//The returned strings are added on to the end of the long description.
   return call_hooks("extra_long", (: $1 + $2 :), "") || "";
}

string get_desc_properties()
{
   string output = "";

   if (!this_object()->is_body())
   {
      if (this_object()->query_body_size() > 0)
      {
         output += "\nIt is " + size_description(this_object()->query_body_size()) + " sized.";
      }

      if (test_flag(F_ACID_EXPOSURE))
      {
         if (this_object()->is_damaged_by_acid())
         {
            output += "\n%^YELLOW%^It is weakened by acid.%^RESET%^";
         }
         else
         {
            output += "\n%^YELLOW%^It has been exposed to acid.%^RESET%^";
         }
      }

      if (test_flag(F_BROKEN))
      {
         output += "\nIt is broken.";
      }
      else if (test_flag(F_DAMAGED))
      {
         output += "\nIt is damaged.";
      }

      foreach (string type in keys(customize_record))
      {
         output += "\n" + customize_description(type, customize_record[type]);
      }

      if (this_object()->is_autoload())
      {
         output += "\nIt is autoloading.";
      }
   }

   if (this_user() && wizardp(this_user()))
   {
      if (sizeof(ANNOTATION_D->retrieve_annotations(base_name(this_object()))))
      {
         output += "\n%^YELLOW%^There is a 'discuss' note.\n%^RESET%^";
      }
/*
      if (!this_object()->is_body() && object_creation_time)
      {
         output += "\nIt was created " + convert_time(time() - object_creation_time) + " ago.";
      }
*/
   }

   return output;
}

// This can be overloaded in an object, and is added directly after the long description.
string get_extra_description()
{
   return "";
}

//:FUNCTION long
//Return the verbose description of an object that you see when you look at it.
string long()
{
   return get_base_long() + get_health_description() + get_extra_long() + get_desc_properties();
}

protected string array discarded_message, plural_discarded_message;

void set_customize_record(string record, string name)
{
   if (strlen(record) && strlen(name))
   {
      customize_record[record] = name;
      customize_times[record] = time();
   }
}

string query_customize_record(string record)
{
   if (strlen(record))
   {
      return customize_record[record];
   }
}

int query_customize_time(string record)
{
   if (strlen(record))
   {
      return customize_times[record];
   }
}

mapping query_customize_records() { return customize_record; }

mapping query_customize_times() { return customize_times; }

void remove_customize_record(string record)
{
   map_delete(customize_record, record);
   map_delete(customize_times, record);
}

varargs void set_object_creation_time(int time)
{
   if (time)
   {
      object_creation_time = time;
   }
   else
   {
      object_creation_time = time();
   }
}

int query_object_creation_time()
{
   return object_creation_time;
}

void set_listen(string str) { listen_desc = str; }

string query_listen() { return listen_desc; }

void set_smell(string str) { smell_desc = str; }

string query_smell() { return smell_desc; }

void set_feel(string str) { feel_desc = str; }

string query_feel() { return feel_desc; }

void set_force(string str) { force_desc = str; }

string query_force() { return force_desc; }

void do_listen()
{
   if (listen_desc)
   {
      write(listen_desc);
   }
   else
   {
      write("You hear nothing unusual.\n");
   }
}

void do_smell()
{
   if (smell_desc)
   {
      write(smell_desc);
   }
   else
   {
      write("You smell nothing unusual.\n");
   }
}

void do_feel()
{
   if (feel_desc)
   {
      write(feel_desc);
   }
   else
   {
      write("You feel nothing unusual.\n");
   }
}

void do_force()
{
   if (force_desc)
   {
      write(force_desc);
   }
   else
   {
      write("You sense nothing unusual.\n");
   }
}

//:FUNCTION show_in_room
//Return a string appropriate for inclusing in a room long description.
//Note that duplicatep() objects return nothing.
string show_in_room()
{
   string str;
   int our_count;
   object link;

   if (is_attached()) { return 0; }

   our_count = count();

   if (our_count > 1 )
   {
      if ((link = this_object()->query_link()) && userp(link) && (str = query_title())) { return capitalize(number_word(our_count)) + " " + pluralize(str); }

      if (plural_in_room_desc) { return capitalize(number_word(our_count)) + " " + plural_in_room_desc; }

      str = short();

      if (!str) { return 0; }

      return capitalize(number_word(our_count)) + " " + plural_short();
   }

   if ((link = this_object()->query_link()) && userp(link) && (str = query_title())) { return str; }

   if (str = query_in_room_desc()) { return str; }

   str = short();

   if (!str) { return 0; }

   return capitalize(str);
}

//:FUNCTION set_in_room_desc
//Set the description an object has when it is sitting in a room
void set_in_room_desc(string arg)
{
   in_room_desc = arg;
}

void set_plural_in_room_desc(string arg) { plural_in_room_desc = arg; }

string query_possessive() { return "its"; }

int query_body_size() { return 0; }

string query_in_room_desc()
{
   return (string)evaluate(in_room_desc);
}

string team_health()
{
   object this_object = this_object();

   if (this_object->is_living() && !wizardp(this_object))
   {
      string output = capitalize(this_object->query_pronoun()) + " " + team_description(this_object->query_team_alignment()) + ".\n";

      if (this_object->is_adversary())
      {      
         output += capitalize(this_object->query_pronoun()) + " is " + health_description((this_object->query_health() * 100) / this_object->query_max_health()) + ".\n";
      }

      return output;
   }

   return "";
}

private string get_health_description()
{
   object this_object = this_object();

   if (this_object->is_living() && this_object->is_adversary())
   {
      return capitalize(this_object->query_pronoun()) + " is " + health_description((this_object->query_health() * 100) / this_object->query_max_health()) + ".";
   }

   return "";
}

string team_description(int alignment)
{
   switch (alignment)
   {
      case 100: return "is a hero of the Rebel Alliance";
      case 86..99: return "is dedicated to the Rebel Alliance";
      case 71..85: return "has very strong ties with the Rebel Alliance";
      case 56..70: return "has strong ties with the Rebel Alliance";
      case 41..55: return "is associated with the Rebel Alliance";
      case 26..40: return "is linked to the Rebel Alliance";
      case 21..25: return "is strongly influenced by the Rebel Alliance";
      case 16..20: return "is somewhat influenced by the Rebel Alliance";
      case 1..15: return "has no significant political affiliation";
      case 0: return "is committed to neutrality";
      case -15..-1: return "has no significant political affiliation";
      case -20..-16: return "is somewhat influenced by the Empire";
      case -25..-21: return "is strongly influenced by the Empire";
      case -40..-26: return "is linked to the Empire";
      case -55..-41: return "is associated with the Empire";
      case -70..-56: return "has strong ties with the Empire";
      case -85..-71: return "has very strong ties with the Empire";
      case -99..-86: return "is loyal to the Empire";
      case -100: return "is a hero of the Empire";
   }

   return "is in an odd state of team alignment";
}

string health_description(int percent)
{
   switch (percent > 100 ? 100 : percent)
   {
      case 91..100: return "in top shape";
      case 76..90: return "in decent shape";
      case 61..75: return "slightly injured";
      case 46..60: return "hurting";
      case 31..45: return "badly injured";
      case 16..30: return "terribly injured";
      case 0..15: return "near death";
   }

   return "in an odd state of health";
}

string size_description(int size)
{
   switch (size)
   {
      case 1: return "very small";
      case 2: return "small";
      case 3: return "medium";
      case 4: return "large";
      case 5: return "very large";
   }

   return "odd";
}

string format_customize_name(string name)
{
   return "%^BOLD%^" + capitalize(name) + "%^RESET%^";
}

string customize_description(string type, string name)
{
   switch (type)
   {
      case "adjusted": return "It has been adjusted by " + format_customize_name(name) + ".";
      case "altered weight": return "Its weight has been reduced by " + format_customize_name(name) + ".";
      case "altered value": return "Its value has been increased by " + format_customize_name(name) + ".";
      case "altered capacity": return "Its capacity has been increased by " + format_customize_name(name) + ".";
      case "constructed": return "It was constructed by " + format_customize_name(name) + ".";
      case "manufactured": return "It was manufactured by " + format_customize_name(name) + ".";
   }

   return "";
}