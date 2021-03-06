inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("power cell", "charger", "pcell", "cell", "blaster_power_cell_standard", "blaster_pcell");
   set_adj("standard");
   set_long("A regular blaster power cell without any special properties.");
   set_component_name("blaster");
   set_component_type("power cell");
   set_damage_bonuses(([ "energy" : 3 ]));
   set_mass(400);
   set_value(1500);
}