void process_component(object thing);

void assemble()
{
   object this_ob = this_object();
   string array overlay_names = ({ });
   string array underlay_names = ({ });

   this_ob->reset_attribute_bonuses();
   this_ob->reset_resistance_bonuses();
   this_ob->reset_weakness_bonuses();
   this_ob->reset_skill_bonuses();
   this_ob->reset_to_hit_bonus();
   this_ob->reset_heal_bonus();
   this_ob->reset_armor_bonus();
   this_ob->reset_body_armor_bonus();

   foreach (object thing in all_inventory(this_ob))
   {
      if (thing->query_component_type() == "armor overlay")
      {
         if (sizeof(overlay_names) >= 1)
         {
            overlay_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         overlay_names += ({ thing->query_component_name() });

         process_component(thing);
      }
      else if (thing->query_component_type() == "armor underlay")
      {
         if (sizeof(underlay_names) >= 1)
         {
            underlay_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         underlay_names += ({ thing->query_component_name() });

         process_component(thing);
      }
   }
}

void process_component(object thing)
{
   object this_ob = this_object();

   foreach (string type, int bonus in thing->query_attribute_bonuses())
   {
      this_ob->add_attribute_bonus(type, bonus);
   }

   foreach (string type, int bonus in thing->query_resistance_bonuses())
   {
      this_ob->add_resistance_bonus(type, bonus);
   }

   foreach (string type, int bonus in thing->query_weakness_bonuses())
   {
      this_ob->add_weakness_bonus(type, bonus);
   }

   foreach (string skill, int bonus in thing->query_skill_bonuses())
   {
      this_ob->add_skill_bonus(skill, bonus);
   }

   this_ob->add_to_hit_bonus(thing->query_to_hit_bonus());
   this_ob->add_heal_bonus(thing->query_heal_bonus());
   this_ob->add_armor_bonus(thing->query_armor_bonus());
   this_ob->add_body_armor_bonus(thing->query_body_armor_bonus());
}

int indirect_install_obj_on_obj() { return 1; }
int indirect_install_obj_in_obj() { return 1; }
int indirect_uninstall_obj_from_obj() { return 1; }
string query_relation(object ob) { return "on"; }
int is_modifiable_armor() { return 1; }
int can_use_armor_overlay() { return 1; }
int can_use_armor_underlay() { return 1; }

mixed receive_object(object target, string relation)
{
   switch (target->query_component_type())
   {
      case "armor overlay":
         if (this_object()->can_use_armor_overlay() && !this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }
      case "armor underlay":
         if (this_object()->can_use_armor_underlay() && !this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }

      return target->short() + " cannot be used with that type of clothing or armor.\n";
   }

   return target->short() + " is not compatible with clothing or armor.\n";
}

void do_check_obj()
{
   if (sizeof(all_inventory()))
   {
      string array overlays = ({ "" });
      string array underlays = ({ "" });

      foreach (object thing in all_inventory())
      {
         switch (thing->query_component_type())
         {
         case "armor overlay":
            overlays = ({ thing->query_component_name() }) + overlays; break;
         case "armor underlay":
            underlays = ({ thing->query_component_name() }) + underlays; break;
         }
      }

      write("You check " + this_object()->the_short() + " and discover that it has the following components:\n\n");

      if (this_object()->can_use_armor_overlay())
      {
         write(sprintf(" Overlay: %s\n", overlays[0]));
      }

      if (this_object()->can_use_armor_underlay())
      {
         write(sprintf("Underlay: %s\n", underlays[0]));
      }

      return;
   }

   write("You check " + this_object()->the_short() + " and discover that it has no modifications beyond the basic components.\n");
}