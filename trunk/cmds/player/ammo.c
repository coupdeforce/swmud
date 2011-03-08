inherit CMD;

private void main(string arg)
{
   object body = this_body();
   object array weapons;

   if (arg == "all")
   {
      weapons = filter_array(all_inventory(body), (: $1->requires_ammo() :));
   }
   else if (arg == "unwielded")
   {
      weapons = filter_array(filter_array(all_inventory(body), (: !$1->query_wielded_by() :)), (: $1->requires_ammo() :));
   }
   else
   {
      weapons = filter_array(body->query_weapons(), (: $1->requires_ammo() :));
   }

   if (!sizeof(weapons))
   {
      if (arg == "all")
      {
         write("You are not carrying any weapons which require ammo.\n");
      }
      else if (arg == "unwielded")
      {
         write("You are not carrying any unwielded weapons which require ammo.\n");
      }
      else
      {
         write("You are not wielding any weapons which require ammo.\n");
      }

      return;
   }

   foreach (object weapon in weapons)
   {
      write(weapon->short() + (weapon->query_wielded_by() ? weapon->wielded_attributes() : "") + ": " + weapon->query_ammo() + " " + weapon->query_ammo_desc() + " remaining\n");
   }
}