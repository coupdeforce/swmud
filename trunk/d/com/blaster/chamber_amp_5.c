inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("amplifying chamber (V)", "amplifying chamber", "amplifier", "chamber", "amp");
   set_long("Amplifying chambers increase the raw energy output of a blaster beam.");
   set_component_name("amplifying");
   set_component_type("chamber");
   set_damage_bonuses(([ "energy" : 16 ]));
   set_mass(500);
   set_value(22000);
}