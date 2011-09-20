inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("discharge energy cell (IV)", "discharge energy cell", "energy cell", "discharge", "ecell", "cell");
   set_long("This power cell modifies the damage potential of lightsabers.");
   set_component_name("discharge");
   set_component_type("energy cell");
   set_critical_multiplier_bonus(5);
   set_mass(500);
   set_value(15000);
}