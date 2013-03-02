inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("pinpoint scope", "scope", "blaster_scope");
   set_adj("improved");
   set_long("Pinpoint scopes increase the chance of slowing a target's attacks.");
   set_component_name("pinpoint");
   set_component_type("scope");
   set_slow_bonus(6);
   set_mass(500);
   set_value(14000);
}