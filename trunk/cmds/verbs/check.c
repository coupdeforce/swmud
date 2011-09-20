inherit VERB_OB;

void do_check_obj(object ob)
{
   ob->do_check_obj();
}

void do_check_obs(array info, string name)
{
   handle_obs(info, (: do_check_obj :));
}

void do_check()
{
   write("Check what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}