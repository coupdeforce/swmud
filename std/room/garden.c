private int max_plants = 30;
private int max_seeds = 30;
private string array seeds = ({ });

void set_max_plants(int value)
{
   if (value > 0)
   {
      max_plants = value;
   }
}

int query_max_plants()
{
   return max_plants;
}

int query_total_plants()
{
   return sizeof(filter_array(all_inventory(), (: $1->id("horticulture_plant") :)));
}

int query_available_plant_room()
{
   return max_plants - sizeof(filter_array(all_inventory(), (: $1->id("horticulture_plant") :)));
}

void set_max_seeds(int value)
{
   if (value > 0)
   {
      max_seeds = value;
   }
}

varargs void set_random_seeds(string array types)
{
   if (!sizeof(types))
   {
      types = ({ "healing", "antidote" });
   }

   while (sizeof(seeds) < max_seeds)
   {
      seeds += ({ choice(types) });
   }
}

int query_max_seeds()
{
   return max_seeds;
}

string array query_seeds()
{
   return seeds;
}

string gather_random_seed()
{
   if (sizeof(seeds))
   {
      string type = seeds[0];
      seeds = seeds[1..];

      return type;
   }

   return "none";
}

int gather_seed(string type)
{
   if (member_array(type, seeds) > -1)
   {
      seeds -= ({ type });

      return 1;
   }

   return 0;
}

void add_seed(string type)
{
   seeds += ({ type });

   if (sizeof(seeds) > max_seeds)
   {
      seeds = seeds[1..];
   }
}

void add_seeds(string type, int amount)
{
   while (amount > 0)
   {
      seeds += ({ type });
      amount--;
   }

   if (sizeof(seeds) > max_seeds)
   {
      seeds = seeds[(sizeof(seeds) - max_seeds)..];
   }
}

int is_garden_here() { return 1; }