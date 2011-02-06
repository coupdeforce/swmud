// Last edited by deforce on 08-03-2007
// An object that can be opened, closed, locked, and unlocked.
//
// NOTE: this should only be mixed into classes that have
//       M_OPENABLE, too.
// NOTE: due to a bug in the driver(?), you must inherit M_OPENABLE
//       _after_ inheriting M_LOCKABLE.
//

#include <hooks.h>

string the_short();
void hook_state(string, function, mixed);
mixed call_hooks(string, int);

private string locked, key_type;
string unlock_msg = "$N $vunlock the $o with $p $o1.";
string unlock_fail = "$N $vtry to unlock the $o, but $p $o1 doesn't fit.";
string lock_msg = "$N $vlock the $o with $p $o1.";
string lock_fail = "$N $vtry to lock the $o, but $p $o1 doesn't fit.";
string pick_msg = "$N $vpick open the $o.";
string pick_fail = "$N $vtry to pick open the $o, but $vfail.";
function my_open_hook = (: capitalize(the_short() + " is locked.\n") :);

int is_lockable() { return 1; }
string query_locked() { return locked; }

string query_key_type() { return key_type; }

varargs void set_locked(string x, string y)
{
   locked = x;

   if (nullp(y)) { key_type = x; }
   else { key_type = y; }

   hook_state("prevent_open", my_open_hook, locked);
}

#ifdef USE_SKILLS
int strength_vs_magic;
int strength_vs_picking;

void set_strength_vs_picking(int s)
{
   strength_vs_picking = s;
}

int get_strength_vs_picking()
{
   return strength_vs_picking;
}

#endif

mixed do_pick()
{
   mixed ex;
   int   i;

   if (!locked) { write("It's not locked.\n"); }

   //:HOOK prevent_picking
   //A yes/no/error hook which can prevent an object from being picked
   //open.  However, if your mud uses skills, use set_strength_vs_picking()
   //instead, unless you're doing something complex.

   ex = call_hooks("prevent_picking", HOOK_YES_NO_ERROR);

   if (!ex) { ex = "You can't seem to get it open.\n"; }

   if (stringp(ex))
   {
      write(ex);
      return;
   }

#ifdef USE_SKILLS
   i = this_body()->test_skill("misc/lockpick", get_strength_vs_picking());

   if (!i)
   {
      this_body()->simple_action(pick_fail, this_object());
      return;
   }
#endif

   set_locked(0, "picked");
   this_body()->simple_action(pick_msg, this_object());
}

varargs void do_unlock(object ob)
{
   //:HOOK prevent_unlock
   //A yes/no/error hook which can prevent an object from being unlocked.
   mixed ex = call_hooks("prevent_unlock", HOOK_YES_NO_ERROR);
   if (!ex) { ex = "You can't seem to unlock it.\n"; }

   if (stringp(ex))
   {
      write(ex);
      return;
   }

   if (undefinedp(ob))
   {
      ob = present("key", this_body());
      write("(with " + ob->the_short() + ")\n");
   }

   if (ob->get_key_type() == key_type)
   {
      this_body()->simple_action(unlock_msg, this_object(), ob);
      set_locked(0, key_type);
      hook_state("prevent_open", my_open_hook, locked);
   }
   else
   {
      this_body()->simple_action(unlock_fail, this_object(), ob);
   }
}


varargs void do_lock(object ob)
{
   //:HOOK prevent_lock
   //A yes/no/error hook which can prevent an object from being locked.
   mixed ex = call_hooks("prevent_lock", HOOK_YES_NO_ERROR);
   if (!ex) { ex = "You can't seem to lock it.\n"; }

   if (stringp(ex))
   {
      write(ex);
      return;
   }

   if (undefinedp(ob))
   {
      ob = present("key", this_body());
      write("(with " + ob->the_short() + ")\n");
   }

   if (ob->get_key_type() == key_type)
   {
      this_body()->simple_action(lock_msg, this_object(), ob);
      set_locked(key_type);
   }
   //:FIXME if the lock type is picked or magic, it should probably
   // open.
   else
   {
      this_body()->simple_action(lock_fail, this_object(), ob);
   }
}

/*********************************************************/
/*    verb interactions                                   */
/**********************************************************/

mixed direct_pick_obj(object ob)
{
   if (!locked) { return "It isn't locked."; }

   return 1;
}

mixed direct_unlock_obj(object ob)
{
   if (!locked) { return "It isn't locked.\n"; }

   if (present("key", this_body())) { return 1; }

   return "With what?\n";
}

mixed direct_lock_obj(object ob)
{
   if (!this_object()->query_closed()) { return "Perhaps you should close it first."; }

   if (locked) { return "It is already locked.\n"; }

   if (present("key", this_body())) { return 1; }

   return "With what?\n";
}

mixed direct_unlock_obj_with_obj(object ob1, object ob2)
{
   if (!locked) { return "It isn't locked."; }

   return 1;
}

mixed direct_pick_obj_with_obj()
{
   return 1;
}

mixed direct_lock_obj_with_obj(object ob1, object ob2)
{
   if (!this_object()->query_closed()) { return "Perhaps you should close it first."; }

   if (locked) { return "It is already locked."; }

   return 1;
}