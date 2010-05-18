inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("ion energy cell (III)", "ion", "ecell", "cell");
   set_long("This power cell modifies the ionic output of lightsabers.");
   set_component_name("ion");
   set_component_type("energy cell");
   set_damage_bonuses(([ "ion" : 3, "vs droid" : 6 ]));
   set_mass(500);
   set_value(13000);
}