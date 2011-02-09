inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("deflection emitter (II)", "deflection", "emitter");
   set_long("Deflection emitters are optimized for use against blaster fire.");
   set_component_name("deflection");
   set_component_type("emitter");
   set_deflection_bonus(2);
   set_mass(100);
   set_value(12000);
}