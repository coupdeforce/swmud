inherit M_ARMOR_COMPONENT;

void setup()
{
   set_id("strengthening underlay (I)", "strengthening", "ulay", "underlay");
   set_long("Strengthening underlays allow for greater strength and constitution.");
   set_component_name("strengthening");
   set_component_type("underlay");
   set_attribute_bonuses(([ "str" : 1, "con" : 1 ]));
   set_mass(1000);
   set_value(12000);
}