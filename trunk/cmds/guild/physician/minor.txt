// Last edited by deforce on 03-03-2010
inherit VERB_OB;

object medpac;

void heal(object body);
int all_healthy(object body);

void do_minor_liv(object living)
{
   object this_body = this_body();

   if (this_body->query_guild_level("physician"))
   {
      if (!this_body->has_learned_skill("minor surgery"))
      {
         write("You have not learned how to perform minor surgery.\n");
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
         foreach (object thing in all_inventory(this_body))
         {
            if (thing->is_surgery_medpac())
            {
               medpac = thing;
               break;
            }
         }

         if (!medpac)
         {
            foreach (object thing in all_inventory(environment(this_body)))
            {
               if (thing->is_surgery_medpac())
               {
                  medpac = thing;
                  break;
               }
            }
         }

         if (medpac)
         {
            if (this_body->test_skill("surgery_minor", (this_body->query_guild_level("physician") * 16)))
            {
               destruct(medpac);
               heal(living);
            }
            else
            {
               if (this_body == living)
               {
                  this_body->my_action("$N $vfail to perform minor surgery on $r.");
                  this_body->other_action("$N $vfail to perform minor surgery on $r.");
               }
               else
               {
                  this_body->targetted_action("$N $vfail to perform minor surgery on $t.", living);
               }
            }
         }
         else
         {
            write("Performing minor surgery requires a medpac.\n");
         }
      }
   }
   else
   {
      write("Only physicians know how to perform minor surgery.\n");
   }
}

void do_minor()
{
   do_minor_liv(this_body());
}

void heal(object body)
{
   object this_body = this_body();
   int rank = to_int(floor(this_body->query_skill("surgery_minor") / 100.0));
   int heal_amount = this_body->query_guild_level("physician")
      + to_int(floor(this_body->query_int() / 5)) + to_int(floor(this_body->query_dex() / 5))
      - (50 - (rank * 5));

   if (heal_amount < (10 + (rank * 4)))
   {
      heal_amount = 10 + (rank * 4);
   }

   if (this_body == body)
   {
      this_body->my_action("$N $vperform minor surgery on $r.");
      this_body->other_action("$N $vperform minor surgery on $r.");
   }
   else
   {
      this_body->targetted_action("$N $vperform minor surgery on $t.", body);
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