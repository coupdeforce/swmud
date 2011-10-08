inherit VERB_OB;

void do_plant_obj(object ob)
{
   if (ob->id("horticulture_seed"))
   {
      if (environment(this_body())->query_available_plant_room() < 1)
      {
         write("There is currently no room for any more plants here.\n");
      }
      else if (this_body()->query_race() == "ithorian")
      {
         string type = ob->query_seed_type();
         int potency = ob->query_seed_potency();

         if (strlen(type) && (potency > 0))
         {
            int rank = this_body()->query_skill("horticulture") / 100;
            int max_potency = 20 + (rank * 8);

            destruct(ob);

            if (this_body()->test_skill("horticulture"))
            {
               max_potency += random(10);
            }
            else
            {
               max_potency -= random(10);
            }

            if (max_potency < 10)
            {
               max_potency = 10;
            }
            else if (max_potency > 100)
            {
               max_potency = 100;
            }

            if (potency > max_potency)
            {
               potency = max_potency;
            }

            write("You plant " + add_article(type) + " seed, with " + potency + "% potency.\n");
            this_body()->other_action("$N $vplant " + add_article(type) + " seed.");

            load_object("/d/obj/horticulture_plant");
            new("/d/obj/horticulture_plant", type, potency)->move(environment(this_body()));

            this_body()->add_experience((this_body()->query_primary_level() > 0) ? (10 * array_sum(this_body()->query_guild_levels())) : 10);
         }
      }
      else
      {
         write("Only Ithorians know how to plant seeds.\n");
      }
   }
   else
   {
      write("You're not able to plant " + ob->short() + ".\n");
   }
}

void do_plant_obs(array info, string name)
{
   handle_obs(info, (: do_plant_obj :));
}

mixed can_plant_obj()
{
   if (!environment(this_body())->is_garden_here())
   {
      return "There is no garden here.\n";
   }

   return 1;
}

mixed can_plant_obs()
{
   if (!environment(this_body())->is_garden_here())
   {
      return "There is no garden here.\n";
   }

   return 1;
}

void do_plant()
{
   write("Plant what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}