inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("diatium energy cell", "energy cell", "diatium", "ecell", "cell", "lightsaber_ecell");
   set_adj("improved");
   set_long("This power cell modifies the energy output of lightsabers.");
   set_component_name("diatium");
   set_component_type("energy cell");
   set_damage_bonuses(([ "energy" : 4 ]));
   set_mass(500);
   set_value(12000);
}