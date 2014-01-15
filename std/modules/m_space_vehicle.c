inherit "/std/classes/move";

void add_room(string room);
void convert_exits();
void set_inside_leave_message(string message);
void set_outside_leave_message(string message);
void set_inside_arrive_message(string message);
void set_outside_arrive_message(string message);
void launch_vehicle();
void land_vehicle();
void leave_message();
void arrival_message();
string format_message(string message);

string array room_files = ({ });
object array vehicle_rooms = ({ });
string inside_leave_message = "%s lifts off.\n";
string outside_leave_message = "%s lifts off.\n";
string inside_arrive_message = "%s lands at %l.\n";
string outside_arrive_message = "%s settles down on the ground.\n";
object this_object = this_object();

void launch_vehicle()
{
   leave_message();
}

void land_vehicle()
{
   arrival_message();
}

void add_room(string room)
{
   object cloned_room = new(room);
   room_files += ({ room });
   vehicle_rooms += ({ cloned_room });

   cloned_room->move(this_object);
   cloned_room->set_this_vehicle(this_object);

   if (sizeof(vehicle_rooms) == 1)
   {
      remove_call_out(find_call_out("convert_exits"));
      call_out("convert_exits", 3);
   }
}

object find_room(string room)
{
   object array matches = filter_array(all_inventory(this_object), (: depath(base_name($1)) == $(room) :));

   if (sizeof(matches))
   {
      return matches[0];
   }

   return 0;
}

void convert_exits()
{
   foreach (object room in vehicle_rooms)
   {
      mapping exits = room->debug_exits();

      foreach (string exit_name in keys(exits))
      {
         if (stringp(exits[exit_name]->destination))
         {
            object cloned_room_object = find_room(exits[exit_name]->destination);

            if (cloned_room_object)
            {
//               room->delete_exit(exit);
//               room->add_exit(exit, vehicle_rooms[file_number]);
               room->add_exit(exit_name + "obj", cloned_room_object);
               room->add_exit(exit_name + "str", file_name(cloned_room_object));
            }
         }
      }
   }
}

mixed direct_enter_obj() { return 1; }
mixed receive_object(object target, string relation) { return 1; }

void do_verb_rule(string enter, string object_name, object vehicle)
{
   object this_body = this_body();

   this_body->simple_action("$N $venter the $o.", vehicle);
   this_body->move(file_name(vehicle_rooms[0]));
   this_body->force_look(1);
}

void leave_message()
{
   tell_from_outside(vehicle_rooms, format_message(inside_leave_message));
   tell_environment(this_object, format_message(outside_leave_message));
}

void arrival_message()
{
   tell_from_outside(vehicle_rooms, format_message(inside_arrive_message));
   tell_environment(this_object, format_message(outside_arrive_message));
}

void set_inside_leave_message(string message)
{
   inside_leave_message = message + "\n";
}

void set_outside_leave_message(string message)
{
   outside_leave_message = message + "\n";
}

void set_inside_arrive_message(string message)
{
   inside_arrive_message = message + "\n";
}

void set_outside_arrive_message(string message)
{
   outside_arrive_message = message + "\n";
}

string format_message(string message)
{
   message = replace_string(message, "%s", this_object->short());
   message = replace_string(message, "%l", environment(this_object)->short());

   return message;
}