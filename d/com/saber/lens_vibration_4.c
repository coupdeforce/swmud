inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("vibration lens", "vibration", "lens", "lightsaber_lens");
   set_adj("advanced");
   set_long("This lens vibrates very rapidly, resulting in a less stable but more deadly beam.");
   set_component_name("vibration");
   set_component_type("lens");
   set_damage_bonuses(([ "slashing" : 9, "energy" : 5 ]));
   set_critical_chance_bonus(-35);
   set_deflection_bonus(-7);
   set_mass(50);
   set_value(15000);
}