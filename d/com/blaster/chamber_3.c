inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("chamber", "blaster_chamber_standard", "blaster_chamber");
   set_adj("standard");
   set_long("A regular blaster chamber without any special properties.");
   set_component_name("blaster");
   set_component_type("chamber");
   set_damage_bonuses(([ "energy" : 3 ]));
   set_mass(300);
   set_value(1500);
}