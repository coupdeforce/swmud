inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("double-sword", "doublesword", "sword", "dsword");
   set_long("The hilt of this weapon has two sword blades, close to a meter long, attached at both ends.  The entire length is just over 2 meters.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 16);
   set_durability(16);
   set_skill_used("slash 2h");
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-25);
   set_mass(2830);
   set_value(1780);
}