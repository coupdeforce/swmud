inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("ion energy cell (II)", "ion energy cell", "energy cell", "ion", "ecell", "cell");
   set_long("This power cell modifies the ionic output of lightsabers.");
   set_component_name("ion");
   set_component_type("energy cell");
   set_damage_bonuses(([ "ion" : 2, "vs droid" : 4 ]));
   set_mass(500);
   set_value(12000);
}