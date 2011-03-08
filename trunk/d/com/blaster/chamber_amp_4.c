inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("amplifying chamber (IV)", "amplifier", "chamber", "amp");
   set_long("Amplifying chambers increase the raw energy output of a blaster beam.");
   set_component_name("amplifying");
   set_component_type("chamber");
   set_damage_bonuses(([ "energy" : 12 ]));
   set_mass(500);
   set_value(19000);
}