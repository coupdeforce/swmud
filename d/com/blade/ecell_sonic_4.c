inherit M_BLADE_COMPONENT;

void setup()
{
   set_id("sonic discharge cell (IV)", "sonic discharge cell", "discharge cell", "sonic", "discharge", "ecell", "cell");
   set_long("Sonic discharge energy cells release sound waves upon impact.  Although the sonic discharge makes the weapon unwieldy, few types of armor protect against this type of attack.");
   set_component_name("sonic discharge");
   set_component_type("energy cell");
   set_damage_bonuses(([ "sonic" : 7 ]));
   set_to_hit_bonus(-50);
   set_critical_multiplier_bonus(7);
   set_mass(500);
   set_value(24000);
}