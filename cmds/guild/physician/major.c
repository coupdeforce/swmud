// Last edited by deforce on 03-03-2010
inherit VERB_OB;

void heal(object body);
int all_healthy(object body);

void do_major_liv(object living)
{
   object this_body = this_body();

   if (this_body->query_guild_level("physician"))
   {
      if (!this_body->has_learned_skill("major surgery"))
      {
         write("You have not learned how to perform major surgery.\n");
      }
      else if (this_body->get_target())
      {
         write("You are unable to perform surgery while in combat.\n");
      }
      else if ((living->query_health() >= living->query_max_health()) && all_healthy(living))
      {
         if (living == this_body)
         {
            write("You are already at full health.\n");
         }
         else
         {
            write(living->short() + " is already at full health.\n");
         }
      }
      else
      {
         object array medpacs = ({ });

         foreach (object thing in all_inventory(this_body))
         {
            if (thing->is_surgery_medpac() && (sizeof(medpacs) < 3))
            {
               medpacs += ({ thing });
            }
         }

         if (sizeof(medpacs) < 3)
         {
            foreach (object thing in all_inventory(environment(this_body)))
            {
               if (thing->is_surgery_medpac() && (sizeof(medpacs) < 3))
               {
                  medpacs += ({ thing });
               }
            }
         }

         if (sizeof(medpacs) == 3)
         {
            if (this_body->test_skill("surgery_major", (this_body->query_guild_level("physician") * 8)))
            {
               destruct(medpacs[0]);
               destruct(medpacs[1]);
               destruct(medpacs[2]);
               heal(living);
            }
            else
            {
               if (this_body == living)
               {
                  this_body->my_action("$N $vfail to perform major surgery on $r.");
                  this_body->other_action("$N $vfail to perform major surgery on $r.");
               }
               else
               {
                  this_body->targetted_action("$N $vfail to perform major surgery on $t.", living);
               }
            }
         }
         else
         {
            write("Performing major surgery requires three medpacs.\n");
         }
      }
   }
   else
   {
      write("Only physicians know how to perform major surgery.\n");
   }
}

void do_major()
{
   do_major_liv(this_body());
}

void heal(object body)
{
   object this_body = this_body();
   int rank = to_int(floor(this_body->query_skill("surgery_major") / 100.0));
   int heal_amount = to_int(floor((this_body->query_guild_level("physician")
      + floor(this_body->query_int() / 5) + floor(this_body->query_dex() / 5)) * 2.5))
      - (100 - (rank * 10));

   if (heal_amount < (20 + (rank * 8)))
   {
      heal_amount = 20 + (rank * 8);
   }

   if (this_body == body)
   {
      this_body->my_action("$N $vperform major surgery on $r.");
      this_body->other_action("$N $vperform major surgery on $r.");
   }
   else
   {
      this_body->targetted_action("$N $vperform major surgery on $t.", body);
   }

   body->heal_us(heal_amount);

   foreach (string limb in body->query_limbs())
   {
      if (body->query_limb_health(limb) < body->query_max_health(limb))
      {
         body->heal_limb(limb, heal_amount);
      }
   }
}

int all_healthy(object body)
{
   foreach (string limb in body->query_limbs())
   {
      if (body->query_limb_health(limb) < body->query_max_health(limb))
      {
         return 0;
      }
   }

   return 1;
}

void create()
{
   add_rules( ({ "", "LIV" }) );
}