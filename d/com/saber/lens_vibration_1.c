inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("vibration lens (I)", "vibration", "lens");
   set_long("This lens vibrates very rapidly, resulting in a less stable but more deadly beam.");
   set_component_name("vibration");
   set_component_type("lens");
   set_damage_bonuses(([ "slashing" : 2 ]));
   set_critical_chance_bonus(-50);
   set_deflection_bonus(-10);
   set_mass(50);
   set_value(11000);
}