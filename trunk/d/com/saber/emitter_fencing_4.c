inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("fencing emitter (IV)", "fencing emitter", "fencing", "emitter");
   set_long("Fencing emitters are most suitable for parrying hand-to-hand attacks.");
   set_component_name("fencing");
   set_component_type("emitter");
   set_damage_bonuses(([ "energy" : 4 ]));
   set_parry_bonus(15);
   set_mass(100);
   set_value(15000);
}