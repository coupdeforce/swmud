inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("Kaiburr crystal", "kaiburr", "crystal");
   set_long("Perhaps the most pwerful crystal that can be used in a lightsaber, the Kaiburr gem bestows insight and fortitude upon its owner.");
   set_component_name("kaiburr");
   set_component_type("focusing");
   set_attribute_bonuses(([ "int" : 15, "con" : 15 ]));
   set_heal_bonus(3);
   set_mass(50);
   set_value(20000);
}