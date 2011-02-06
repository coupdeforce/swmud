// Last edited by deforce on 06-04-2008
inherit VERB_OB;

void do_class_obj(object thing)
{
   if (thing->is_armor() || thing->is_weapon())
   {
      string array required_learned_skills = thing->query_required_learned_skills();
      mapping required_skills = thing->query_required_skills();
      mapping required_guilds = thing->query_required_guilds();

      write(thing->short() + " has the following properties:\n");

      if (thing->is_weapon())
      {
         write(sprintf("\n%16s: %i\n", "Weapon Class", thing->query_weapon_class()));

         foreach (string type, int amount in thing->query_damage_types())
         {
            write(sprintf("%16s: %i\n", title_capitalize(type), amount));
         }

         if (thing->query_to_hit_base())
         {
            write(sprintf("\n%16s: %i", "To-Hit Base", thing->query_to_hit_base()));
         }

         if (thing->query_to_hit_bonus())
         {
            write(sprintf("\n%16s: %i", "To-Hit Bonus", thing->query_to_hit_bonus()));
         }

         if (thing->query_parry_bonus())
         {
            write(sprintf("\n%16s: %i", "Parry Bonus", thing->query_parry_bonus()));
         }

         if (thing->query_stun_bonus())
         {
            write(sprintf("\n%16s: %i", "Stun Bonus", thing->query_stun_bonus()));
         }

         if (thing->query_slow_bonus())
         {
            write(sprintf("\n%16s: %i", "Slow Bonus", thing->query_slow_bonus()));
         }

         if (thing->query_tear_bonus())
         {
            write(sprintf("\n%16s: %i", "Tear Bonus", thing->query_tear_bonus()));
         }

         if (thing->query_critical_chance_bonus())
         {
            write(sprintf("\n%16s: %i", "Critical % Bonus", thing->query_critical_chance_bonus()));
         }

         if (thing->query_critical_multiplier_bonus())
         {
            write(sprintf("\n%16s: %i", "Critical x Bonus", thing->query_critical_multiplier_bonus()));
         }

         if (!thing->is_living())
         {
            write(sprintf("\n%16s: %i", "Base Speed", thing->query_weapon_speed()));
         }

         if (thing->is_lightsaber())
         {
            if (thing->query_deflection_bonus())
            {
               write(sprintf("\n%16s: %i", "Deflection Bonus", thing->query_deflection_bonus()));
            }
         }
      }

      if (thing->is_armor())
      {
         if (thing->is_living())
         {
            write(sprintf("\n%16s: %i", "Armor Class", thing->query_armor_class() + thing->query_jedi_armor_class()));
         }
         else
         {
            write(sprintf("\n%16s: %i", "Armor Class", thing->query_armor_class()));
            write(sprintf("\n%16s: %i", "Max Armor Class", thing->query_max_armor_class()));
         }
      }

      if (sizeof(thing->query_attribute_bonuses()))
      {
         write("\n\nAttribute Bonuses:");

         foreach (string type, int bonus in thing->query_attribute_bonuses())
         {
            write(sprintf("\n%16s: %i", capitalize(type), bonus));
         }

         write("\n");
      }

      if (thing->query_heal_bonus())
      {
         write(sprintf("\n%16s: %i\n", "Heal Bonus", thing->query_heal_bonus()));
      }

      if (thing->query_armor_bonus())
      {
         write(sprintf("\n%16s: %i\n", "Armor Bonus", thing->query_armor_bonus()));
      }

      if (!thing->is_living())
      {
         write(sprintf("\n%16s: %i", "Durability", thing->query_durability()));
         write(sprintf("\n%16s: %i", "Max Durability", thing->query_max_durability()));
         write(sprintf("\n%16s: %i", "Value", thing->query_value()));
      }

      if (sizeof(required_learned_skills))
      {
         string array lines = ({ });

         write("\nRequires the following learned skills:\n");

         foreach (string learned_skill in required_learned_skills)
         {
            lines += ({ "  " + learned_skill });
         }

         write(implode(sort_array(lines, 1), "\n") + "\n");
      }

      if (sizeof(required_skills))
      {
         string array lines = ({ });

         write("\nRequires the following skill training:\n");

         foreach (string skill in keys(required_skills))
         {
            mixed data = SKILL_D->query_skill(skill);

            if (data)
            {
               lines += ({ sprintf("  %-43s : %i", data[0] + " (" + skill + ")", required_skills[skill]) });
            }
            else
            {
               lines += ({ sprintf("  %-43s : %i", skill + " (invalid)", required_skills[skill]) });
            }
         }

         write(implode(sort_array(lines, 1), "\n") + "\n");
      }

      if (sizeof(required_guilds))
      {
         string array lines = ({ });

         write("\nRequires the following guilds:\n");

         foreach (string guild in keys(required_guilds))
         {
            lines += ({ sprintf("  %-13s : %i", title_capitalize(guild), required_guilds[guild]) });
         }

         write(implode(sort_array(lines, 1), "\n") + "\n");
      }

      write("\n");
   }
}

void do_class()
{
   write("Get the armor/weapon class of what?\n");
}

mixed can_class_obj() { return wizardp(this_body()); }
mixed can_class() { return wizardp(this_body()); }

void create()
{
   add_rules( ({ "", "OBJ" }) );
}