inherit VERB_OB;

void do_gather_wrd_wrd(string type, string word)
{
   if (word == "seed")
   {
      if (this_body()->query_race() == "ithorian")
      {
         write("You gather a " + type + " seed.\n");
      }
      else
      {
         write("Only Ithorians know how to gather seeds.\n");
      }
   }
   else
   {
      write("Gather what?\n");
   }
}

void do_gather_wrd(string word)
{
   if (word == "seed")
   {
      if (this_body()->query_race() == "ithorian")
      {
         string type = environment(this_body())->gather_random_seed();

         if (type != "none")
         {
            write("You gather a " + type + " seed.\n");

            environment(this_body())->add_seed(type);
         }
         else
         {
            write("There are no seeds here for you to gather.\n");
         }
      }
      else
      {
         write("Only Ithorians know how to gather seeds.\n");
      }
   }
   else
   {
      write("Gather what?\n");
   }
}

void do_gather()
{
   write("Gather what?\n");
}

void create()
{
   add_rules( ({ "", "WRD", "WRD WRD" }) );
}