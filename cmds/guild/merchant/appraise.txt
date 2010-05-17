// Last edited by deforce on 03-03-2010

inherit VERB_OB;

void appraise(object thing);

void do_appraise_obj(object thing)
{
   object this_body = this_body();

   this_body->add_skill_delay(2);

   if (this_body->test_skill("appraise", (this_body->query_guild_level("merchant") * 10)))
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
   float perception = this_body->query_per();
   float level = this_body->query_guild_level("merchant");
   int rank = to_int(floor(this_body->query_skill("appraise") / 100.0));
   int value = thing->query_value();

   if ((100 - level - (rank * 5)) > random(100))
   {
      int variation = random(26);

      value += value * (200 + variation - level - perception - (rank * 5)) / (200 - variation);
   }

   value = value * 100 / 300;

   write("You think you could sell " + thing->the_short() + " for "  + value + " credits.\n");
}

mixed can_appraise_obj()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant"))
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
      return "Only merchants know how to appraise the value of something.\n";
   }

   return 1;
}

mixed can_appraise()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant"))
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
      return "Only merchants know how to appraise the value of something.\n";
   }

   return 1;
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}