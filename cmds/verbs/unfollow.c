inherit VERB_OB;

mixed direct_unfollow_obj(object ob)
{
   if (!present(ob, environment(this_body())))
   {
      this_body()->my_action("$N $vis not here.", ob);
      return 0;
   }

   return 1;
}

void do_unfollow_obj(object ob)
{
   if (this_body()->is_potential_follow(ob))
   {
      this_body()->do_follow_obj(ob);
      return;
   }

   write("You are not following " + ob->short() + ".\n");
}

mixed map_func(mixed element)
{
   if (objectp(element))
   {
      return element->short();
   }

   if (stringp(element))
   {
      return element;
   }

   return "";
}

void do_unfollow_str(string str)
{
   string array follows = this_body()->query_follow_search();

   if (member_array(str, flatten_array(follows->query_id())) == -1)
   {
      write("You are not following \"" + str + "\".\n");
      return;
   }

   foreach (object thing in follows)
   {
      if (thing->id(str))
      {
         this_body()->remove_follow_search(({ thing }));

         write("You stop following " + thing->short() + ".\n");
      }
      else
      {
         write(thing->short() + " does not respond to \"" + str + "\".\n");
      }
   }
}

void do_unfollow()
{
   string array follows = map_array(this_body()->query_follow_search(), (: map_func :));
   follows -= ({ "" });

   if (!sizeof(follows))
   {
      write("You are not following anything.\n");
      return;
   }

   if (sizeof(follows) == 1)
   {
      write("Unfollow " + follows[0] + "?\n");
      return;
   }

   write("Unfollow whom? " + implode(follows[0..<2], ", ") + " or " + follows[<1] +  "?\n");
}

void create()
{
   add_rules( ({ "", "OBJ" }) );
   add_rules( ({ "STR" }) );
}