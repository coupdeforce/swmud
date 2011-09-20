inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("crippling scope (V)", "crippling scope", "scope");
   set_long("Crippling scopes increase the chance of stunning a target.");
   set_component_name("crippling");
   set_component_type("scope");
   set_stun_bonus(25);
   set_mass(500);
   set_value(22000);
}