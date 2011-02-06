// Last edited by deforce on 07-04-2008
#include <flags.h>
#include <playerflags.h>

private nosave string remote_desc;

int query_light();
string short();
string show_exits();
string long();
void test_flag(int);

private nosave int this_look_is_forced;

//:FUNCTION show_objects
//Return a string describing the objects in the room
varargs string show_objects(object except)
{
   object *obs;
   string array user_show = ({ });
   string array obj_show = ({ });
   string str;
   object link;

   obs = filter(all_inventory() - ({ this_body() }), (: $1->is_visible() :));

   if (except)
   {
      obs -= ({ except });
   }

   if (!sizeof(obs)) { return ""; }

   foreach (object thing in obs)
   {
      if (thing->is_living() && !thing->test_flag(F_HIDDEN))
      {
         if (!duplicatep(thing))
         {
            user_show += ({ thing->show_in_room() });
         }
      }
      else if (!thing->test_flag(F_HIDDEN))
      {
         if (!duplicatep(thing))
         {
            if ((str = thing->show_in_room()) && strlen(str))
            {
               if (except && !except->is_body())
               { 
                  str += sprintf(" (outside %s)", except->the_short());
               }

               obj_show += ({ str + thing->get_attributes() });
            }

            if (thing->inventory_visible() && !thing->query_hide_contents())
            {
               obj_show += ({ thing->show_contents() });
            }
         }
      }
   }

   if (except) // We're inside an object
   {
      obj_show += ({ except->inventory_recurse(0, this_body()) });
   }

   return (sizeof(user_show) ? "\n%^ROOM_LIVINGS%^" + implode(sort_array(user_show, 1), "\n") + "%^RESET%^\n" : "") + (sizeof(obj_show) ? "\n%^ROOM_OBJECTS%^" + implode(sort_array(obj_show, 1), ", ") + "%^RESET%^\n" : "");
}

protected int dont_show_long()
{
   return !this_look_is_forced && this_body()->test_flag(F_BRIEF);
}

//:FUNCTION do_looking
//print out the description of the current room
varargs void do_looking(int force_long_desc, object who)
{
   if (query_light() < 1)
   {
      tell(who, "It is too dark to see anything.\n");
   }
   else if (query_light() > 9)
   {
      tell(who, "It is too bright to see anything.\n");
   }
   else
   {
      if (force_long_desc || !who->test_flag(F_BRIEF))
      {
         this_look_is_forced = force_long_desc;
         tell(who, long());
         this_look_is_forced = 0;
      }
      else
      {
         tell(who, sprintf("%%^ROOM_SHORT%%^%s%%^RESET%%^\n[%%^ROOM_EXIT%%^%s%%^RESET%%^]\n", short(), show_exits()));
      }
   }
}

// This should be overloaded if you want to be able to give different descs from different rooms
void remote_look(object o)
{
   if (remote_desc)
   {
      printf("%s\n", remote_desc);
   }
   else
   {
      printf("You can't seem to make out anything.\n");
   }
}

void set_remote_desc(string s)
{
   remote_desc = s;
}