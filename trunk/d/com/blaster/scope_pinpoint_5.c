inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("pinpoint scope (V)", "pinpoint scope", "scope");
   set_long("Pinpoint scopes increase the chance of slowing a target's attacks.");
   set_component_name("pinpoint");
   set_component_type("scope");
   set_slow_bonus(25);
   set_mass(500);
   set_value(22000);
}