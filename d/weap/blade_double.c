inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("double-blade", "doubleblade", "blade", "dblade");
   set_long("The hilt of this weapon has two blades, about half a meter long, attached at both ends.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 12);
   set_durability(12);
   set_skill_used("slash 2h");
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-15);
   set_mass(2060);
   set_value(1250);
}