inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("discharge energy cell", "energy cell", "discharge", "ecell", "cell", "lightsaber_ecell");
   set_adj("basic");
   set_long("This power cell modifies the damage potential of lightsabers.");
   set_component_name("discharge");
   set_component_type("energy cell");
   set_critical_multiplier_bonus(1);
   set_damage_bonuses(([ "energy" : 1 ]));
   set_mass(500);
   set_value(11000);
}