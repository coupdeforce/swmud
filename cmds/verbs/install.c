#include <flags.h>

inherit VERB_OB;

void do_install_obj_in_obj(object ob1, object ob2)
{
   if (this_body()->query_target())
   {
      write("You are unable to install a component while in combat.\n");
      return;
   }
   else if (ob2->test_flag(F_WIELDED))
   {
      write("You must unwield " + ob2->the_short() + " before installing a component.\n");
      return;
   }
   else if (ob2->test_flag(F_WORN))
   {
      write("You must remove " + ob2->the_short() + " before installing a component.\n");
      return;
   }

   if ((ob1->is_lightsaber_component() && ob2->is_lightsaber())
      || (ob1->is_blade_component() && ob2->is_blade())
      || (ob1->is_blaster_component() && ob2->is_blaster())
      || (ob1->is_armor_component() && ob2->is_modifiable_armor()))
   {
      mixed test = ob1->move(ob2);

      if (stringp(test))
      {
         write(test);
         return;
      }

      ob2->assemble();

      if (ob1->test_flag(F_CONCEALED))
      {
         this_body()->my_action("$N $vinstall a $o in a $o1.", ob1, ob2);
         this_body()->other_action("$N $vinstall something in a $o.", ob2);
         ob1->clear_flag(F_CONCEALED);
      }
      else
      {
         this_body()->simple_action("$N $vinstall a $o in a $o1.", ob1, ob2);
      }

      return;
   }

   write("It doesn't seem possible to install " + ob1->a_short() + " in " + ob2->the_short() + ".\n");
}

void do_install_obj_on_obj(object ob1, object ob2)
{
   do_install_obj_in_obj(ob1, ob2);
}

void do_install_obs_in_obj(array info, object ob2)
{
   handle_obs(info, (: do_install_obj_in_obj :), ob2);
}

void do_install_obs_on_obj(array info, object ob2)
{
   handle_obs(info, (: do_install_obj_in_obj :), ob2);
}

void do_install_obj(object ob)
{
   write("Install " + ob->the_short() + " in what?\n");
}

void do_install()
{
   write("Install what in what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ in OBJ", "OBJ on OBJ", "OBS in OBJ", "OBS on OBJ" }) );
}