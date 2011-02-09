inherit BLADE_BASE;
inherit BLADE_CUSTOM;

void setup()
{
   set_id("vibro double-sword", "vibrosword", "vibro doublesword", "double-sword", "doublesword", "sword", "vsword", "vdsword");
   set_long("The hilt of this weapon has two vibrating sword blades, close to a meter long, attached at both ends.  The entire length is just over 2 meters.");
   set_combat_messages("combat-blade");
   set_damage_type("slashing", 20);
   set_durability(20);
   set_skill_used("slash_2h");
   set("two_hands", 1);
   set_weapon_speed(2);
   set_to_hit_base(-25);
   set_mass(8500);
   set_value(5350);
}

int can_use_energy_cell() { return 1; }

class event_info source_modify_event(class event_info evt)
{
   if (stringp(evt->data)) { return evt; }

   if ((query_wielded_by()->query_per() / 5) > random(20))
   {
      int damage_before_adj = evt->data[1];

      query_wielded_by()->targetted_action("%^YELLOW%^$N $vdo a perceptive strike against $t with $p $o!%^RESET%^", evt->target, this_object());

      evt->data[1] += (random(query_wielded_by()->query_per() / 5) + 1) * 2;

      tell(query_wielded_by(), sprintf("Damage adjusted from %d to %d.\n", damage_before_adj, evt->data[1]));
   }
   else if (((100 - query_wielded_by()->query_per()) / 5) > random(19))
   {
      query_wielded_by()->my_action("%^BOLD%^%^RED%^$N $vinjure $r with $p $o!%^RESET%^", this_object());
      query_wielded_by()->other_action("%^BOLD%^%^RED%^$N carelessly $vinjure $r due to $p ineptitude with the $o!%^RESET%^", this_object());

      query_wielded_by()->hurt_us((random((100 - query_wielded_by()->query_per()) / 5) + 1) * 2);

      evt->data = "miss";
   }

   return evt;
}