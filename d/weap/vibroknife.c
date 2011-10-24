inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibroknife", "knife", "vknife");
   set_long("This knife has a small, vibrating blade.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 12);
   set_durability(12);
   set_skill_used("slash 1h");
   set_weapon_speed(1);
   set_mass(2000);
   set_value(600);
}

int can_use_blade_energy_cell() { return 1; }