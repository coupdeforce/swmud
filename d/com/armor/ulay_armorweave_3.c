inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("armorweave underlay (III)", "armorweave", "armorweave underlay", "ulay", "underlay");
   set_long("Armorweave underlays reduce damage from energy-based attacks.");
   set_component_name("armorweave");
   set_component_type("underlay");
   set_resistance_bonuses(([ "%energy" : 15 ]));
   set_mass(1000);
   set_value(25000);
}