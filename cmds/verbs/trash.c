inherit VERB_OB;

void do_trash_obj(object ob)
{
   if (environment(ob) == this_body())
   {
      write("You trash " + ob->the_short() + ".\n");

      destruct(ob);
   }
}

void do_trash_obs(array info, string name)
{
   handle_obs(info, (: do_trash_obj :));
}

void do_trash()
{
   write("Trash what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}