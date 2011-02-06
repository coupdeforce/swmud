inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("contoured grip (II)", "contoured", "grip");
   set_long("This shorter grip allows maximum accuracy, but the reduced leverage limits the weapon's power.");
   set_component_name("contoured");
   set_component_type("grip");
   set_to_hit_bonus(30);
   set_damage_bonuses(([ "slashing" : -4 ]));
   set_mass(500);
   set_value(16000);
}