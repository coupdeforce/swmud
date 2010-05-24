// Last edited by deforce on 05-12-2010
inherit LIGHTSABER_BASE;
inherit LIGHTSABER_CUSTOM;

void setup()
{
   add_id("double-bladed lightsaber", "dsaber");
   set_long("This is a simple double-bladed lightsaber, manufactured for those who have not yet constructed their own.  The case is made of polished silver metal, with buttons along the side.");
   set_damage_type("energy", 20);
   set_durability(20);
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-25);
   set_mass(2000);
   set_value(2000);
}