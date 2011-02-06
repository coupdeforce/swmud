// Last edited by deforce on 03-04-2010
#include <flags.h>
#include <move.h>

varargs void simple_action(mixed msg, array obs...);

protected nosave mixed vehicle;

void set_driving_vehicle(object thing)
{
   if (!thing)
   {
      vehicle = 0;
   }
   else
   {
      vehicle = thing;
   }
}

mixed query_vehicle()
{
   return vehicle;
}

mixed is_driving_vehicle()
{
   if (vehicle)
   {
      return 1;
   }

   return 0;
}

void drop_object(object ob)
{
   mixed tmp = ob->drop();
   int was_concealed = 0;

   ob->do_remove();
   ob->do_unwield();

   if (ob->test_flag(F_CONCEALED))
   {
      was_concealed = 1;
      ob->clear_flag(F_CONCEALED);
   }

   if (!tmp) tmp = "You aren't able to drop it.\n";

   if (stringp(tmp))
   {
      write(tmp);
      return tmp;
   }

   tmp = ob->move(environment(this_body()), environment(this_body())->query_relation(this_body()));

   if (tmp == MOVE_OK)
   {
      if (ob)
      {
         if (was_concealed)
         {
            simple_action("$N $vpull out " + ob->a_short() + " from the folds of $p clothing.\n");
            simple_action("$N $vdrop $o.", ob);
         }
         else
         {
            simple_action("$N $vdrop $o.", ob);
         }
      }
      else
      {
         write("Done.\n");
      }
   }
   else
   {
      write(tmp);
   }
}