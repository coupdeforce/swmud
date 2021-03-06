inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("emitter", "lightsaber_emitter_superior", "lightsaber_emitter");
   set_adj("superior");
   set_long("A regular lightsaber emitter without any special properties.");
   set_component_name("lightsaber");
   set_component_type("emitter");
   set_damage_bonuses(([ "energy" : 8 ]));
   set_mass(400);
   set_value(2500);
}