// Last edited by deforce on 10-15-2009
#include <flags.h>
#include <move.h>

inherit VERB_OB;

void assign_flag(int which, int state);
void test_flag(int);
void hide(object thing);
int get_duration();

void do_hide_obj(object thing)
{
   if (!thing->test_flag(F_CONCEALED) && !thing->is_attached())
   {
      object this_body = this_body();

      if (present(thing, this_body()))
      {
         this_body->add_skill_delay(2);

         if (this_body->test_skill("hide", (this_body->query_guild_level("smuggler") * 10)))
         {
            hide(thing);
         }
         else
         {
            write("You fail to hide " + thing->the_short() + " in the shadows.\n");
            this_body()->other_action("$N $vattempt to hide something in the shadows, and fails.\n");
         }
      }
   }
}

void do_hide_obs(array info, object thing)
{
   handle_obs(info, (: do_hide_obj :), thing);
}

void do_hide()
{
   object this_body = this_body();

   this_body->add_skill_delay(2);

   if (this_body->test_skill("hide", (this_body->query_guild_level("smuggler") * 10)))
   {
      load_object("/d/buffs/hide");
      this_body->add_buff(new("/d/buffs/hide", get_duration()));
   }
   else
   {
      write("You fail to hide in the shadows.\n");
      this_body()->other_action("$N $vattempt to hide in the shadows, and fails.\n");
   }
}

void hide(object thing)
{
   write("You hide " + thing->the_short() + " in the shadows.\n");

   if (thing->move(environment(this_body())))
   {
      thing->assign_flag(F_HIDDEN, 1);
   }
}

mixed can_hide_obj()
{
   object this_body = this_body();

   if (this_body->query_guild_level("smuggler"))
   {
      if (!this_body->has_learned_skill("hiding"))
      {
         return "You have not learned how to hide something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to hide somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on hiding something.\n";
      }
   }
   else
   {
      return "Only smugglers know how to hide something.\n";
   }

   return 1;
}

mixed can_hide()
{
   object this_body = this_body();

   if (this_body->query_guild_level("smuggler"))
   {
      if (!this_body->has_learned_skill("hiding"))
      {
         return "You have not learned how to hide in the shadows.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to hide in the shadows while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on hiding in the shadows.\n";
      }
      else if (this_body->has_buff("/d/buffs/hide"))
      {
         return "You are already hidden in the shadows.\n";
      }
      else if (this_body->test_flag(F_INVIS))
      {
         return "You are already hidden in some manner.\n";
      }
   }
   else
   {
      return "Only smugglers know how to hide in the shadows.\n";
   }

   return 1;
}

int get_duration()
{
   object this_body = this_body();
   int level = this_body->query_guild_level("smuggler");
   int rank = this_body->query_skill("hide") / 100;

   return 20 + ((level / 5) * 12) + (rank * 16);
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}