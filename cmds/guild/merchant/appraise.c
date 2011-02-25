inherit VERB_OB;

void appraise(object thing);

void do_appraise_obj(object thing)
{
   object this_body = this_body();

   if (thing->is_living())
   {
      if (thing != this_body)
      {
         write("You judge " + thing->short() + " to be a fine, upstanding member of society.\n");
      }
      else
      {
         write("You judge yourself to be an example of everything that is good in the universe.\n");
      }

      return;
   }

   this_body->add_skill_delay(2);

   if (this_body->test_skill("appraise", ((this_body->query_guild_level("merchant") + this_body->query_guild_level("smuggler")) * 10)))
   {
      appraise(thing);
   }
   else
   {
      write("You fail to appraise the value of " + thing->the_short() + ".\n");
      this_body()->other_action("$N $vlook at $o and it boggles $p mind.\n", thing);
   }
}

void do_appraise_obs(array info, object thing)
{
   handle_obs(info, (: do_appraise_obj :), thing);
}

void do_appraise()
{
   write("Appraise what?\n");
}

void appraise(object thing)
{
   object this_body = this_body();
   int perception = this_body->query_per();
   int merchant_level = this_body->query_guild_level("merchant");
   int smuggler_level = this_body->query_guild_level("smuggler");
   int rank = this_body->query_skill("appraise") / 100;
   int value = thing->query_value();
   int sell_value = value;
   int variation = random(230 - merchant_level - smuggler_level - perception - (rank * 10));

   if (perception < random(100))
   {
      value += (value * 10 * variation / 100);
   }

   if (smuggler_level)
   {
      write(capitalize(thing->the_short()) + " appears to be worth "  + value + " credits.\n");
   }

   if (merchant_level)
   {
      write("You think you could sell " + thing->the_short() + " for "  + (value * 100 / 300) + " credits.\n");
   }
}

mixed can_appraise_obj()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant") || this_body->query_guild_level("smuggler"))
   {
      if (!this_body->has_learned_skill("appraising"))
      {
         return "You have not learned how to appraise the value of something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to appraise the value of somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on appraising the value of something.\n";
      }
   }
   else
   {
      return "Only merchants and smugglers know how to appraise the value of something.\n";
   }

   return 1;
}

mixed can_appraise()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant") || this_body->query_guild_level("smuggler"))
   {
      if (!this_body->has_learned_skill("appraising"))
      {
         return "You have not learned how to appraise the value of something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to appraise the value of somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on appraising the value of something.\n";
      }
   }
   else
   {
      return "Only merchants and smugglers know how to appraise the value of something.\n";
   }

   return 1;
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}