inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("heat shielding (III)", "heat shielding", "heat shielding overlay", "olay", "overlay");
   set_long("Heat shielding reduces damage from fire and extreme heat.");
   set_component_name("heat");
   set_component_type("overlay");
   set_resistance_bonuses(([ "fire" : 6, "%fire" : 15 ]));
   set_mass(1000);
   set_value(25000);
}