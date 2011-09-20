inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("ion energy cell (III)", "ion energy cell", "energy cell", "ion", "ecell", "cell");
   set_long("Ion cells discharge ion energy along the length of a blade.");
   set_component_name("ion");
   set_component_type("energy cell");
   set_damage_bonuses(([ "ion" : 6, "vs droid" : 9 ]));
   set_mass(500);
   set_value(16000);
}