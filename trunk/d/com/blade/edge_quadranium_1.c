inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("quadranium edge (I)", "quadranium", "edge");
   set_long("Quadranium is an amazingly strong material that is sometimes used for fuel storage containers.");
   set_component_name("quadranium");
   set_component_type("edge");
   set_damage_bonuses(([ "slashing" : 2 ]));
   set_mass(500);
   set_value(12000);
}