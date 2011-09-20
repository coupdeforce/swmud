inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("targeting scope (II)", "targeting scope", "scope");
   set_long("A targeting scope will increase the range and chance to hit.");
   set_component_name("targeting");
   set_component_type("scope");
   set_to_hit_bonus(20);
   set_range_bonus(2);
   set_mass(500);
   set_value(14000);
}