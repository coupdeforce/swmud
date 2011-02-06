// Last edited by deforce on 10-23-2009
#include <flags.h>
#include <move.h>

inherit VERB_OB;

void assign_flag(int which, int state);
int test_flag(int);
void conceal(object thing);

void do_conceal_obj(object thing)
{
   if (!thing->test_flag(F_CONCEALED) && !thing->is_attached() && !thing->test_flag(F_LIGHTED)
      && !thing->test_flag(F_WORN) && !thing->test_flag(F_WIELDED))
   {
      object this_body = this_body();

      if (present(thing, this_body()))
      {
         this_body->add_skill_delay(2);

         if (this_body->test_skill("hide", (this_body->query_guild_level("smuggler") * 10)))
         {
            conceal(thing);
         }
         else
         {
            write("You fail to conceal " + thing->the_short() + " within the folds of your clothing.\n");
            this_body()->other_action("$N $vattempt to slip $o inside $p clothes, and fails.\n", thing);
         }
      }
   }
}

void do_conceal_obs(array info, object thing)
{
   handle_obs(info, (: do_conceal_obj :), thing);
}

void do_conceal()
{
   string list = inv_list_plain(filter_array(all_inventory(this_body()), (: $1->test_flag(F_CONCEALED) :) ));

   if (strlen(list))
   {
      write("You are concealing the following items:\n" + list);
   }
   else
   {
      write("You have nothing concealed.\n");
   }
}

void conceal(object thing)
{
   write("You conceal " + thing->the_short() + " within the folds of your clothing.\n");
   this_body()->other_action("$N $vslip something inside $p clothes.\n");

   thing->assign_flag(F_CONCEALED, 1);
}

mixed can_conceal_obj(object thing)
{
   object this_body = this_body();

   if (this_body->query_guild_level("smuggler"))
   {
      if (!this_body->has_learned_skill("concealing"))
      {
         return "You have not learned how to conceal objects.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to conceal somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on concealing something.\n";
      }
   }
   else
   {
      return "Only smugglers know how to conceal objects within their clothing.\n";
   }

   return 1;
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}