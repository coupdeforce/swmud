// Last edited by deforce on 04-13-2010
#include <move.h>
#include <hooks.h>

private object last_location;

void call_hooks(string, int);
int query_light();
void force_look(int force_long_desc);

//:FUNCTION contains
//Contains(o) returns 1 if o is in this object, or in an object contained by this object (recursively).
int contains(object o)
{
    return o->is_in(this_object());
}

//:FUNCTION is_in
//is_in(o) returns 1 if this object is in o, or in an object which is in o (recursively).
int is_in(object o)
{
   object env = this_object();

   while(env = environment(env))
   {
      if (env == o) { return 1; }
   }
}

//:FUNCTION move
//move(dest, relation) moves this object to be 'relation' ("in", "on", etc) to the object dest, if possible.
//If it fails, it returns a string //error message or zero; if it succeeds it returns 1.
varargs mixed move(mixed dest, string relation)
{
   object env;
   string oldrelation;
   object tmp;
   mixed ret;
   mixed err;
   int light;

   if (environment())
   {
      env = environment();
      oldrelation = env->query_relation(this_object());

      //:HOOK prevent_drop
      //A yes/no/error type hook that can be used to prevent the object from being
      //moved out of it's environment.  The error value is discarded.
      err = call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
      if ((err == 0) || (this_object()->is_attached())) { err = MOVE_PREVENTED; }
      if (stringp(err)) { return err; }
   }

   if (stringp(dest))
   {
      dest = load_object(dest);
   }

   if (!objectp(dest)) { return MOVE_NO_DEST; }

   if (this_object()->query_stunned()) { return MOVE_STUNNED; }

   if (dest->is_in(this_object()))
   {
      return "You can't move an object inside itself.\n";
   }

   if (dest->is_carry_container() && this_object()->is_carry_container())
   {
      return "You can't put " + this_object()->a_short() + " inside " + dest->a_short() + ".\n";
   }

   if (env)
   {
      ret = env->release_object(this_object());

      if (ret == 0) { ret = MOVE_NOT_RELEASED; }

      if (stringp(ret)) { return ret; }
   }

   ret = dest->receive_object(this_object(), relation);

   if (ret == 0) { ret = MOVE_NOT_RECEIVED; }

   if (stringp(ret))
   {
      if (env)
      {
         env->reinsert_object(this_object(), oldrelation);
         env->update_capacity();
      }

      return ret;
   }

   last_location=env;

   if ((light = query_light()) && env)
   {
      env->containee_light_changed(-light);
   }

   move_object(dest);

   //:HOOK move
   //Called when an object moves.  The return value is ignored.
   call_hooks("move", HOOK_IGNORE);

   // requery light just in case a hook changed it (e.g. a sword stopped glowing when released)
   if ((light = query_light()) && dest)
   {
      dest->containee_light_changed(light);
   }

   // doing things (like desting the object) above in receive_object would be bad.
   // Give the destination a chance to do what it wants

   //:HOOK object_left
   //Called when an object leaves an that object.  The return value is ignored.
   //The object moving is passed as the first arg.

   //:HOOK object_arrived
   //Called when an object arrives in an object.  The return value is ignored.
   //The object moving is passed as the first arg.

   if (env)
   {
      env->call_hooks("object_left", HOOK_IGNORE, 0, this_object());
   }

   dest->call_hooks("object_arrived", HOOK_IGNORE, 0, this_object());

   return MOVE_OK;
}

//### IMO this is only used for hacks, and should be removed (-Beek)
//:FUNCTION query_last_location
//Returns the last object an object was inside before it moved to where
//it is now.
object query_last_location()
{
   return last_location;
}

//:FUNCTION give_new_obj
//give_new_obj creates a new object and moves it to target. If recurse is 1
//the object will be moved to the environment if the move fails.
//The optional args are passed to the object when it is created.
varargs object give_new_obj(object target, string obj, int recurse, mixed args)
{
   object ob;

   if (undefinedp(args))
   {
      ob = new(absolute_path(obj));
   }
   else if (arrayp(args))
   {
      ob = new(absolute_path(obj), args...);
   }
   else
   {
      ob = new(absolute_path(obj), args);
   }

   if (!ob)
   {
      error("give_new_obj() failed: Object could not be created: " + obj + ".\n");
   }

   if (!target)
   {
      error("give_new_obj() failed: Invalid target.");
   }

   while(1)
   {
      if (ob->move(target) == MOVE_OK)
      {
         return ob;
      }

      if (recurse && (target = environment(target)))
      {
         continue;
      }

      error("give_new_obj() failed: No room for object anywhere (recursed).");
   }
}