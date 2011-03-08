inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibroblade", "blade", "vblade");
   set_long("The vibrating blade of this weapon is about half a meter long.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 16);
   set_durability(16);
   set_skill_used("slash_1h");
   set_weapon_speed(1);
   set_mass(4500);
   set_value(1400);
}

int can_use_blade_energy_cell() { return 1; }