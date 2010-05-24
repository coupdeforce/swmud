inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("neutronium edge (II)", "neutronium", "edge");
   set_long("Neutronium is a highly durable and resilient material used in the creation of durasteel. It can be used to increase the damage infliced by a blade.");
   set_component_name("neutronium");
   set_component_type("edge");
   set_damage_bonuses(([ "slashing" : 3 ]));
   set_critical_multiplier_bonus(2);
   set_mass(500);
   set_value(13000);
}