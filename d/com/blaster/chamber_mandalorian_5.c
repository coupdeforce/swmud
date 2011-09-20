inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("Mandalorian chamber (V)", "mandalorian chamber", "chamber");
   set_long("Mandalorian chambers sacrifice the charge time of the energy cell in order to greatly increase the base damage of a blaster, as well as its maximum damage potential.");
   set_component_name("Mandalorian");
   set_component_type("chamber");
   set_damage_bonuses(([ "energy" : 20 ]));
   set_critical_chance_bonus(15);
   set_critical_multiplier_bonus(5);
   set_max_ammo_recharge_time_bonus(-50);
   set_mass(500);
   set_value(22000);
}