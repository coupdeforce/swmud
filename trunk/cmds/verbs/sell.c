// Last edited by deforce on 02-24-2010
inherit VERB_OB;

mixed test_vendor()
{
   object ob, vendor;

   foreach (ob in all_inventory(parser_root_environment(environment(this_body()))))
   {
      if (ob->query_vendor())
      {
         if (vendor)
         {
            return "Sell to which vendor?\n";
         }

         vendor = ob;
      }
   }

   if (!vendor)
   {
      return "There is no vendor here.\n";
   }

   return 1;
}

object find_vendor()
{
   foreach (object ob in all_inventory(parser_root_environment(environment(this_body()))))
   {
      if (ob->query_vendor())
      {
         return ob;
      }
   }

   // can't happen, because test_vendor() was called already
   return 0;
}

mixed can_sell_obj()
{
   return test_vendor();
}

void do_sell_obj_to_liv(object ob, object vendor)
{
   if (!try_to_acquire(ob))
   {
      return;
   }

   vendor->buy_object_from(ob, this_body());
}

void do_sell_obs_to_liv(array info, object thing, object vendor)
{
   handle_obs(info, (: do_sell_obj_to_liv :), thing, vendor);
}

void do_sell_obj(object ob)
{
   if (!try_to_acquire(ob))
   {
      return;
   }

   find_vendor()->buy_object_from(ob, this_body());
}

void do_sell_obs(array info, object thing)
{
   handle_obs(info, (: do_sell_obj :), thing);
}

void create()
{
   add_rules( ({ "OBJ to LIV", "OBS to LIV", "OBJ", "OBS" }) );
}