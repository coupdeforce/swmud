inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibrosword", "sword", "vsword");
   set_long("This sword can barely be wielded in one hand, with a vibrating blade close to a meter long.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 20);
   set_durability(20);
   set_skill_used("slash_1h");
   set_weapon_class(20);
   set_weapon_speed(1);
   set_mass(6000);
   set_value(2000);
}

int can_use_energy_cell() { return 1; }