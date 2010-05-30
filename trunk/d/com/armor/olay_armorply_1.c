inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("armorply plating (I)", "armorply", "plating", "olay", "overlay");
   set_long("Armorply plating increases freedom of movement and makes it easier to remain undetected.");
   set_component_name("armorply");
   set_component_type("overlay");
   set_attribute_bonuses(([ "dex" : 1 ]));
   set_mass(1000);
   set_value(12000);
}