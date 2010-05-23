inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("blade");
   set_long("The blade of this weapon is about half a meter long.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 12);
   set_durability(12);
   set_skill_used("slash_1h");
   set_weapon_speed(1);
   set_mass(1500);
   set_value(460);
}