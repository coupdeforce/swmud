inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("energy cell", "ecell", "cell", "lightsaber_ecell_superior", "lightsaber_ecell");
   set_adj("superior");
   set_long("A regular lightsaber energy cell without any special properties.");
   set_component_name("lightsaber");
   set_component_type("energy cell");
   set_damage_bonuses(([ "energy" : 5 ]));
   set_mass(400);
   set_value(2500);
}