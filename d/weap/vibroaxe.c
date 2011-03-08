inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibroaxe", "axe", "vaxe");
   set_long("This large, heavy axe has a vibrating blade.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 14);
   set_damage_type("striking", 14);
   set_durability(14);
   set_skill_used("slash_2h");
   set("two_hands", 1);
   set_weapon_speed(1);
   set_mass(9250);
   set_value(4800);
}

int can_use_blade_energy_cell() { return 1; }