inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("kunda lens (V)", "kunda", "lens");
   set_long("The kunda stone has numerous practical applications in medicine and communications. It can also be combined with a normal lens to produce a broader, yet still focused beam.");
   set_component_name("kunda");
   set_component_type("lens");
   set_damage_bonuses(([ "energy" : -10 ]));
   set_to_hit_bonus(50);
   set_critical_chance_bonus(20);
   set_critical_multiplier_bonus(5);
   set_deflection_bonus(240);
   set_mass(50);
   set_value(18000);
}