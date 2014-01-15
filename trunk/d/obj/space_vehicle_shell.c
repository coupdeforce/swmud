// Shell for space vehicles
#include <playerflags.h>

inherit M_INPUT;

#include "shields.h"

object this_vehicle;  // Points to the vehicle object
object target_object; // Points to the last targetted object

object find_space_object(string name);
string get_prompt();
string color_percent(int percent);
void shell_input(string input);
void dock_input(object thing, string input);
void orbit_input(object thing, string input);
void do_launch();
void do_launch_vector(int x, int y, int z);
void do_land(string target);
void do_orbit(string target);
void display_scan();
void display_shields();
void send_radio_message(string message);
void show_players(string thing);
void show_info();
void show_help();

void display_scan()
{
   int x = this_vehicle->get_location_x();
   int y = this_vehicle->get_location_y();
   int z = this_vehicle->get_location_z();
   int dx = this_vehicle->get_direction_x();
   int dy = this_vehicle->get_direction_y();
   int dz = this_vehicle->get_direction_z();

   write("\n%^CYAN%^         Object Description               ID     Dist     X     Y     Z%^RESET%^\n");
   write("%^CYAN%^-------------------------------------- -------- ------  ----- ----- -----%^RESET%^\n");

   foreach (object thing in all_inventory(environment(this_vehicle)))
   {
      if ((thing != this_vehicle) && thing->is_space_object() && thing->is_launched())
      {
         int xd = thing->get_location_x() - x;
         int yd = thing->get_location_y() - y;
         int zd = thing->get_location_z() - z;
         float distance = sqrt((xd * xd) + (yd * yd) + (zd * zd));
         int xv = to_int((xd * 1000) / (distance != 0.0 ? distance : 1));
         int yv = to_int((yd * 1000) / (distance != 0.0  ? distance : 1));
         int zv = to_int((zd * 1000) / (distance != 0.0  ? distance : 1));

         write(sprintf("%-38s %8s %%^BOLD%%^%6d  %5d %5d %5d%%^RESET%%^\n", thing->short(),
            (thing->is_space_target() ? thing->query_unique_id() : "*****"),
            to_int(distance - thing->get_radius()), xv, yv, zv));
//         write(sprintf("%s  %%^BOLD%%^%5d %5d %5d%%^RESET%%^\n", repeat_string(" ", 54),
//            thing->get_location_x(), thing->get_location_y(), thing->get_location_z()));
      }
   }

   write("%^CYAN%^-------------------------------------- -------- ------  ----- ----- -----%^RESET%^\n");
   write(sprintf("%%^CYAN%%^Location:%%^RESET%%^ %-27s  %%^CYAN%%^Current Vector:%%^RESET%%^  %%^BOLD%%^%5d %5d %5d%%^RESET%%^\n\n",
      environment(this_vehicle)->short(), dx, dy, dz));
//   write(sprintf("%71s  %%^BOLD%%^%5d %5d %5d%%^RESET%%^\n\n", "%^CYAN%^Current Point:%^RESET%^",
//      x, y, z));
}

void shell_input(string input)
{
   int velocity;
   int auto_pilot;
   int vector_x;
   int vector_y;
   int vector_z;
   string thing;

   if ((input == "s") || (input == "scan"))
   {
      display_scan();
   }
   else if ((sscanf(input, "vector %d %d %d", vector_x, vector_y, vector_z) == 3)
      || (sscanf(input, "v %d %d %d", vector_x, vector_y, vector_z) == 3))
   {
      if ((vector_x < 10000) && (vector_y < 10000) && (vector_z < 10000))
      {
         float vector_length = sqrt(pow(vector_x, 2) + pow(vector_y, 2) + pow(vector_z, 2));

         vector_x = to_int((vector_x * 1000) / (vector_length != 0.0  ? vector_length : 1));
         vector_y = to_int((vector_y * 1000) / (vector_length != 0.0  ? vector_length : 1));
         vector_z = to_int((vector_z * 1000) / (vector_length != 0.0  ? vector_length : 1));

         this_vehicle->set_direction(vector_x, vector_y, vector_z);
         write(sprintf("Direction changed to: %d %d %d\n", vector_x, vector_y, vector_z));
      }
      else
      {
         write("Units must be less than 10000.\n");
      }
   }
   else if ((input == "vector") || (input == "v"))
   {
      write(sprintf("Current vector: %d %d %d\n", this_vehicle->get_direction_x(),
         this_vehicle->get_direction_y(), this_vehicle->get_direction_z()));
   }
   else if ((sscanf(input, "vector %s", thing) == 1) || (sscanf(input, "v %s", thing) == 1))
   {
      object bleh = find_space_object(thing[0..0] == "-" ? thing[1..] : thing);

      if (bleh)
      {
         int xd = bleh->get_location_x() - this_vehicle->get_location_x();
         int yd = bleh->get_location_y() - this_vehicle->get_location_y();
         int zd = bleh->get_location_z() - this_vehicle->get_location_z();
         float distance = sqrt((xd * xd) + (yd * yd) + (zd * zd));
         int xv = to_int((xd * 1000) / (distance != 0.0  ? distance : 1));
         int yv = to_int((yd * 1000) / (distance != 0.0  ? distance : 1));
         int zv = to_int((zd * 1000) / (distance != 0.0  ? distance : 1));

         if (thing[0..0] == "-")
         {
            this_vehicle->set_direction((xv * -1), (yv * -1), (zv * -1));

            write(sprintf("Direction changed to: %d %d %d\n", (xv * -1), (yv * -1), (zv * -1)));
         }
         else
         {
            this_vehicle->set_direction(xv, yv, zv);

            write(sprintf("Direction changed to: %d %d %d\n", xv, yv, zv));
         }
      }
   }
   else if (sscanf(input, "sp %d %d", velocity, auto_pilot) == 2)
   {
      this_vehicle->adjust_velocity(velocity);
      this_vehicle->set_auto_pilot(auto_pilot);
   }
   else if (sscanf(input, "sp %d", velocity) == 1)
   {
      this_vehicle->adjust_velocity(velocity);
   }
   else if (sscanf(input, "ap %d", auto_pilot) == 1)
   {
      this_vehicle->set_auto_pilot(auto_pilot);
   }
   else if (input == "launch")
   {
      do_launch();
   }
   else if (sscanf(input, "launch %d %d %d", vector_x, vector_y, vector_z) == 3)
   {
      do_launch_vector(vector_x, vector_y, vector_z);
   }
   else if (input == "land")
   {
      write("land <object>\n");
   }
   else if (sscanf(input, "land %s", thing) == 1)
   {
      if (this_vehicle->is_launched())
      {
         do_land(thing);

         return;
      }
      else
      {
         write("This vehicle is already on the ground.\n");
      }
   }
   else if (sscanf(input, "orbit %s", thing) == 1)
   {
      if (this_vehicle->is_launched())
      {
         do_orbit(thing);

         return;
      }
      else
      {
         write("This vehicle can only orbit in space.\n");
      }
   }
   else if ((input == "target") || (input == "t"))
   {
      if (target_object)
      {
         write("Current target: " + target_object->short() + "\n");
      }
      else
      {
         write("There is currently no target.\n");
      }
   }
   else if ((sscanf(input, "target %s", thing) == 1) || (sscanf(input, "t %s", thing) == 1))
   {
      object bleh = find_space_object(thing);

      if (bleh && bleh->is_space_object() && bleh->is_space_target())
      {
         int xd = bleh->get_location_x() - this_vehicle->get_location_x();
         int yd = bleh->get_location_y() - this_vehicle->get_location_y();
         int zd = bleh->get_location_z() - this_vehicle->get_location_z();
         int distance = sqrt((xd * xd) + (yd * yd) + (zd * zd)) - bleh->get_radius();

         if (distance <= 250)
         {
            target_object = bleh;

            write(target_object->short() + " has been targetted.\n");
         }
         else
         {
            write("Target must be within 250 units.\n");
         }
      }
      else if (bleh && bleh->is_space_object() && !bleh->is_space_target())
      {
         write("Targetting that would be pointless.\n");
      }
      else
      {
         write("Target not found.\n");
      }
   }
   else if (sscanf(input, "players %s", thing) == 1)
   {
      show_players(thing);
   }
   else if (sscanf(input, "radio %s", thing) == 1)
   {
      send_radio_message(thing);
   }
   else if (input == "stop")
   {
      this_vehicle->adjust_velocity(this_vehicle->get_velocity() * -1);
      this_vehicle->set_auto_pilot(0);
   }
   else if ((input == "i") || (input == "info"))
   {
      show_info();
   }
   else if ((input == "sh") || (input == "shields"))
   {
      display_shields();
   }
   else if (input == "exit")
   {
      present("console", environment(this_body()))->set_unoccupied();
      this_body()->simple_action("$N $vstand up from the console.");
      this_body()->clear_flag(F_IN_EDIT);
      destruct(this_object());
      return;
   }
   else if (input == "help")
   {
      show_help();
   }
   else
   {
      write("Unrecognized command.\n");
   }

   modal_simple((: shell_input :), (: get_prompt :));
}

void dock_input(object thing, string input)
{
   if (member_array(input, thing->get_all_docks()) != -1)
   {
      string file = thing->get_dock_file(input);

      if (file)
      {
         this_vehicle->set_launched(0);
         this_vehicle->set_location(0, 0, 0);
         this_vehicle->adjust_velocity(this_vehicle->get_velocity() * -1);
         this_vehicle->set_auto_pilot(0);
         present("console", environment(this_body()))->set_unoccupied();
         this_vehicle->move(file);
         this_body()->simple_action("$N $vland the vehicle on the ground.");
         this_body()->simple_action("$N $vstand up from the console.");
         this_body()->clear_flag(F_IN_EDIT);
         destruct(this_object());
         return;
      }
   }

   write("Docking failed.\n");
   modal_simple((: shell_input :), (: get_prompt :));
}

void orbit_input(object thing, string input)
{
   int vector_x;
   int vector_y;
   int vector_z;

   if (sscanf(input, "orbit %d %d %d", vector_x, vector_y, vector_z) == 3)
   {
      if ((vector_x < 10000) && (vector_y < 10000) && (vector_z < 10000))
      {
         int distance = thing->get_radius() + 50 + random(151);
         float xs = pow(vector_x, 2);
         float ys = pow(vector_y, 2);
         float zs = pow(vector_z, 2);
         float vector_length = sqrt(xs + ys + zs);

         vector_x = to_int((vector_x * 1000) / (vector_length != 0.0  ? vector_length : 1));
         vector_y = to_int((vector_y * 1000) / (vector_length != 0.0  ? vector_length : 1));
         vector_z = to_int((vector_z * 1000) / (vector_length != 0.0  ? vector_length : 1));

         this_vehicle->set_location((vector_x / 1000.0) * distance, (vector_y / 1000.0) * distance, (vector_z / 1000.0) * distance);
         this_vehicle->set_direction(vector_x, vector_y, vector_z);
         write(sprintf("%%^TELL%%^Orbital Control radios:%%^RESET%%^ You are cleared to orbit to vector %d %d %d.\n", vector_x, vector_y, vector_z));
         write("The ship enters into orbit, and makes its way around " + thing->short() + " to the proper escape vector.\n");
         write("%^TELL%^Orbital Control radios:%^RESET%^ Orbital maneuver complete.  Have a safe journey.\n");
      }
      else
      {
         write("Units must be less than 10000.\n");
      }
   }
   else
   {
      write("%^TELL%^Orbital Control radios:%^RESET%^ Your transmission was unclear, please rephrase request.\n");
   }

   modal_simple((: shell_input :), (: get_prompt :));
}

void display_shields()
{
   int sections = this_vehicle->query_shield_sections();
   int array shield_strengths = this_vehicle->query_all_shield_strengths();
   int array max_shield_strengths = this_vehicle->query_all_max_shield_strengths();

   printf("Shields: %s%%\n",
      color_percent((this_vehicle->query_total_shield_strength() * 100) / this_vehicle->query_total_max_shield_strength()));

   if (sections > 1)
   {
      for (int count = 1; count <= sections; count++)
      {
         printf("\n%10s: %s%%", get_shield_name(sections, count),
            color_percent((this_vehicle->query_shield_strength(count) * 100) / this_vehicle->query_max_shield_strength(count)));
      }

      write("\n\n");
   }
}

void do_launch()
{
   if (!this_vehicle->is_launched())
   {
      object carrier_ship = environment(environment(environment(environment(this_body()))));
      int vector_x = 10 - random(21);
      int vector_y = 10 - random(21);
      int vector_z = 10 - random(21);
      float vector_length = sqrt(pow(vector_x, 2) + pow(vector_y, 2) + pow(vector_z, 2));
      vector_x = to_int((vector_x * 1000) / (vector_length != 0.0  ? vector_length : 1));
      vector_y = to_int((vector_y * 1000) / (vector_length != 0.0  ? vector_length : 1));
      vector_z = to_int((vector_z * 1000) / (vector_length != 0.0  ? vector_length : 1));

      if (carrier_ship && carrier_ship->is_space_object())
      {
         int radius = carrier_ship->get_radius();
         int random_distance = random(151);
         this_vehicle->set_location((carrier_ship->get_location_x() + radius + 50 + random_distance),
            (carrier_ship->get_location_y() + radius + 50 + random_distance),
            (carrier_ship->get_location_z() + radius + 50 + random_distance));

         this_vehicle->move(environment(carrier_ship));
      }
      else
      {
         object dock_room = environment(environment(environment(this_body())));

         if (dock_room && strlen(dock_room->get_space_room()))
         {
            int radius = dock_room->get_space_location_radius();
            int random_distance = random(151);
            this_vehicle->set_location((dock_room->get_space_location_x() + radius + 50 + random_distance),
               (dock_room->get_space_location_y() + radius + 50 + random_distance),
               (dock_room->get_space_location_z() + radius + 50 + random_distance));

            this_vehicle->move(dock_room->get_space_room());
         }
         else
         {
            write("There's nowhere to launch to from this dock, please contact a wizard.\n");

            return;
         }
      }

      this_vehicle->set_launched(1);
      this_vehicle->set_direction(vector_x, vector_y, vector_z);
      this_body()->simple_action("$N $vlaunch the vehicle into space.");
   }
   else
   {
      write("This vehicle has already been launched into space.\n");
   }
}

void do_launch_vector(int x, int y, int z)
{
   if (!this_vehicle->is_launched())
   {
      if ((x < 10000) && (y < 10000) && (z < 10000))
      {
         object carrier_ship = environment(environment(environment(environment(this_body()))));
         float xs = pow(x, 2);
         float ys = pow(y, 2);
         float zs = pow(z, 2);
         float vector_length = sqrt(xs + ys + zs);

         x = to_int((x * 1000) / (vector_length != 0.0  ? vector_length : 1));
         y = to_int((y * 1000) / (vector_length != 0.0  ? vector_length : 1));
         z = to_int((z * 1000) / (vector_length != 0.0  ? vector_length : 1));

         if (carrier_ship && carrier_ship->is_space_object())
         {
            int radius = carrier_ship->get_radius();
            this_vehicle->set_location((carrier_ship->get_location_x() + radius + 50) + (random(151) * (x / 1000)),
               (carrier_ship->get_location_y() + radius + 50) + (random(151) * (y / 1000)),
               (carrier_ship->get_location_z() + radius + 50) + (random(151) * (z / 1000)));

            this_vehicle->move(environment(carrier_ship));
         }
         else
         {
            object dock_room = environment(environment(environment(this_body())));

            if (dock_room && strlen(dock_room->get_space_room()))
            {
               int radius = dock_room->get_space_location_radius();
               this_vehicle->set_location((dock_room->get_space_location_x() + radius + 50) + (random(151) * (x / 1000)),
                  (dock_room->get_space_location_y() + radius + 50) + (random(151) * (y / 1000)),
                  (dock_room->get_space_location_z() + radius + 50) + (random(151) * (z / 1000)));

               this_vehicle->move(dock_room->get_space_room());
            }
            else
            {
               write("There's nowhere to launch to from this dock, please contact a wizard.\n");
            }
         }

         this_vehicle->set_launched(1);
         this_vehicle->set_direction(x, y, z);
         this_body()->simple_action("$N $vlaunch the vehicle into space.");
      }
      else
      {
         write("Units must be less than 10000.\n");
      }
   }
   else
   {
      write("This vehicle has already been launched into space.\n");
   }
}

void do_land(string target)
{
   object thing = find_space_object(target);

   if (thing && thing->is_space_dock())
   {
      write("You see the following docks:\n");
      write(implode(thing->get_visible_docks(), ", "));
      write("\nChoose a dock:\n");
      modal_simple((: dock_input, thing :), (: get_prompt :));
   }
   else
   {
      write("You don't see any place to land.\n");
   }
}

void do_orbit(string target)
{
   object thing = find_space_object(target);

   if (thing && thing->is_space_orbitable())
   {
      write("%^TELL%^Orbital Control radios:%^RESET%^ Request for orbit acknowledged. Please transmit desired orbital maneuver at this time.\n");
      modal_simple((: orbit_input, thing :), (: get_prompt :));
   }
   else
   {
      write("That doesn't seem like something you could orbit.\n");
      modal_simple((: shell_input :), (: get_prompt :));
   }
}

void show_players(string thing)
{
   object ship = find_space_object(thing);

   if (ship)
   {
      string array players = ({ });

      foreach (object bleh in deep_inventory(ship))
      {
         if (bleh->is_body())
         {
            players += ({ capitalize(bleh->query_userid()) });
         }
      }

      if (sizeof(players))
      {
         sort_array(players, 1);

         write("Players aboard " + ship->short() + ":\n");
         write(implode(players, "\n") + "\n");
      }
      else
      {
         write("No players aboard " + ship->short() + ".\n");
      }
   }
}

void send_radio_message(string message)
{
   foreach (object thing in all_inventory(environment(this_vehicle)))
   {
      if (thing->is_space_object())
      {
         foreach (object room in all_inventory(thing))
         {
            if (present("console", room))
            {
               tell_from_outside(room, sprintf("%%^CYAN%%^%s radios:%%^RESET%%^ %s\n", this_vehicle->short(), message));
            }
         }
      }
   }
}

void show_info()
{
   write(this_vehicle->short() + " status report:\n");

   write("Speed: " + this_vehicle->get_velocity() + " / " + this_vehicle->get_max_velocity() + "\n\n");

   printf("Shields: %s%%  Hull: %s%%\n",
      color_percent((this_vehicle->query_total_shield_strength() * 100) / this_vehicle->query_total_max_shield_strength()),
      color_percent((this_vehicle->query_hull_strength() * 100) / this_vehicle->query_max_hull_strength()));
}

void show_help()
{
   write("Commands available:\n\n");

   write("scan                     - Displays scan information\n");
   write("vector <x> <y> <z>       - Changes the ship's direction\n");
   write("launch [x y z]           - Launches the ship from a dock\n");
   write("land <object>            - Lands a ship on an object\n");
   write("sp <speed> [auto pilot]  - Adjusts speed and optionally sets auto pilot\n");
   write("ap <auto pilot>          - Sets auto pilot\n");
   write("orbit <object>           - Orbits an object in space\n");
   write("target <ship>            - Sets the target for bridge or turret weapon\n");
   write("players <ship>           - Shows all players aboard a ship\n");
   write("radio <message>          - Sends a message to all ships in the area\n");
   write("stop                     - Stops the ship abruptly in an emergency\n");
   write("exit                     - Exits the ship control shell\n");
   write("help                     - Displays this information\n");
}

object find_space_object(string name)
{
   foreach (object space_object in (all_inventory(environment(this_vehicle)) - ({ this_vehicle })))
   {
      if (space_object->id(name))
      {
         return space_object;
      }
   }
}

string get_prompt()
{
   return sprintf("Speed: %d] %s", this_vehicle->get_velocity(),
      (this_vehicle->get_auto_pilot() ? sprintf("{%d} ", this_vehicle->get_auto_pilot()) : ""));
}

string color_percent(int percent)
{
   switch (percent)
   {
      case 90..100: return sprintf("%s%i%s", "%^BOLD%^%^GREEN%^", percent, "%^RESET%^");
      case 60..89: return sprintf("%s%i%s", "%^YELLOW%^", percent, "%^RESET%^");
      case 0..59: return sprintf("%s%i%s", "%^BOLD%^%^RED%^", percent, "%^RESET%^");
   }

   return sprintf("%i", percent);
}

void start_menu()
{
   this_vehicle = environment(environment(this_body()));

   if (!this_vehicle)
   {
      write("Vehicle's parent object not found.\n");
      return;
   }

   this_body()->set_flag(F_IN_EDIT);
   modal_simple((: shell_input :), (: get_prompt :));
}