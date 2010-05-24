// Last edited by deforce on 05-13-2010
void process_component(object thing);

void assemble()
{
   object this_ob = this_object();
   int alignment = 0;
   string color = "";
   string array crystal_names = ({ });
   string array ecell_names = ({ });
   string array emitter_names = ({ });
   string array lens_names = ({ });

   this_ob->reset_attribute_bonuses();
   this_ob->reset_damage_bonuses();
   this_ob->reset_to_hit_bonus();
   this_ob->reset_stun_bonus();
   this_ob->reset_slow_bonus();
   this_ob->reset_parry_bonus();
   this_ob->reset_heal_bonus();
   this_ob->reset_armor_bonus();
   this_ob->reset_critical_chance_bonus();
   this_ob->reset_critical_multiplier_bonus();
   this_ob->reset_deflection_bonus();

   foreach (object thing in all_inventory(this_ob))
   {
      if (thing->query_component_type() == "lightsaber color")
      {
         if (!strlen(color))
         {
            color = thing->query_component_name();
         }
         else
         {
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));
         }
      }
      else if (thing->query_component_type() == "lightsaber focusing")
      {
         if ((member_array(thing->query_component_name(), crystal_names) > -1)
            || (sizeof(crystal_names) >= 2))
         {
            crystal_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         crystal_names += ({ thing->query_component_name() });

         process_component(thing);

         if (thing->query_alignment_restriction())
         {
            if (!alignment)
            {
               alignment = thing->query_alignment_restriction();
            }
            else if (thing->query_alignment_restriction() != alignment)
            {
               environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
               thing->move(environment(this_ob));
            }
         }
      }
      else if (thing->query_component_type() == "lightsaber energy cell")
      {
         if ((member_array(thing->query_component_name(), ecell_names) > -1)
            || (sizeof(ecell_names) >= 1))
         {
            ecell_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         ecell_names += ({ thing->query_component_name() });

         process_component(thing);
      }
      else if (thing->query_component_type() == "lightsaber emitter")
      {
         if ((member_array(thing->query_component_name(), emitter_names) > -1)
            || (sizeof(emitter_names) >= 1))
         {
            emitter_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         emitter_names += ({ thing->query_component_name() });

         process_component(thing);
      }
      else if (thing->query_component_type() == "lightsaber lens")
      {
         if ((member_array(thing->query_component_name(), lens_names) > -1)
            || (sizeof(lens_names) >= 1))
         {
            lens_names -= ({ thing->query_component_name() });
            environment(this_ob)->simple_action("$N $vuninstall a $o from a $o1.", thing, this_ob);
            thing->move(environment(this_ob));

            continue;
         }

         lens_names += ({ thing->query_component_name() });

         process_component(thing);
      }
   }

   this_ob->set_lightsaber_color(color);
   this_ob->set_lightsaber_alignment(alignment);
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

   this_ob->add_to_hit_bonus(thing->query_to_hit_bonus());
   this_ob->add_stun_bonus(thing->query_stun_bonus());
   this_ob->add_slow_bonus(thing->query_slow_bonus());
   this_ob->add_parry_bonus(thing->query_parry_bonus());
   this_ob->add_heal_bonus(thing->query_heal_bonus());
   this_ob->add_armor_bonus(thing->query_armor_bonus());
   this_ob->add_critical_chance_bonus(thing->query_critical_chance_bonus());
   this_ob->add_critical_multiplier_bonus(thing->query_critical_multiplier_bonus());
   this_ob->add_deflection_bonus(thing->query_deflection_bonus());
}

int indirect_install_obj_in_obj() { return 1; }
int indirect_uninstall_obj_from_obj() { return 1; }
string query_relation(object ob) { return "in"; }

mixed receive_object(object target, string relation)
{
   switch (target->query_component_type())
   {
      case "lightsaber color": return 1;
      case "lightsaber emitter": return 1;
      case "lightsaber energy cell": return 1;
      case "lightsaber focusing": return 1;
      case "lightsaber lens": return 1;
   }

   return target->short() + " does not belong in a lightsaber.\n";
}

void do_check_obj()
{
   if (sizeof(all_inventory()))
   {
      string array color_crystals = ({ "" });
      string array focusing_crystals = ({ "", "" });
      string array lenses = ({ "" });
      string array emitters = ({ "" });
      string array ecells = ({ "" });

      foreach (object thing in all_inventory())
      {
         switch (thing->query_component_type())
         {
         case "lightsaber color":
            color_crystals = ({ thing->query_component_name() }) + color_crystals; break;
         case "lightsaber focusing":
            focusing_crystals = ({ thing->query_component_name() }) + focusing_crystals; break;
         case "lightsaber lens":
            lenses = ({ thing->query_component_name() }) + lenses; break;
         case "lightsaber emitter":
            emitters = ({ thing->query_component_name() }) + emitters; break;
         case "lightsaber energy cell":
            ecells = ({ thing->query_component_name() }) + ecells; break;
         }
      }

      write("You check " + this_object()->the_short() + " and discover that it has the following components:\n\n");

      write(sprintf("    Color Crystal: %-20s        Lens: %-20s\n", color_crystals[0], lenses[0]));
      write(sprintf("Focusing Crystals: %-20s     Emitter: %-20s\n", focusing_crystals[0], emitters[0]));
      write(sprintf("                   %-20s Energy Cell: %-20s\n\n", focusing_crystals[1], ecells[0]));

      return;
   }

   write("You check " + this_object()->the_short() + " and discover that it has no modifications beyond the basic components.\n");
}