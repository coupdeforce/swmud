inherit HOLSTER;

void setup()
{
   set_id("weapons holster", "holster");
   set_long("A brown leather holster with enough room for two one-handed weapons, which you can wear around your waist.");
   set_holster_relation("holstered");
   set_mass(800);
   set_slot("waist");
   set_wear_relation("around");
}

int indirect_holster_obj_in_obj() { return 1; }
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

mixed receive_object(object target, string relation)
{
   if (target->is_weapon() && !target->query("two_hands")
      && !target->is_lightsaber() && !target->is_blaster()
      && (sizeof(all_inventory(this_object())) < 2))
   {
      return 1;
   }

   return target->short() + " cannot be holstered in a weapons holster.\n";
}