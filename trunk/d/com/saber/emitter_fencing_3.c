inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("fencing emitter", "fencing", "emitter", "lightsaber_emitter");
   set_adj("standard");
   set_long("Fencing emitters are most suitable for parrying hand-to-hand attacks.");
   set_component_name("fencing");
   set_component_type("emitter");
   set_damage_bonuses(([ "energy" : 3 ]));
   set_parry_bonus(9);
   set_mass(100);
   set_value(13000);
}