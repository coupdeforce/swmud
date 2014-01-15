inherit OBJ;

void start_shuttle();
void add_destination(mixed room, int fare, int waiting_time, int travel_time);
void add_notify_room(mixed room);
void set_cost_currency(string currency);
void set_entry_room(mixed room);
void set_inside_leave_message(string message);
void set_outside_leave_message(string message);
void set_inside_arrive_message(string message);
void set_outside_arrive_message(string message);
void moving();
void waiting();
void leave_message();
void arrival_message();
string format_message(string message);

string array destinations = ({ });
object array notify_rooms = ({ });
string layover = "/domains/common/shuttles";
string inside_leave_message = "The exit closes and %s lifts off.\n";
string outside_leave_message = "%s lifts off.\n";
string inside_arrive_message = "%s lands at %l and the exit opens.\n";
string outside_arrive_message = "%s settles down on the ground.\n";
int array costs = ({ });
string cost_currency = "credits";
int array move_delays = ({ });
int array stop_delays = ({ });
int current_stop = 0;
object this_object = this_object();
object main_room;

void start_shuttle()
{
   if ((sizeof(destinations) > 1) && main_room && environment(this_object))
   {
      main_room->add_exit("out", file_name(environment(this_object)));
      call_out("waiting", stop_delays[current_stop]);
   }
   else
   {
      call_out("start_shuttle", 5);
   }
}

void moving()
{
   this_object->move(destinations[current_stop]);
   main_room->add_exit("out", destinations[current_stop]);

   arrival_message();

   call_out("waiting", stop_delays[current_stop]);
}

void waiting()
{
   main_room->delete_exit("out");

   leave_message();

   this_object->move(layover);

   call_out("moving", move_delays[current_stop]);

   current_stop < (sizeof(destinations) - 1) ? current_stop++ : current_stop = 0;
}

void add_destination(mixed room, int fare, int waiting_time, int travel_time)
{
   if ((fare >= 0) && (travel_time >= 0) && (waiting_time >= 0))
   {
      if (stringp(room) && load_object(room))
      {
         destinations += ({ room });
      }
      else if (objectp(room))
      {
         destinations += ({ file_name(room) });
      }
      else { return; }

      costs += ({ fare });
      stop_delays += ({ waiting_time });
      move_delays += ({ travel_time });
   }
}

void add_notify_room(mixed room)
{
   if (stringp(room) && find_object(room))
   {
      notify_rooms += ({ find_object(room) });
   }
   else if (objectp(room))
   {
      notify_rooms += ({ room });
   }
}

void set_cost_currency(string currency)
{
   if (strlen(currency) && MONEY_D->is_currency(currency)) { cost_currency = currency; }
}

void set_entry_room(mixed room)
{
   if (stringp(room))
   {
      main_room = load_object(room);
   }
   else if (objectp(room))
   {
      main_room = room;
   }
}

mixed direct_enter_obj()
{
   if (sizeof(destinations) > 1) { return 1; }
}

void do_verb_rule(string enter, string object_name, object shuttle)
{
   object this_body = this_body();

   int cost = costs[current_stop];

   if (cost)
   {
      if (this_body->query_amt_money(cost_currency) >= cost)
      {
         this_body->subtract_money(cost_currency, costs[current_stop]);

         this_body->my_action("$N $vpay " + cost + " " + cost_currency + " and enter the $o.", shuttle);
         this_body->other_action("$N $venter the $o.", shuttle);
      }
      else
      {
         tell(this_body, "You realize you don't have " + cost + " " + cost_currency + ".");
         return;
      }
   }
   else
   {
      this_body->simple_action("$N $venter the $o.", shuttle);
   }

   this_body->move(main_room);
   this_body->other_action("$N $venter the $o.", shuttle);
   this_body->force_look(1);
}

void leave_message()
{
   tell_from_outside((({ main_room }) + notify_rooms), format_message(inside_leave_message));
   tell_environment(this_object, format_message(outside_leave_message));
}

void arrival_message()
{
   tell_from_outside((({ main_room }) + notify_rooms), format_message(inside_arrive_message));
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
   message = replace_string(message, "%l", find_object(destinations[current_stop])->short());

   if (strsrch(message, "%d") != -1)
   {
      object destination = find_object(destinations[(current_stop < (sizeof(destinations) - 1) ? (current_stop + 1) : 0)]);

      if (destination)
      {
         message = replace_string(message, "%d", destination->short());
      }
   }

   return message;
}