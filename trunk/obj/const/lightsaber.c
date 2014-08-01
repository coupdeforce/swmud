#include <jedi_ranks.h>
#include <move.h>

int construct_object(object body)
{
   int energy = 20;
   int durability = 20;
   int mass = 2000;
   int value = 2000;
   int to_hit_base;
   object color_crystal = filter_array(all_inventory(body), (: $1->id("lightsaber_color_crystal") :))[0];
   object ecell = filter_array(all_inventory(body), (: $1->id("lightsaber_ecell") :))[0];
   object emitter = filter_array(all_inventory(body), (: $1->id("lightsaber_emitter") :))[0];
   object lens = filter_array(all_inventory(body), (: $1->id("lightsaber_lens") :))[0];
   object product = clone_object("/d/weap/lightsaber");
   int level = body->query_guild_level("jedi");
   int guild_rank_bonus = body->query_guild_rank("jedi") > 2 ? body->query_guild_rank("jedi") - 2 : 0;
   int rank = body->query_skill("saber const") / 100;
   int spec = body->query_guild_specialization_rank("jedi", "lightsaber");
   int rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);

   if (body->query_guild_rank("jedi") < 2)
   {
      string sponsor = JEDI_SPONSOR_D->query_sponsor(body->query_userid());

      if (strlen(sponsor))
      {
         object array players_in_room = filter_array(all_inventory(environment(this_body())), (: $1->query_userid() == $(sponsor) :) );

         if (!sizeof(players_in_room))
         {
            write("Your sponsor must be in the same room when you construct your first lightsaber.\n");
            return 0;
         }
      }
      else if (body->query_guild_level("jedi") < 20)
      {
         write("You must reach level 20 before you can construct your first lightsaber.\n");
         return 0;
      }
      else if (body->query_primary_guild() != "jedi")
      {
         write("You must have a sponsor when you construct your first lightsaber.\n");
         return 0;
      }
   }

   if (!product)
   {
      write("Unable to construct lightsaber: base object was not found.  Please tell a wizard.\n");
      return 0;
   }

   if (!color_crystal || !ecell || !emitter || !lens)
   {
      write("Unable to construct lightsaber: required components were not found.  Please tell a wizard.\n");
      return 0;
   }

   energy = product->query_damage_types()["energy"] + (level / 5) + rank_spec + (guild_rank_bonus * level / 5);
   durability = product->query_durability() + (level / 5) + rank_spec + (guild_rank_bonus * level / 5);
   to_hit_base = product->query_to_hit_base() + level + rank_spec + (guild_rank_bonus * level / 5);
   mass = product->query_mass() - ((level + (rank_spec * 2)) * 10) - (guild_rank_bonus * level * 4);
   value = product->query_value() + ((level + (rank_spec * 2)) * 20) + (guild_rank_bonus * level * 4);

   color_crystal->move(product);
   ecell->move(product);
   emitter->move(product);
   lens->move(product);
   product->assemble();
   product->set_long("This is a carefully constructed lightsaber.  The case is made of polished silver metal, with buttons along the side.");
   product->set_damage_type("energy", energy);
   product->set_durability(durability);
   product->set_max_durability(durability);
   product->set_to_hit_base(to_hit_base);
   product->set_mass(mass);
   product->set_value(value);
   product->set_customize_record("constructed", body->query_userid());
   product->set_autoload();

   if (product->move(body) == MOVE_NO_ROOM)
   {
      product->move(environment(body));
   }

   body->simple_action("$N carefully $vconstruct a lightsaber.");

   if (body->query_guild_rank("jedi") < 2)
   {
      string sponsor = JEDI_SPONSOR_D->query_sponsor(body->query_userid());

      if (strlen(sponsor) || (body->query_primary_guild() == "jedi"))
      {
         object array players_in_room = filter_array(all_inventory(environment(this_body())), (: $1->query_userid() == $(sponsor) :) );
         object jedi_sponsor;
         string force_message;

         if (sizeof(players_in_room))
         {
            jedi_sponsor = players_in_room[0];
         }

         if (jedi_sponsor)
         {
            mapping sponsor_times = JEDI_SPONSOR_D->query_sponsored(jedi_sponsor->query_userid());
            force_message = "%^BOLD%^%^RED%^You sense the " + (jedi_sponsor->query_jedi_alignment() < 0 ? "Dark" : "Light") + " Side of the Force grow stronger, as " + get_jedi_rank_title(jedi_sponsor->query_guild_rank("jedi"), jedi_sponsor->query_jedi_alignment()) + " " + capitalize(jedi_sponsor->query_userid()) + " has fully trained " + jedi_sponsor->query_possessive() + " " + get_jedi_rank_title(1, body->query_jedi_alignment()) + " " + capitalize(body->query_userid()) + ".%^RESET%^\n";

            jedi_sponsor->add_jedi_training_complete(body->query_userid(), sponsor_times[body->query_userid()]);

            body->set_guild_rank("jedi", 2);

            foreach (object jedi in filter_array(bodies(), (: $1->query_toggle("sense disturbance") == "on" :) ))
            {
               tell(jedi, force_message);
            }

            JEDI_SPONSOR_D->remove_sponsored(body->query_userid());
         }
         else if (body->query_primary_guild() == "jedi")
         {
            force_message = "%^BOLD%^%^RED%^You sense the " + (body->query_jedi_alignment() < 0 ? "Dark" : "Light") + " Side of the Force grow stronger, as " + capitalize(body->query_userid()) + " becomes " + add_article(get_jedi_rank_title(2, body->query_jedi_alignment())) + ".%^RESET%^\n";

            body->set_guild_rank("jedi", 2);

            foreach (object jedi in filter_array(bodies(), (: $1->query_toggle("sense disturbance") == "on" :) ))
            {
               tell(jedi, force_message);
            }
         }
      }
   }

   return 1;
}