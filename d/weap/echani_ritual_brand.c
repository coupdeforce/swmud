inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("Echani ritual brand", "brand");
   set_long("Echani Firedancers use the double-bladed Ritual Brand in a dodging and slicing pattern which is so graceful that it looks more like a dance than combat.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 12);
   set_damage_type("fire", 4);
   set_durability(16);
   set_skill_used("slash_2h");
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-25);
   set_mass(3000);
   set_value(3000);
}