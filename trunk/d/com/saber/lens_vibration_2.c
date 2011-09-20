inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("vibration lens (II)", "vibration lens", "vibration", "lens");
   set_long("This lens vibrates very rapidly, resulting in a less stable but more deadly beam.");
   set_component_name("vibration");
   set_component_type("lens");
   set_damage_bonuses(([ "slashing" : 4 ]));
   set_critical_chance_bonus(-45);
   set_deflection_bonus(-9);
   set_mass(50);
   set_value(12000);
}