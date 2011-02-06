#include <flags.h>

inherit VERB_OB;

void do_uninstall_obj_from_obj(object ob1, object ob2)
{
   if (this_body()->query_target())
   {
      write("You are unable to uninstall a component while in combat.\n");
      return;
   }
   else if (ob2->test_flag(F_WIELDED))
   {
      write("You must unwield " + ob2->the_short() + " before uninstalling a component.\n");
      return;
   }
   else if (ob2->test_flag(F_WORN))
   {
      write("You must remove " + ob2->the_short() + " before uninstalling a component.\n");
      return;
   }

   if ((ob1->is_lightsaber_component() && ob2->is_lightsaber())
      || (ob1->is_blade_component() && ob2->is_blade())
      || (ob1->is_blaster_component() && ob2->is_blaster())
      || (ob1->is_armor_component() && ob2->is_modifiable_armor()))
   {
      ob1->move(environment(ob2));
      ob2->assemble();

      this_body()->simple_action("$N $vuninstall a $o from a $o1.", ob1, ob2);

      return;
   }

   write("It doesn't seem possible to uninstall " + ob1->the_short() + " from " + ob2->the_short() + ".\n");
}

void do_uninstall_obs_from_obj(array info, object ob2)
{
   handle_obs(info, (: do_uninstall_obj_from_obj :), ob2);
}

void do_uninstall_obj(object ob)
{
   write("Uninstall " + ob->the_short() + " from what?\n");
}

void do_uninstall()
{
   write("Uninstall what from what?\n");
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ from OBJ", "OBS from OBJ" }) );
}