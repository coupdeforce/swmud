// Last edited by deforce on 03-03-2010
inherit HOLSTER;

void setup()
{
   set_id("large weapon holster", "large holster", "holster");
   set_long("A brown leather holster for a two-handed weapon, which you can wear on your back.");
   add_relation("holstered in");
   set_default_relation("holstered in");
   set_mass(800);
   set_max_capacity(200);
   set_slot("back");
}

int indirect_holster_obj_to_obj() { return 1; }
int indirect_unholster_obj_from_obj() { return 1; }

int can_holster(object attachment)
{
   if (attachment->is_weapon() && attachment->query("two_hands")
      && !attachment->is_lightsaber() && !attachment->is_blaster()
      && (sizeof(all_inventory(this_object())) < 1))
   {
      return 1;
   }

   return 0;
}