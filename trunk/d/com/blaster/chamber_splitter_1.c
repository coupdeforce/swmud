inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("beam splitter (I)", "splitter", "chamber");
   set_long("Beam splitters create two or more divergent beams from the same blaster, which increases the chance of hitting a target as well as the maximum damage potential while also increasing the base damage.");
   set_component_name("beam splitter");
   set_component_type("chamber");
   set_damage_bonuses(([ "energy" : 1 ]));
   set_to_hit_bonus(5);
   set_critical_chance_bonus(5);
   set_mass(500);
   set_value(12000);
}