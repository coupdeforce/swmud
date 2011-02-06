inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("ablative plating (IV)", "ablative", "plating", "olay", "overlay");
   set_long("Ablative plating increases defense and reduces damage from energy-based attacks, while sacrificing some freedom of movement.");
   set_component_name("ablative");
   set_component_type("overlay");
   set_armor_bonus(4);
   set_resistance_bonuses(([ "%energy" : 25 ]));
   set_attribute_bonuses(([ "dex" : -10 ]));
   set_mass(1000);
   set_value(35000);
}