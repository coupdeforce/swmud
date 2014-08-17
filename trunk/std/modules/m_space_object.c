// Functions
int get_radius();
int get_location_x();
int get_location_y();
int get_location_z();
int array get_location_xyz();
int get_direction_x();
int get_direction_y();
int get_direction_z();
int array get_direction_xyz();
int get_velocity();
int get_max_velocity();
int get_auto_pilot();
int is_space_object();
int is_space_orbitable();
int is_launched();
void set_radius(int number);
void set_location(int x, int y, int z);
void set_direction(int x, int y, int z);
void set_launched(int value);
void set_relative_direction(string direction);
void adjust_velocity(int amount);
void set_max_velocity(int amount);
void set_auto_pilot(int amount);
void heart_beat();

// Variables
int radius = 50;
int location_x = 0;
int location_y = 0;
int location_z = 0;
int direction_x = 0;
int direction_y = 0;
int direction_z = 0;
int velocity = 0;
int max_velocity = 100;
int auto_pilot = 0;
int space_orbitable = 0;
int launch_state = 0;

int get_radius() { return radius; }
int get_location_x() { return location_x; }
int get_location_y() { return location_y; }
int get_location_z() { return location_z; }

int array get_location_xyz()
{
   return ({ location_x, location_y, location_z });
}

int get_direction_x() { return direction_x; }
int get_direction_y() { return direction_y; }
int get_direction_z() { return direction_z; }

int array get_direction_xyz()
{
   return ({ direction_x, direction_y, direction_z });
}

int get_velocity() { return velocity; }
int get_max_velocity() { return max_velocity; }
int get_auto_pilot() { return auto_pilot; }
int is_space_object() { return 1; }
int is_space_orbitable() { return space_orbitable; }
int is_launched() { return launch_state; }

void set_radius(int number)
{
   if (number > 0) { radius = number; }
}

void set_location(int x, int y, int z)
{
   location_x = x;
   location_y = y;
   location_z = z;
}

void set_direction(int x, int y, int z)
{
   direction_x = x;
   direction_y = y;
   direction_z = z;
}

void set_launched(int value)
{
   if (value > 0)
   {
      launch_state = 1;
   }
   else
   {
      launch_state = 0;
   }
}

void set_relative_direction(string direction)
{
   int degrees;

   if (lower_case(direction) == "reverse")
   {
      direction_x *= -1;
      direction_y *= -1;
      direction_z *= -1;
   }
   else if (lower_case(direction) == "left")
   {
      // 45-degree turn to the left
   }
   else if (lower_case(direction) == "right")
   {
      // 45-degree turn to the right
   }
   else if (sscanf(lower_case(direction), "left %d", degrees) == 1)
   {
      // N-degree turn to the left
   }
   else if (sscanf(lower_case(direction), "right %d", degrees) == 1)
   {
      // N-degree turn to the right
   }
}

void adjust_velocity(int amount)
{
   velocity += amount;

   if (velocity < 0) { velocity = 0; }

   if (velocity > 0)
   {
      set_heart_beat(2);
   }
   else
   {
      set_heart_beat(0);
   }
}

void set_max_velocity(int amount)
{
   if (amount >= 0)
   {
      max_velocity = amount;

      if (velocity > max_velocity)
      {
         adjust_velocity(max_velocity - velocity);
      }
   }
}

void set_auto_pilot(int amount)
{
   auto_pilot = amount;
}

void set_space_orbitable(int value)
{
   if (value > 0) { space_orbitable = 1; }
   else { space_orbitable = 0; }
}

void heart_beat()
{
   if (velocity > 0)
   {
      location_x += (direction_x * velocity) / 1000.0;
      location_y += (direction_y * velocity) / 1000.0;
      location_z += (direction_z * velocity) / 1000.0;
   }

   if (auto_pilot > 0)
   {
      auto_pilot--;

      if (auto_pilot == 0)
      {
         velocity = 0;
         set_heart_beat(0);

         foreach (object room in all_inventory())
         {
            if (present("console", room))
            {
               tell_from_outside(room, "Autopilot auto-stop engaged.\n");
            }
         }
      }
   }
}