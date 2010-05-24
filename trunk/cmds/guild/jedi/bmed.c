// Last edited by deforce on 03-23-2010

inherit CMD; 

void concentration();

void main()
{
   object this_body = this_body();

   if (this_body->query_guild_level("jedi"))
   {
      if (!this_body->has_learned_skill("battle meditation"))
      {
         out("You have not learned how to perform battle meditation.\n");
      }
      else if (this_body->has_blocked_skill("battle meditation"))
      {
         out("Your ability to use this command has been disabled.\n");
      }
      else if (this_body->has_skill_delay())
      {
         out("You are too busy to concentrate on battle meditation.\n");
      }
      else if (this_body->has_special_skill_delay("battle meditation"))
      {
         write("You need to wait " + convert_time(this_body->has_special_skill_delay("battle meditation")) + " before you can perform battle meditation.\n");
      }
      else
      {
         concentration();
      }
   }
   else
   {
      out("Only Jedi know how to perform battle meditation.\n");
   }
}

void concentration()
{
   object this_body = this_body();
   int alignment = this_body->query_jedi_alignment();
   object array targets = ({ });

   if (alignment > 0)
   {
      targets = filter_array(all_inventory(environment(this_body)), (: $1->is_body() :)) - this_body->query_targets();
   }
   else
   {
      targets = this_body->query_targets();

      if (!sizeof(targets) || !targets[0])
      {
         tell(this_body, "You don't have any targets to weaken at the moment.\n");

         return;
      }
   }

   this_body->add_skill_delay(8);

   if (this_body->test_skill("bmed", (alignment > 0 ? alignment * 10 : alignment * -10)))
   {
      int force = this_body->query_for();
      int level = this_body->query_guild_level("jedi");
      int rank = this_body->query_skill("bmed") / 100;
      int to_hit = (rank * 5) + level + force;
      int skill_adjustment = 10 + (12 * rank) + ((level / 5) * 12);
      int damage_adjustment = floor(((level / 5) + rank) * (force / 150.0));
      int delay = 300 - (4 * rank) - ((((level / 5) * 4) + ((force / 5) * 2)));
      int duration = 4 + (4 * rank) + (floor((level / 50.0) * (force / 100.0) * 38) * 2);
      string array weapon_skills;

      load_object("/d/buffs/battle_meditation");

      if (alignment > 0)
      {
         tell(this_body, "You focus your concentration on battle, boosting your abilites" + (sizeof(targets) > 1 ? " and those of your allies" : "") + ".\n");
         this_body->other_action("$N focuses $p concentration on battle, boosting $p abilites" + (sizeof(targets) > 1 ? " and those of $p allies" : "") + ".\n");
      }
      else
      {
         tell(this_body, "Your enemies weaken before you as you concentrate on the battle.\n");
         this_body->other_action("$N focuses $p concentration on battle, weakening the abilites of $p enemies.\n");
      }

      foreach (object target in targets)
      {
         weapon_skills = ({ });

         foreach (object weapon in target->query_weapons())
         {
            if (member_array(weapon->query_skill_used(), weapon_skills) == -1)
            {
               weapon_skills += ({ weapon->query_skill_used() });
            }
         }

         if (alignment > 0)
         {
            target->add_buff(new("/d/buffs/battle_meditation", to_hit, skill_adjustment, weapon_skills, damage_adjustment, duration));
         }
         else
         {
            target->add_buff(new("/d/buffs/battle_meditation", to_hit * -1, skill_adjustment * -1, weapon_skills, damage_adjustment * -1, duration));
         }
      }

      this_body->add_special_skill_delay("battle meditation", (300 - (rank * 4) - ((level / 5) * 4) - ((force / 5) * 2)));
   }
   else
   {
      tell(this_body, "You fail to concentrate on the battle.\n");
   }
}