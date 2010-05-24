inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("ostrine edge (II)", "ostrine", "edge");
   set_long("Ostrine is a rare mineral that exists on the planet Hargeeva.  Ostrine is excellent at absorbing heat and can inflict cold damage when refined and applied to a blade.");
   set_component_name("ostrine");
   set_component_type("edge");
   set_damage_bonuses(([ "cold" : 5 ]));
   set_critical_chance_bonus(3);
   set_mass(500);
   set_value(15000);
}