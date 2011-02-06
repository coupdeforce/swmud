// Last edited by deforce on 12-15-2007
inherit VERB_OB;

void do_testtest()
{
   this_body()->simple_action("$N $vdo something.");
}

void create()
{
   add_rules( ({ "" }) );
}