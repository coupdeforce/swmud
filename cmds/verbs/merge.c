inherit VERB_OB;

void do_merge_obj_with_obj(object ob1, object ob2)
{
   if (ob1 == ob2)
   {
      write("It's not possible to merge something with itself.\n");
      return;
   }

   if (!ob1 || !ob2)
   {
      write("One of the objects for the merge does not exist.\n");
      return;
   }

   if (ob1->is_medpac() && ob2->is_medpac())
   {
      int bacta1 = ob1->query_bacta_left();
      int bacta2 = ob2->query_bacta_left();

      if ((bacta1 > 0) || (bacta2 > 0))
      {
         int bacta_taken = ob1->use_bacta(bacta1);

         ob2->add_bacta(bacta_taken);

         this_body()->simple_action("$N $vmerge bacta from two medpacs.");
      }

      if (ob1->query_bacta_left() < 1)
      {
         destruct(ob1);
         this_body()->simple_action("$N $vdiscard an empty medpac.");
      }

      if (ob2->query_bacta_left() < 1)
      {
         destruct(ob2);
         this_body()->simple_action("$N $vdiscard an empty medpac.");
      }

      return;
   }
   else if (ob1->is_toolbox() && ob2->is_toolbox())
   {
      mapping parts1 = ob1->query_parts_list();
      mapping parts2 = ob2->query_parts_list();

      if (sizeof(parts1) || sizeof(parts2))
      {
         int parts_taken;

         foreach (string type in keys(parts1))
         {
            parts_taken = ob1->use_parts(type, parts1[type]);
            ob2->add_parts(type, parts_taken);
         }

         this_body()->simple_action("$N $vmerge parts from two toolboxes.");
      }

      if (!sizeof(ob1->query_parts_list()))
      {
         destruct(ob1);
         this_body()->simple_action("$N $vdiscard an empty toolbox.");
      }

      if (!sizeof(ob2->query_parts_list()))
      {
         destruct(ob2);
         this_body()->simple_action("$N $vdiscard an empty toolbox.");
      }

      return;
   }

   write("It doesn't seem possible to merge the contents of " + ob1->a_short() + " with the contents of " + ob2->a_short() + ".\n");
}

void do_merge_obj(object ob)
{
   write("Merge " + ob->short() + " with what?\n");
}

void do_merge_obs(array info)
{
   if (sizeof(info) > 1)
   {
      object array groups = ({ });

      while (sizeof(info))
      {
         object array group = ({ });
         string base = base_name(info[0]);

         foreach (object thing in info)
         {
            if (base_name(thing) == base)
            {
               group += ({ thing });
               info -= ({ thing });
            }
         }

         if (sizeof(group) > 1)
         {
            groups += ({ group });
         }
      }

      foreach (object array group in groups)
      {
         if (sizeof(group) > 1)
         {
            while (sizeof(group) > 1)
            {
               if (!group[1])
               {
                  group -= ({ group[1] });
               }

               if (!group[0])
               {
                  group -= ({ group[0] });
               }

               if (sizeof(group) > 1)
               {
                  do_merge_obj_with_obj(group[0], group[1]);

                  group -= ({ group[0] });
               }
            }
         }
      }

      return;
   }

   write("You appear to have only one " + info[0]->short() + ".\n");
}

void do_merge()
{
   write("Merge what with what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ with OBJ", "OBS" }) );
}