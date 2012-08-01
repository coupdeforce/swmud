inherit VERB_OB;

void do_smash_obj(object ob)
{
   ob->do_smash();
}

void do_smash_obs(array info, string name)
{
   handle_obs(info, (: do_smash_obj :));
}

void do_smash()
{
   write("Smash what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBS" }) );
}