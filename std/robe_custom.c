void process_component(object thing);

void assemble()
{
   object this_ob = this_object();
   string array underlay_names = ({ });

   this_ob->reset_attribute_bonuses();
   this_ob->reset_resistance_bonuses();
   this_ob->reset_weakness_bonuses();
   this_ob->reset_to_hit_bonus();
   this_ob->reset_heal_bonus();
   this_ob->reset_armor_bonus();
   this_ob->reset_body_armor_bonus();

   foreach (object thing in all_inventory(this_ob))
   {
      if (thing->query_component_type() == "armor underlay")
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

   this_ob->add_to_hit_bonus(thing->query_to_hit_bonus());
   this_ob->add_heal_bonus(thing->query_heal_bonus());
   this_ob->add_armor_bonus(thing->query_armor_bonus());
   this_ob->add_body_armor_bonus(thing->query_body_armor_bonus());
}

int indirect_install_obj_on_obj() { return 1; }
int indirect_install_obj_in_obj() { return 1; }
int indirect_uninstall_obj_from_obj() { return 1; }
string query_relation(object ob) { return "under"; }
int is_modifiable_armor() { return 1; }

mixed receive_object(object target, string relation)
{
   switch (target->query_component_type())
   {
      case "armor underlay":
         if (!this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }

      return target->short() + " cannot be used under a robe.\n";
   }

   return target->short() + " is not compatible with a robe.\n";
}

void do_check_obj()
{
   if (sizeof(all_inventory()))
   {
      string array underlays = ({ "" });

      foreach (object thing in all_inventory())
      {
         if (thing->query_component_type() == "armor underlay")
         {
            underlays = ({ thing->query_component_name() }) + underlays;
         }
      }

      write("You check " + this_object()->the_short() + " and discover that it has a " + underlays[0] + " underlay.\n");

      return;
   }

   write("You check " + this_object()->the_short() + " and discover that it has no underlay.\n");
}