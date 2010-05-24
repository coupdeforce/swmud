// Last modified by deforce on 03-14-2010
inherit VERB_OB;

void do_wear_obj(object ob)
{
   if (!try_to_acquire(ob))
   {
      write("You need to be holding something before trying to wear it.\n");

      return;
   }

   if (!ob->query_slots())
   {
      write("The " + ob->short() + " is not something that can be worn.\n");

      return;
   }

   if (ob->check_required_learned_skills() != 1)
   {
      write("You have not learned how to wear something like that.\n");

      return 0;
   }

   if (ob->check_required_skills() != 1)
   {
      write("You're not skilled enough to know how to wear something like that.\n");

      return 0;
   }

   if (ob->check_required_guilds() != 1)
   {
      write("You have not advanced enough to know how to wear something like that.\n");

      return 0;
   }

   if ((this_body()->query_body_size() - ob->query_body_size()) > 1)
   {
      write(capitalize(ob->the_short()) + " is too small for you to wear.\n");

      return 0;
   }
   else if ((this_body()->query_body_size() - ob->query_body_size()) < -1)
   {
      write(capitalize(ob->the_short()) + " is too large for you to wear.\n");

      return 0;
   }

   if (ob->valid_wear()) { ob->do_wear(); }
}

void do_wear_str(string arg)
{
   if (arg == "all")
   {
      foreach (object thing in all_inventory(this_body()))
      {
         do_wear_obj(thing);
      }
   }
   else
   {
      foreach (object thing in all_inventory(this_body()))
      {
         if (thing->id(arg))
         {
            do_wear_obj(thing);

            return;
         }
      }
   }
}

void create()
{
   add_rules(({ "OBJ", "STR" }), ({ "put on" }));
}