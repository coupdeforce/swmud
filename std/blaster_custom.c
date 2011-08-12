void process_component(object thing);

void assemble()
{
   object this_ob = this_object();
   string array chamber_names = ({ });
   string array pcell_names = ({ });
   string array scope_names = ({ });

   this_ob->reset_attribute_bonuses();
   this_ob->reset_damage_bonuses();
   this_ob->reset_to_hit_bonus();
   this_ob->reset_range_bonus();
   this_ob->reset_stun_bonus();
   this_ob->reset_slow_bonus();
   this_ob->reset_tear_bonus();
   this_ob->reset_parry_bonus();
   this_ob->reset_skill_bonuses();
   this_ob->reset_heal_bonus();
   this_ob->reset_armor_bonus();
   this_ob->reset_critical_chance_bonus();
   this_ob->reset_critical_multiplier_bonus();
   this_ob->reset_max_ammo_recharge_time_bonus();

   foreach (object thing in all_inventory(this_ob))
   {
      if (thing->query_component_type() == "blaster chamber")
      {
         if (sizeof(chamber_names) >= 1)
         {
            chamber_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         chamber_names += ({ thing->query_component_name() });

         process_component(thing);
      }
      else if (thing->query_component_type() == "blaster power cell")
      {
         if (sizeof(pcell_names) >= 1)
         {
            pcell_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         pcell_names += ({ thing->query_component_name() });

         process_component(thing);
      }
      else if (thing->query_component_type() == "blaster scope")
      {
         if (sizeof(scope_names) >= 1)
         {
            scope_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         scope_names += ({ thing->query_component_name() });

         process_component(thing);
      }
   }
}

void process_component(object thing)
{
   object this_ob = this_object();

   foreach (string type, int bonus in thing->query_damage_bonuses())
   {
      this_ob->add_damage_bonus(type, bonus);
   }

   foreach (string type, int bonus in thing->query_attribute_bonuses())
   {
      this_ob->add_attribute_bonus(type, bonus);
   }

   foreach (string skill, int bonus in thing->query_skill_bonuses())
   {
      this_ob->add_skill_bonus(skill, bonus);
   }

   this_ob->add_to_hit_bonus(thing->query_to_hit_bonus());
   this_ob->add_range_bonus(thing->query_range_bonus());
   this_ob->add_stun_bonus(thing->query_stun_bonus());
   this_ob->add_slow_bonus(thing->query_slow_bonus());
   this_ob->add_tear_bonus(thing->query_tear_bonus());
   this_ob->add_parry_bonus(thing->query_parry_bonus());
   this_ob->add_heal_bonus(thing->query_heal_bonus());
   this_ob->add_armor_bonus(thing->query_armor_bonus());
   this_ob->add_critical_chance_bonus(thing->query_critical_chance_bonus());
   this_ob->add_critical_multiplier_bonus(thing->query_critical_multiplier_bonus());
   this_ob->add_max_ammo_recharge_time_bonus(this_ob->query_base_max_ammo_recharge_time() * thing->query_max_ammo_recharge_time_bonus() / 100);
}

int indirect_install_obj_on_obj() { return 1; }
int indirect_install_obj_in_obj() { return 1; }
int indirect_uninstall_obj_from_obj() { return 1; }
string query_relation(object ob) { return "on"; }
int is_modifiable_blaster() { return 1; }
int can_use_blaster_chamber() { return 1; }
int can_use_blaster_power_cell() { return 1; }
int can_use_blaster_scope() { return 1; }

mixed receive_object(object target, string relation)
{
   switch (target->query_component_type())
   {
      case "blaster chamber":
         if (this_object()->can_use_blaster_chamber() && !this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }
         return target->short() + " cannot be used with that type of blaster.\n";
      case "blaster power cell":
         if (this_object()->can_use_blaster_power_cell() && !this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }
         return target->short() + " cannot be used with that type of blaster.\n";
      case "blaster scope":
         if (this_object()->can_use_blaster_scope() && !this_object()->cannot_use_component(target->query_component_type(), target->query_component_name())) { return 1; }
         return target->short() + " cannot be used with that type of blaster.\n";
   }

   return target->short() + " is not part of a blaster.\n";
}

void do_check_obj()
{
   if (sizeof(all_inventory()))
   {
      string array chambers = ({ "" });
      string array pcells = ({ "" });
      string array scopes = ({ "" });

      foreach (object thing in all_inventory())
      {
         switch (thing->query_component_type())
         {
            case "blaster chamber":
               chambers = ({ thing->query_component_name() }) + chambers; break;
            case "blaster power cell":
               pcells = ({ thing->query_component_name() }) + pcells; break;
            case "blaster scope":
               scopes = ({ thing->query_component_name() }) + scopes; break;
         }
      }

      write("You check " + this_object()->the_short() + " and discover that it has the following components:\n\n");

      if (this_object()->can_use_blaster_chamber())
      {
         write(sprintf("   Chamber: %s\n", chambers[0]));
      }

      if (this_object()->can_use_blaster_power_cell())
      {
         write(sprintf("Power Cell: %s\n", pcells[0]));
      }

      if (this_object()->can_use_blaster_scope())
      {
         write(sprintf("     Scope: %s\n\n", scopes[0]));
      }
   }
   else
   {
      write("You check " + this_object()->the_short() + " and discover that it has no modifications beyond the basic components.\n");
   }

   if (this_object()->requires_ammo())
   {
      int last_recharged = time() - this_object()->query_last_recharge_time();

      write("It was last recharged " + convert_time(last_recharged) + " ago and has " + this_object()->query_ammo() + " " + this_object()->query_ammo_desc() + " remaining.\n");

      if ((this_object()->query_max_ammo_recharge_time() - last_recharged) > 0)
      {
         write("The remaining time on its energy cell is " + convert_time(this_object()->query_max_ammo_recharge_time() - last_recharged) + ".\n");
      }
      else
      {
         write("Its energy cell has no charge left.\n");
      }
   }
}