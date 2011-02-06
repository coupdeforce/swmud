// Last edited by deforce on 10-07-2009
#include <flags.h>
#include <move.h>

inherit VERB_OB;

void clear_flag(int);
int test_flag(int);

void do_unconceal_obj(object thing)
{
   if (thing->test_flag(F_CONCEALED) && present(thing, this_body()))
   {
      thing->clear_flag(F_CONCEALED);
      this_body()->simple_action("$N $vpull out " + thing->a_short() + " from the folds of $p clothing.\n");
   }
}

void do_unconceal_obs(array info, object thing)
{
   handle_obs(info, (: do_unconceal_obj :), thing);
}

void do_unconceal()
{
   write("Unconceal what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}