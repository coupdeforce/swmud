inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("accuracy scope (V)", "scope");
   set_long("Accuracy scopes increase the chance to hit a target.");
   set_component_name("accuracy");
   set_component_type("scope");
   set_to_hit_bonus(100);
   set_mass(500);
   set_value(22000);
}