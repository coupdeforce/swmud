inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("disrupting emitter", "disrupting", "emitter", "lightsaber_emitter");
   set_adj("standard");
   set_long("Disrupting emitters create an unsteady beam which can wreak havoc on a victim's nervous system.");
   set_component_name("disrupting");
   set_component_type("emitter");
   set_stun_bonus(9);
   set_damage_bonuses(([ "energy" : 3 ]));
   set_mass(100);
   set_value(13000);
}