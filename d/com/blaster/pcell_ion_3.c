inherit M_BLASTER_COMPONENT;

void setup()
{
   set_id("ion charger power cell (III)", "charger", "pcell", "cell");
   set_long("Ion chargers add an ion charge to a blaster beam, which is especially devastating against droids.");
   set_component_name("ion charger");
   set_component_type("power cell");
   set_damage_bonuses(([ "ion" : 6, "vs droid" : 9 ]));
   set_max_ammo_recharge_time_bonus(30);
   set_mass(500);
   set_value(16000);
}