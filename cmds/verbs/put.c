// Last edited by deforce on 10-21-2009
#include <flags.h>
#include <move.h>

inherit VERB_OB;

void do_put_obj_wrd_obj(object ob1, string p, object ob2)
{
   mixed tmp;
   string prep;

   if (ob2->is_living())
   {
      write("Try giving it.\n");
      return;
   }

   if (!try_to_acquire(ob1))
   {
      return;
   }

   prep = PREPOSITION_D->translate_preposition(p);
   tmp = ob1->move(ob2, prep);

   if (tmp == MOVE_OK)
   {
      if (ob1->test_flag(F_CONCEALED))
      {
         this_body()->my_action("$N $vput a $o " + prep + " a $o1.", ob1, ob2);
         this_body()->other_action("$N $vput something " + prep + " a $o.", ob2);
         ob1->clear_flag(F_CONCEALED);
      }
      else
      {
         this_body()->simple_action("$N $vput a $o " + prep + " a $o1.", ob1, ob2);
      }

      return;
   }

   if (!tmp)
   {
      tmp = "That doesn't seem possible.\n";
   }

   write(tmp);
}

void do_put_obs_wrd_obj(array info, string p, object ob2)
{
   handle_obs(info, (: do_put_obj_wrd_obj :), p, ob2);
}

void create()
{
   add_rules( ({ "OBS WRD OBJ" }) );
}