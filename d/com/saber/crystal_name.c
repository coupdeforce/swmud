inherit M_LIGHTSABER_COMPONENT;

string crystal_name = "";

void setup(string name)
{
   crystal_name = name;

   set_id(capitalize(crystal_name) + " crystal", crystal_name, "crystal");
   set_long(".");
   set_component_name("name");
   set_component_type("focusing");
   set_damage_bonuses(([ "energy" : 2 ]));
   set_mass(50);
   set_value(0);
}