inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("energy cell (I)", "energy", "ecell", "cell");
   set_long("Energy cells can stabilize a blade, increasing its damage.");
   set_component_name("energy");
   set_component_type("energy cell");
   set_damage_bonuses(([ "energy" : 2 ]));
   set_mass(500);
   set_value(12000);
}