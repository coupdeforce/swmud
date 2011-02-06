inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("beam gem lens (II)", "beam", "gem", "lens");
   set_long("The aptly named beam gem crystals are common in optical computers such as those involved in navigational systems. Not surprisingly, they also can create a highly focues lightsaber beam.");
   set_component_name("beam gem");
   set_component_type("lens");
   set_damage_bonuses(([ "energy" : 2 ]));
   set_critical_chance_bonus(10);
   set_deflection_bonus(-9);
   set_mass(50);
   set_value(12000);
}