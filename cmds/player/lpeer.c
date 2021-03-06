// Last modified by deforce on 07-05-2008
#include <mudlib.h>
inherit CMD;
inherit M_EXIT;

private mapping translations = ([ "n" : "north", "s" : "south", "e" : "east", "w" : "west", "ne" : "northeast", "nw" : "northwest", "se" : "southeast", "sw" : "southwest", "u" : "up", "d" : "down" ]);

private void main(string direction)
{
   if (direction)
   {
      object room;

      if (translations[direction]) { direction = translations[direction]; }

      room = environment(this_body());

      if (room->query_exit_check(direction) && (member_array(direction, environment(this_body())->query_exit_directions(wizardp(this_body()) > 0 ? 1 : 0)) > -1))
      {
         object dir_room = load_object(room->query_exit_destination(direction));
         int room_count = 1;
         string array exits = explode(dir_room->show_exits(), ", ");

         out("You look " + direction + " and see:\n\n");

         if (dir_room->query_light() < 1)
         {
            out("It is too dark to see anything.\n");
         }
         else if (dir_room->query_light() > 9)
         {
            out("It is too bright to see anything.\n");
         }
         else
         {
            out("%^ROOM_SHORT%^" + dir_room->short() + "%^RESET%^\n");

            if (sizeof(exits)) { out("[" + sizeof(exits) + ": %^ROOM_EXIT%^" + implode(sort_array(exits, 1), "%^RESET%^, %^ROOM_EXIT%^") + "%^RESET%^]"); }
            else { out("[%^ROOM_EXIT%^no exits%^RESET%^]"); }

            out("\n" + dir_room->show_objects());

            while ((member_array(direction, exits) > -1) && (wizardp(this_user()) || (room_count < 5)))
            {
               dir_room = load_object(dir_room->query_exit_destination(direction));

               out("\n%^ROOM_SHORT%^" + dir_room->short() + "%^RESET%^\n");

               if (dir_room->query_exit_check(direction) && (member_array(direction, environment(this_body())->query_exit_directions(wizardp(this_body()) > 0 ? 1 : 0)) > -1))
               {
                  exits = explode(dir_room->show_exits(), ", ");
               }
               else
               {
                  exits = ({ });
               }

               if (sizeof(exits)) { out("[" + sizeof(exits) + ": %^ROOM_EXIT%^" + implode(sort_array(exits, 1), "%^RESET%^, %^ROOM_EXIT%^") + "%^RESET%^]"); }
               else { out("[%^ROOM_EXIT%^no exits%^RESET%^]"); }

               out("\n" + dir_room->show_objects());

               room_count++;
            }
         }
      }
      else { out("You don't see anything in that direction.\n"); }
   }
   else { out("Usage: lpeer <direction>\n"); }
}