// Last edited by deforce on 05-17-2010
int object_value = 0;

void set_value(int value)
{
   object_value = value;
}

int query_value_pure()
{
   return object_value;
}

int query_value()
{
   object this_ob = this_object();
   int value = object_value;

   if (this_ob->is_armor() || this_ob->is_weapon())
   {
      value = value * this_ob->query_durability() / this_ob->query_max_durability();

      foreach (object thing in all_inventory())
      {
         value += thing->query_value();
      }
   }

   return value;
}