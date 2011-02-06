inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("diatium energy cell (II)", "diatium", "ecell", "cell");
   set_long("This power cell modifies the energy output of lightsabers.");
   set_component_name("diatium");
   set_component_type("energy cell");
   set_damage_bonuses(([ "energy" : 2 ]));
   set_mass(500);
   set_value(12000);
}