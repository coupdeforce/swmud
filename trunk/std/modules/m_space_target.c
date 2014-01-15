// Functions
int is_space_target();
int query_hull_strength();
void set_hull_strength(int value);
int query_max_hull_strength();
void set_max_hull_strength(int value);
void adjust_hull_strength(int amount);
int query_shield_sections();
void set_shield_sections(int value);
int find_shield_section(int x, int y, int z);
int query_shield_strength(int section);
void set_shield_strength(int section, int value);
int query_max_shield_strength(int section);
void set_max_shield_strength(int section, int value);
void adjust_shield_strength(int section, int amount);
int query_total_shield_strength();
int query_total_max_shield_strength();
int array query_all_shield_strengths();
int array query_all_max_shield_strengths();

// Variables
int hull_strength = 10;
int max_hull_strength = 10;
int shield_sections = 1;
int array shield_strength = ({ 10 });
int array max_shield_strength = ({ 10 });

int is_space_target() { return 1; }

int query_hull_strength()
{
   return hull_strength;
}

void set_hull_strength(int value)
{
   if (value > 0)
   {
      hull_strength = value;

      if (max_hull_strength < hull_strength)
      {
         max_hull_strength = hull_strength;
      }
   }
}

int query_max_hull_strength()
{
   return max_hull_strength;
}

void set_max_hull_strength(int value)
{
   if (value > 0)
   {
      max_hull_strength = value;

      if (hull_strength > max_hull_strength)
      {
         hull_strength = max_hull_strength;
      }
   }
}

void adjust_hull_strength(int amount)
{
   hull_strength += amount;

   if (hull_strength < 0)
   {
      hull_strength = 0;
   }
   else if (hull_strength > max_hull_strength)
   {
      hull_strength = max_hull_strength;
   }
}

int query_shield_sections()
{
   return shield_sections;
}

int find_shield_section(int x, int y, int z)
{
   return 1;
}

void set_shield_sections(int value)
{
   if ((value >= 0) && (value < 9))
   {
      shield_sections = value;

      while (sizeof(shield_strength) < shield_sections)
      {
         shield_strength += ({ 10 });
         max_shield_strength += ({ 10 });
      }
   }
}

int query_shield_strength(int section)
{
   if ((section > 0) && (section <= shield_sections) && (sizeof(shield_strength) <= shield_sections))
   {
      return shield_strength[section - 1];
   }

   return 0;
}

void set_shield_strength(int section, int value)
{
   if ((section > 0) && (sizeof(shield_strength) >= section) && (value >= 0))
   {
      shield_strength[section - 1] = value;

      if ((sizeof(max_shield_strength) >= section) && (max_shield_strength[section - 1] < value))
      {
         max_shield_strength[section - 1] = value;
      }
   }
}

int query_max_shield_strength(int section)
{
   if ((section > 0) && (section <= shield_sections) && (sizeof(max_shield_strength) <= shield_sections))
   {
      return max_shield_strength[section - 1];
   }

   return 0;
}

void set_max_shield_strength(int section, int value)
{
   if ((section > 0) && (sizeof(max_shield_strength) >= section) && (value >= 0))
   {
      max_shield_strength[section - 1] = value;

      if ((sizeof(shield_strength) >= section) && (shield_strength[section - 1] > value))
      {
         shield_strength[section - 1] = value;
      }
   }
}

void adjust_shield_strength(int section, int amount)
{
   if ((section > 0) && (sizeof(shield_strength) >= section))
   {
      section--;

      shield_strength[section] += amount;

      if (shield_strength[section] < 0)
      {
         shield_strength[section] = 0;
      }
      else if (shield_strength[section] > max_shield_strength[section])
      {
         shield_strength[section] = max_shield_strength[section];
      }
   }
}

int query_total_shield_strength()
{
   int total = 0;

   foreach (int value in shield_strength)
   {
      total += value;
   }

   return total;
}

int query_total_max_shield_strength()
{
   int total = 0;

   foreach (int value in max_shield_strength)
   {
      total += value;
   }

   return total;
}

int array query_all_shield_strengths()
{
   return shield_strength;
}

int array query_all_max_shield_strengths()
{
   return max_shield_strength;
}