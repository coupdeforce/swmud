inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("Ankarres Sapphire", "ankarres", "sapphire", "crystal");
   set_long("This crystal possesses legendary healing powers and numerous other effects.  It is one of the most potent crystals that can be used in lightsaber creation.");
   set_component_name("ankarres sapphire");
   set_component_type("focusing");
   set_attribute_bonuses(([ "str" : 10, "dex" : 5 ]));
   set_heal_bonus(2);
   set_critical_chance_bonus(10);
   set_mass(50);
   set_value(7500);
}