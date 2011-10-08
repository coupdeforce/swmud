inherit VERB_OB;

void do_harvest_obj(object ob)
{
   if (ob->id("horticulture_plant"))
   {
      if (this_body()->query_race() == "ithorian")
      {
         string type = ob->query_plant_type();
         int potency = ob->query_plant_potency();

         if (ob->ready_for_harvest())
         {
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

               write("You harvest an herb from " + add_article(type) + " plant, with " + potency + "% potency.\n");
               this_body()->other_action("$N $vharvest an herb from " + add_article(type) + " plant.");

               load_object("/d/obj/horticulture_herb");
               new("/d/obj/horticulture_herb", type, potency)->move(this_body());

               this_body()->add_experience((this_body()->query_primary_level() > 0) ? (10 * array_sum(this_body()->query_guild_levels())) : 10);

               environment(this_body())->add_seed(type);
            }
         }
         else
         {
            write("The " + ob->short() + " is not ready to be harvested.\n");
         }
      }
      else
      {
         write("Only Ithorians know how to harvest plants.\n");
      }
   }
   else
   {
      write("You're not able to harvest " + ob->short() + ".\n");
   }
}

void do_harvest_obs(array info, string name)
{
   handle_obs(info, (: do_harvest_obj :));
}

void do_harvest()
{
   write("Harvest what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}