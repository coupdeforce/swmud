inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("precision chamber (I)", "precision chamber", "chamber");
   set_long("Precision chambers create a beam with much greater accuracy and range, while sacrificing the maximum damage potential.");
   set_component_name("precision chamber");
   set_component_type("chamber");
   set_to_hit_bonus(20);
   set_range_bonus(1);
   set_critical_chance_bonus(-5);
   set_mass(500);
   set_value(12000);
}