// Last modified by deforce on 10-01-2008
inherit VERB_OB;

void do_check_obj(object ob)
{
   ob->do_check_obj();
}

void do_check()
{
   write("Check what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ" }) );
}