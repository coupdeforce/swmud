inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("ion energy cell (IV)", "ion energy cell", "energy cell", "ion", "ecell", "cell");
   set_long("This power cell modifies the ionic output of lightsabers.");
   set_component_name("ion");
   set_component_type("energy cell");
   set_damage_bonuses(([ "ion" : 5, "vs droid" : 9 ]));
   set_mass(500);
   set_value(15000);
}