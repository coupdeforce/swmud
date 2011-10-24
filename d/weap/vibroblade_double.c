inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibro double-blade", "vibroblade", "vibro doubleblade", "double-blade", "doubleblade", "blade", "vblade", "vdblade");
   set_long("The hilt of this weapon has two vibrating blades, about half a meter long, attached at both ends.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 16);
   set_durability(16);
   set_skill_used("slash 2h");
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-15);
   set_mass(6200);
   set_value(3750);
}

int can_use_blade_energy_cell() { return 1; }