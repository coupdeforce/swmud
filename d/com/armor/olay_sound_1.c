inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("sound dampening (I)", "sound", "dampening", "olay", "overlay");
   set_long("Sound dampening overlays provide excellent protection against a sonic assault, as well as reducing damage from sonic attacks.");
   set_component_name("sound dampening");
   set_component_type("overlay");
   set_resistance_bonuses(([ "sonic" : 2, "%sonic" : 10 ]));
   set_mass(1000);
   set_value(20000);
}