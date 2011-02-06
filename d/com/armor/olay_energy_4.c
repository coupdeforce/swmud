inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("energy shielding (IV)", "energy", "shielding", "olay", "overlay");
   set_long("Energy shielding reduces damage from energy-based attacks.");
   set_component_name("energy");
   set_component_type("overlay");
   set_resistance_bonuses(([ "%energy" : 20 ]));
   set_mass(1000);
   set_value(30000);
}