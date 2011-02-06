inherit WEAPON;

void mudlib_setup()
{
   add_id("blade");
   set_combat_messages("combat-blade");
   ::mudlib_setup();
}

int is_blade() { return 1; }

mixed direct_holster_obj_to_obj() { return 1; }
mixed direct_unholster_obj_from_obj() { return 1; }
/*
class event_info source_modify_event(class event_info evt)
{
   if (stringp(evt->data)) { return evt; }

   if (evt->data[1] > 0)
   {
      object wielder = query_wielded_by();
      string skill = this_object()->query_skill_used();
      int rank = (wielder->query_skill(skill) / 100);
      int weapon_class = this_object()->query_weapon_class();
      int damage_before_adj = evt->data[1];
      int cap = floor(weapon_class * ((10.0 + (rank * 9.0)) / 100.0));

      if (evt->data[1] > cap)
      {
         evt->data[1] = (cap > 0 ? cap : 1);

         tell(wielder, sprintf("Damage adjusted from %d to %d.\n", damage_before_adj, evt->data[1]));
      }
   }

   return evt;
}
*/