// Last edited by deforce on 11-10-2009
inherit CONTAINER;
inherit M_DECAY;
inherit M_MESSAGES;

string the_name;
string killed_by;
string killed_with;
object link;
int inspected = 0;
int death_time = 0;

int is_living()
{
   return 0;
}

string in_room_desc()
{
   return query_in_room_desc();
}

mixed ob_state()
{
   return the_name;
}

object query_link()
{
   return link;
}

string query_killer()
{
   return killed_by;
}

string query_killer_weapon()
{
   return killed_with;
}

void mudlib_setup(string name, string long, object l, string killer, string killer_weapon)
{
   ::mudlib_setup();

   link = l;

   if (!name) { return; }

   the_name = name;
   killed_by = killer;
   killed_with = killer_weapon;
   death_time = time();
   set_long(long || "This is the dead body of " + capitalize(the_name) + ".");

   set_id("corpse", "body");

   if (the_name[<1] == 's')
   {
      set_adj("lifeless", the_name);
      set_proper_name(capitalize(the_name) + "' body");
   }
   else
   {
      set_adj("lifeless", the_name + "s");
      set_proper_name(capitalize(the_name) + "'s body");
   }

   set_in_room_desc("corpse of " + capitalize(the_name));
   set_plural_in_room_desc("corpses of " + capitalize(the_name) + ".");
   add_relation("on");
   set_default_relation("on");
   set_max_capacity(300);
   set_mass(59000);
   set_num_decays(1);
   set_last_decay_action("The corpse of " + the_name + " $vdecay into dust and ash.\n");
   set_decay_time(3600);
   set_decay_auto_remove(1);
}

string introduce_contents()
{
   if (sizeof(all_inventory()))
   {
      return "On " + the_short() + " you see:\n";
   }

   return "";
}

mixed indirect_get_obj_from_obj(object ob1, object ob2) { return 1; }
mixed indirect_put_obj_wrd_obj(object ob1, object ob2) { return 1; }

int is_corpse() { return 1; }
int has_been_inspected() { return inspected; }
int query_death_time() { return death_time; }

int query_hide_contents()
{
   return 1;
}

void set_inspected()
{
   inspected = 1;
}

void remove()
{
   foreach (object thing in all_inventory(this_object()))
   {
      thing->move(environment(this_object()));
   }

   destruct();
}