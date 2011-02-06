inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("agrinium grip (IV)", "agrinium", "grip");
   set_long("This grip, made from the lighter mineral agrinium, balances a weapon towards the point, resulting in heftier swings.");
   set_component_name("agrinium");
   set_component_type("grip");
   set_damage_bonuses(([ "slashing" : 9 ]));
   set_critical_chance_bonus(-25);
   set_mass(500);
   set_value(19000);
}