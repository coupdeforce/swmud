inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("rylith power cell (III)", "rylith power cell", "power cell", "pcell", "cell");
   set_long("Rylith power cells greatly increase the base damage of a blaster.");
   set_component_name("rylith");
   set_component_type("power cell");
   set_damage_bonuses(([ "energy" : 8 ]));
   set_max_ammo_recharge_time_bonus(12);
   set_mass(500);
   set_value(16000);
}