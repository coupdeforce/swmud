// Last edited by deforce on 03-03-2010
inherit HOLSTER;

void setup()
{
   set_id("weapons holster", "holster");
   set_long("A brown leather holster with enough room for two one-handed weapons, which you can wear around your waist.");
   add_relation("holstered in");
   set_default_relation("holstered in");
   set_mass(800);
   set_max_capacity(200);
   set_slot("waist");
}

int indirect_holster_obj_to_obj() { return 1; }
int indirect_unholster_obj_from_obj() { return 1; }

int can_holster(object attachment)
{
   if (attachment->is_weapon() && !attachment->query("two_hands")
      && !attachment->is_lightsaber() && !attachment->is_blaster()
      && (sizeof(all_inventory(this_object())) < 2))
   {
      return 1;
   }

   return 0;
}