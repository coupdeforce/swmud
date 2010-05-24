inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("jolt energy cell (V)", "jolt", "ecell", "cell");
   set_long("This power cell modifies the electrical output of lightsabers.");
   set_component_name("jolt");
   set_component_type("energy cell");
   set_damage_bonuses(([ "electrical" : 8 ]));
   set_mass(500);
   set_value(18000);
}