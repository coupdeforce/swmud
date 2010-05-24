inherit VERB_OB;

void do_use_obj_on_liv(object thing, object living)
{
   thing->do_use_on_liv(living);
}

void do_use_obj(object thing)
{
   thing->do_use();
}

void do_use()
{
   write("What do you want to use?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ on LIV" }) );
}