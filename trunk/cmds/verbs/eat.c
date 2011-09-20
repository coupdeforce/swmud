#include <verbs.h>

inherit VERB_OB;

void do_eat_obj(object ob)
{
   mixed err = try_to_acquire(ob);

   if (stringp(err))
   {
      write(err);

      return;
   }

   if (err==1)
   {
      ob->do_eat_obj();
   }
}

void create()
{
   add_flag(TRY_TO_ACQUIRE);

   add_rules( ({ "OBJ" }) );
}