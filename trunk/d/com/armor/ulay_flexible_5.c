inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("flexible underlay (V)", "flexible", "ulay", "underlay");
   set_long("Flexible underlays allow for greater freedom of movement.");
   set_component_name("flexible");
   set_component_type("underlay");
   set_attribute_bonuses(([ "dex" : 15 ]));
   set_mass(1000);
   set_value(40000);
}