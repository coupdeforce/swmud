inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("broadened chamber (II)", "broadened chamber", "chamber");
   set_long("Broadened chambers broaden the beam of blasters, increasing the chance to hit as well as the maximum damage potential.");
   set_component_name("broadened");
   set_component_type("chamber");
   set_to_hit_bonus(20);
   set_critical_chance_bonus(6);
   set_critical_multiplier_bonus(2);
   set_mass(500);
   set_value(14000);
}