// Last edited by deforce on 04-06-2010
// Verpine racial skill
#include <flags.h>

inherit VERB_OB;

void test_flag(int);
void adjust(object thing);

void do_adjust_obj(object thing)
{
   object this_body = this_body();

   if (thing->is_livng())
   {
      if (thing != this_body)
      {
         write("If you want to adjust " + thing->short() + ", you could try installing implants instead.\n");
      }
      else
      {
         write("If you want to adjust yourself, you could try installing implants instead.\n");
      }
   }
   else if (!thing->test_flag(F_ADJUSTED))
   {
      this_body->add_skill_delay(8);

      if (this_body->test_skill("adjust", this_body->query_primary_level() * 10))
      {
         adjust(thing);
      }
      else
      {
         this_body->simple_action("$N $vattempt to adjust $o and $vfail, damaging it slightly.", thing);

         if (thing->is_armor() || thing->is_weapon())
         {
            thing->decrease_class(1);
         }
      }
   }
   else
   {
      write("The " + thing->short() + " has already been adjusted.\n");
   }
}

void do_adjust()
{
   write("Adjust what?\n");
}

mixed can_adjust_obj()
{
   object this_body = this_body();

   if (this_body->query_race() == "verpine")
   {
      if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on adjusting something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to adjust somethng while in combat.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are unable to adjust something while paralyzed.\n";
      }
   }
   else
   {
      return "Only Verpine can adjust something.\n";
   }

   return 1;
}

mixed can_adjust()
{
   if (this_body()->query_race() != "verpine")
   {
      return "Only Verpine can adjust something.\n";
   }

   return 1;
}

void adjust(object thing)
{
   int skill = this_body()->query_skill("adjust");
   int rank = skill / 100;
   int amount = (random(skill) == 0 ? -1 : 0) + (skill > random(100) ? 1 : 0) + ((rank * 10) > random(100) ? 1 : 0);

   if (amount < 0) { amount = 0; }

   this_body()->other_action("$N $vstudy $o for a moment, and adjust things in various places.", thing);

   thing->do_adjust(amount);
}

void create()
{
   add_rules( ({ "", "OBJ" }) );
}