// Last edited by deforce on 04-04-2010
inherit VERB_OB;

void calm_target(object target);

void do_calm_liv(object target)
{
   if (target->query_target() || sizeof(target->query_unfinished_business()))
   {
      object this_body = this_body();
      int alignment = this_body->query_jedi_alignment();

      this_body->add_skill_delay(8);
      this_body->adjust_jedi_alignment(1);

      if (this_body->test_skill("force_calm", alignment * (alignment < 0 ? 10 : 5)))
      {
         calm_target(target);
      }
      else
      {
         this_body->targetted_action("$N $vfail to calm $p1 aggression.", target);
      }
   }
   else
   {
      write(capitalize(target->short()) + " has no aggression with anyone.\n");
   }
}

void do_calm()
{
   object target = this_body()->get_target();

   if (!this_body()->is_body())
   {
      calm_target(target);

      return;
   }

   if (target) { do_calm_liv(target); }
   else { write("You are not in combat with anyone.\n"); }
}

mixed can_calm_liv()
{
   object this_body = this_body();

   if (this_body->query_guild_level("jedi"))
   {
      if (!this_body->has_learned_skill("force calm"))
      {
         return "You have not learned how to calm aggression with the Force.\n";
      }
      else if (this_body->query_guild_level("assassin"))
      {
         return "You are too dedicated to the Dark side of the Force to use it for calming aggression.\n";
      }
      else if (this_body->has_buff("/d/buffs/anger"))
      {
         return "You are too angry to use the Force for calming aggression.\n";
      }
      if (this_body->has_skill_delay())
      {
         return "You are too busy to calm someone's aggression.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are too distracted to calm someone's aggression.\n";
      }
   }
   else
   {
      return "Only Jedi know how to use the Force to calm someone's aggression.\n";
   }

   return 1;
}

mixed can_calm()
{
   object this_body = this_body();

   if (this_body->query_guild_level("jedi"))
   {
      if (!this_body->has_learned_skill("force calm"))
      {
         return "You have not learned how to calm aggression with the Force.\n";
      }
      else if (this_body->query_guild_level("assassin"))
      {
         return "You are too dedicated to the Dark side of the Force to use it for calming aggression.\n";
      }
      else if (this_body->has_buff("/d/buffs/anger"))
      {
         return "You are too angry to use the Force for calming aggression.\n";
      }
      if (this_body->has_skill_delay())
      {
         return "You are too busy to calm someone's aggression.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are too distracted to calm someone's aggression.\n";
      }
   }
   else
   {
      return "Only Jedi know how to use the Force to calm someone's aggression.\n";
   }

   return 1;
}

void calm_target(object target)
{
   object this_body = this_body();
   object array aggressions = ({ this_body }) + target->query_targets() + target->query_unfinished_business();
   int rank = this_body->query_skill("force_calm") / 100;

   if (rank < 10)
   {
      int calm_count = 0;

      while ((calm_count <= rank) && (calm_count < sizeof(aggressions)))
      {
         target->stop_fight(aggressions[calm_count]);
         aggressions[calm_count]->stop_fight(target);

         calm_count++;
      }
   }
   else
   {
      foreach (object thing in aggressions)
      {
         target->stop_fight(thing);
         thing->stop_fight(target);
      }
   }

   this_body->targetted_action("$N $vcalm $p1 aggression.", target);
}

void create()
{
   add_rules( ({ "", "LIV" }) );
}