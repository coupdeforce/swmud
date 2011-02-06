// Last edited by deforce on 03-03-2010
inherit HOLSTER;

void setup()
{
   set_id("lightsaber belt", "saber belt", "belt");
   set_long("A simple, brown leather belt with attachments for a standard lightsaber on both sides.");
   add_relation("attached to");
   set_default_relation("attached to");
   set_mass(400);
   set_max_capacity(200);
   set_slot("waist");
}

int indirect_attach_obj_to_obj() { return 1; }
int indirect_detach_obj_from_obj() { return 1; }

int can_attach(object attachment)
{
   if (attachment->is_lightsaber() && !attachment->query("two_hands")
      && (sizeof(all_inventory(this_object())) < 2))
   {
      return 1;
   }

   return 0;
}