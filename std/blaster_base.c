inherit AMMO_WEAPON;
inherit RANGED_BASE;

void mudlib_setup()
{
   add_id("blaster");
   set_combat_messages("combat-blaster");
   set_skill_used("blaster_combat");
   ::mudlib_setup();
}

int is_blaster() { return 1; }

mixed direct_holster_obj_to_obj() { return 1; }
mixed direct_unholster_obj_from_obj() { return 1; }