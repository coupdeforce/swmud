// Last edited by deforce on 02-22-2010
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
            return "Buy from which vendor?\n";
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

mixed can_buy_str()
{
   return test_vendor();
}

void do_buy(string arg, object vendor)
{
   int listnum = 0, number = 1;
   string *args = explode(arg, " ");

   if (sscanf(args[<1], "#%d", listnum))
   {
      if (listnum < 0)
      {
         write("No negative numbers please!\n");
         return;
      }

      args = args[0..<2];
   }

   if (sizeof(args) > 0 && sscanf(args[0], "%d", number))
   {
      args = args[1..<1];
   }

   if (sizeof(args) > 0)
   {
      arg = implode(args, " ");
   }
   else
   {
      arg = "all";
   }

   vendor->sell_stored_objects_to(arg, listnum, number, this_body());
}

void do_buy_str(string arg)
{
   do_buy(arg, find_vendor());
}

void do_buy_str_from_liv(string arg, object liv)
{
   do_buy(arg, liv);
}

void create()
{
   add_rules( ({ "STR", "STR from LIV" }) );
}