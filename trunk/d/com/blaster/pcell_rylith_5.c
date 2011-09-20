inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("rylith power cell (V)", "rylith power cell", "power cell", "pcell", "cell");
   set_long("Rylith power cells greatly increase the base damage of a blaster.");
   set_component_name("rylith");
   set_component_type("power cell");
   set_damage_bonuses(([ "energy" : 16 ]));
   set_max_ammo_recharge_time_bonus(20);
   set_mass(500);
   set_value(22000);
}