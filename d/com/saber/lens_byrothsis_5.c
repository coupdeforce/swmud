inherit M_LIGHTSABER_COMPONENT;

void setup()
{
   set_id("byrothsis lens", "byrothsis", "lens", "lightsaber_lens");
   set_adj("superior");
   set_long("Byrothsis was originally used to create infrared camera lenses, but has since found use in other applications including lightsaber lenses.");
   set_component_name("byrothsis");
   set_component_type("lens");
   set_damage_bonuses(([ "energy" : 8 ]));
   set_to_hit_bonus(75);
   set_deflection_bonus(-6);
   set_mass(50);
   set_value(18000);
}