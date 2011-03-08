inherit VERB_OB;

void do_recharge_obj_with_obj(object ob1, object ob2)
{
   if (!ob1->requires_ammo())
   {
      write("The " + ob1->short() + " doesn't need to be recharged.\n");
      return;
   }
   else if (!ob2->can_recharge(ob1))
   {
      write("The " + ob2->short() + " cannot be used to recharge a " + ob1->short() + ".\n");
      return;
   }

   ob2->on_recharge(ob1);

   this_body()->simple_action("$N $vrecharge the $o.", ob1);
}

void do_recharge_obs_with_obj(array info, object ob2)
{
   handle_obs(info, (: do_recharge_obj_with_obj :), ob2);
}

void do_recharge()
{
   write("Recharge what with what?\n");
}

void do_recharge_obj(object ob)
{
   write("Recharge " + ob->short() + " with what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ with OBJ", "OBS with OBJ" }) );
}