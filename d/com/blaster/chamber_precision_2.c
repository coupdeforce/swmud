inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("precision chamber (II)", "precision chamber", "chamber");
   set_long("Precision chambers create a beam with much greater accuracy and range, while sacrificing the maximum damage potential.");
   set_component_name("precision chamber");
   set_component_type("chamber");
   set_to_hit_bonus(50);
   set_range_bonus(2);
   set_critical_chance_bonus(-10);
   set_mass(500);
   set_value(14000);
}