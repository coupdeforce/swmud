inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("ion energy cell (V)", "ion", "ecell", "cell");
   set_long("This power cell modifies the ionic output of lightsabers.");
   set_component_name("ion");
   set_component_type("energy cell");
   set_damage_bonuses(([ "ion" : 8, "vs droid" : 12 ]));
   set_mass(500);
   set_value(18000);
}