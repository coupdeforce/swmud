// Last edited by deforce on 11-14-2009
inherit VERB_OB;

void backstab(object body);

void do_backstab_liv(object living)
{
   object this_body = this_body();

   if (living == this_body)
   {
      write("You stab yourself in the back and will never forgive yourself.\n");
   }
   else
   {
      this_body->add_skill_delay(4);

      if (this_body->test_skill("backstab", (this_body->query_guild_level("assassin") * 6)))
      {
         backstab(living);
      }
      else
      {
         int skill;

         this_body->refresh_stats();

         skill = ((50.0 / this_body->query_guild_level("assassin")) + (1000.0 / ((this_body->query_skill("backstab") + 1) * 10.0))) * (100.0 / this_body->query_agi()) * (100.0 / this_body->query_dex());

         if (random(100) < skill)
         {
            this_body->targetted_action("$N $vtry to stab $t in the back and $vfail terribly, injuring $r in the process!\n", living);

            this_body->weaken_us(random(this_body->query_weapon()->query_weapon_class()) + 1);
         }
         else
         {
            this_body->targetted_action("$N $vtry to stab $t in the back, and $vfail.\n", living);
         }

         if (!living->is_body() && (random(living->query_per() + 1) > random(100)))
         {
            living->attacked_by(this_body, 1);
         }
      }
   }
}

mixed can_backstab_liv()
{
   object this_body = this_body();

   if (this_body->query_guild_level("assassin"))
   {
      if (!this_body->has_learned_skill("backstab"))
      {
         return "You have not learned how to backstab someone.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to backstab when you're already in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on backstabbing someone.\n";
      }
   }
   else
   {
      return "Only assassins know how to backstab someone.\n";
   }

   return 1;
}

void do_backstab()
{
   write("Backstab who?\n");
}

void backstab(object body)
{
   object this_body = this_body();
   object weapon = this_body->query_weapon();
   int rank = to_int(floor(this_body->query_skill("backstab") / 100.0));
   int damage = to_int(floor(rank * (((random(3) + 1) + floor(this_body->query_guild_level("assassin") / 100.0)) / 10.0) * weapon->query_weapon_class())) + weapon->query_weapon_class();

   this_body->targetted_action(weapon->query_backstab_message(), body, weapon);

   this_body->add_event(body, weapon, "torso", damage);

   this_body->start_fight(body);
}

void create()
{
   add_rules( ({ "", "LIV" }) );
}