inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("mullinine edge (III)", "mullinine", "edge");
   set_long("Mullinine is an ideal building material for melee weapons as it holds an edge very well.");
   set_component_name("mullinine");
   set_component_type("edge");
   set_damage_bonuses(([ "slashing" : 3 ]));
   set_critical_chance_bonus(15);
   set_mass(500);
   set_value(13000);
}