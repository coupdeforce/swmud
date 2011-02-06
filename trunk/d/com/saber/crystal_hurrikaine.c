inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("Hurrikaine crystal", "hurrikaine", "crystal");
   set_long("The very rare Hurrikaine crystal is best known for its unparalleled beauty.  When used in a lightsaber, the dark gemstone creates a devistating beam that can partially penitrate all defenses.");
   set_component_name("hurrikaine");
   set_component_type("focusing");
   set_damage_bonuses(([ "unstoppable" : 8 ]));
   set_mass(50);
   set_value(15000);
}