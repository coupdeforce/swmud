inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("ionite edge (V)", "ionite edge", "ionite", "edge");
   set_long("Ionite alloy produces the inverse charge of its surroundings, creating a disrupting effect to electronic systems.");
   set_component_name("ionite");
   set_component_type("edge");
   set_damage_bonuses(([ "ion" : 8, "vs droid" : 12 ]));
   set_mass(500);
   set_value(22000);
}