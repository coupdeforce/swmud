inherit AMMO_WEAPON;
inherit RANGED_BASE;

void mudlib_setup()
{
   ammo_weapon::mudlib_setup();
   ranged_base::mudlib_setup();
   add_id("blaster");
   set_combat_messages("combat-blaster");
   set_skill_used("blaster combat");
   set_repair_skill("blaster repair");
   set_repair_guilds(({"engineer", "mercenary", "smuggler", "bounty hunter"}));
   set_parts_type("blaster");
}

int is_blaster() { return 1; }