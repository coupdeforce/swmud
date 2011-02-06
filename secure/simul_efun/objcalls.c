#include <commands.h>

object this_body();
object find_body(string);
string evaluate_path(string);
varargs void tell_environment(object, string);

string base_name(mixed val)
{
   if (nullp(val))
   {
      val = previous_object();
   }

   if (objectp(val))
   {
      val = file_name(val);
   }
   else if (!stringp(val))
   {
      return val;
   }

   sscanf(val, "%s#%*d", val);

   return val;
}

#include <setbit.h>
#include <driver/origin.h>

// return number of objects w/ primary id of previous_object()
// in the environment of previous_object()

// changed by Beek to be objects identical to previous_object()
// since otherwise things get put in groups they don't belong in.
// red sword + blue sword -> 2 red swords.

int compare_objects(object o1, object o2)
{
   return ((base_name(o1) == base_name(o2))
      && (o1->ob_state() == o2->ob_state())
      && (o1->get_attributes() == o2->get_attributes())
      && (o1->short() == o2->short())
      && (o1->query_value() == o2->query_value())
      && ((int)o2->ob_state() != -1));
}

varargs int count(object o)
{
   int num;
   object *obs;
   int i;

   if (!o)
   {
      if (origin() == ORIGIN_LOCAL)
      {
         o = this_object();
      }
      else
      {
         o = previous_object();
      }
   }

   if (!objectp(environment(o)))
   {
      return 1;
   }

   obs = all_inventory(environment(o));

   for (i=0; i<sizeof(obs); i++)
   {
      if (compare_objects(obs[i], o))
      {
         num++;
      }
   }

   return num;
}

// returns 1 for all identical objects in the environment except the first one.
varargs int duplicatep(object o)
{
   int i;
   object *obs;

   if (!o)
   {
      o = previous_object();
   }

   obs = all_inventory(environment(o));

   for (i=0; i<sizeof(obs); i++)
   {
      if (obs[i]==o) { return 0; }
      if (compare_objects(obs[i], o)) { return 1; }
   }
}

//### The member_array(item, users()->query_body()); crap is to workaround
//### a parser bug in MudOS v22.2a21.
mixed deep_useful_inv_parser_formatted(object ob)
{
    mixed ret = ({});
    object *inv;
    object *next_inv;

    if(!ob->inventory_accessible()) return 0;
    inv = all_inventory(ob);
    if(!sizeof(inv)) return 0;
    foreach(object item in inv)
    {
        next_inv = deep_useful_inv_parser_formatted(item);
        if(!next_inv)
        {
            // ### When the parser bug is fixed, remove the following line
//          if (member_array(item, users()->query_body()) == -1)
                ret += ({ item });
        }
        else
        {
            // ### When the parser bug is fixed remove the if test
            // ### and ret += ({ next_inv });
//          if (member_array(item, users()->query_body()) != -1)
//              ret += ({ next_inv });
//          else
                ret += ({ item, next_inv });
        }
    }
    return ret;
}
mixed deep_useful_inv( object ob )
{
    object *ret;
    int i,n;

    if( ob->inventory_accessible() )
    {
        ret = all_inventory( ob );
        n = sizeof( ret );
        for( i = 0; i<n; i++ )
            ret += deep_useful_inv( ret[i] );
        return ret;
    }
    return ({  });
}

object get_object(string arg)
{
    object ob;

    if ( !arg )
        return 0;

    if ( arg == "me" )
        return this_body();

    if ( arg == "here" )
        return environment(this_body());

    if ( arg == "shell" )
        return this_user()->query_shell_ob();

    if ( !(ob = present( arg, this_body())) )
        if ( environment(this_body()) &&
          !(ob = present(arg, environment(this_body()))) )
            if ( !(ob = find_body(arg)) )
                if ( !(ob = load_object(evaluate_path(arg))) )
                    if (!(ob = load_object(evaluate_path(arg)+".scr")))
                        return 0;

    return ob;
}

object get_object_number(object environment, string arg, int number)
{
   object ob;
   int number_counter = 1;

   if (!arg) { return 0; }

   foreach (object thing in all_inventory(environment))
   {
      if (thing->id(arg))
      {
         if (number == number_counter)
         {
            return thing;
         }

         number_counter++;
      }
   }

   return ob;
}

object root_environment(object o)
{
    object env, env2;

    env = environment(o);
    if(!env)
        return 0;
    while(env2 = environment(env))
    {
        env = env2;
    }
    return env;
}

int immediately_accessible(object o)
{
   object env;

   if (!objectp(o)) { return 0; }

   env = environment(o);

   if ((env == this_body()) || (env == environment(this_body())))
   {
      return 1;
   }

   if (!objectp(env)) { return 0; }

   return ((int)env->inventory_accessible()
      && (member_array(o, deep_inventory(root_environment(this_body())))!= -1));
}

int usable( object o, int flag )
{
    if( interactive(o) || o->is_living() )
        return 0;

    /* for things that can be used in certain conditions without
     * actually being in the player's inventory
     */
    if (o->always_usable()) return 1;

    if (environment(o) == this_body()) return 1;

    if( immediately_accessible( o ) )
    {
        if( flag )
        {
            write( "(Taking the "+o->the_short()+" first)\n" );
            VERB_OB_GET->do_get("OBJ", o, 0);

            return immediately_accessible(o);
        }
        return 1;
    }

    write( "(Taking the "+o->the_short()+" first)\n" );
    VERB_OB_GET->do_get("OBJ", o, 0);

    return immediately_accessible(o);
}

/* returns a nice listing of the given objects */
/* if (flag) then don't print untouched obs */
/* depth is for internal use only */
varargs string inv_list(object array obs, int flag, int depth)
{
   string res = "";
   int j;

   depth++;

   if (!sizeof(obs)) { return ""; }

   foreach (object ob in obs)
   {
      if (!ob->is_visible()) { continue; }

      if (!ob->short()) { continue; }

      if (flag && !ob->test_flag(TOUCHED) && ob->untouched_long()) { continue; }

      if (ob->test_flag(F_CONCEALED)) { continue; }

      if (ob->is_attached())
      {
         if (ob->inventory_visible() && !ob->query_hide_contents())
         {
            res += ob->inventory_recurse(depth);
         }

         continue;
      }

      if (!duplicatep(ob))
      {
         for (j = 0; j < depth; j++)
         {
            res += "  ";
         }

         if ((j = count(ob)) > 1)
         {
            res += j + " " + ob->plural_short();
         }
         else
         {
            if (ob->is_living())
            {
               res += ob->in_room_desc();
            }
            else
            {
               res += ob->a_short() + ob->get_attributes();
            }
         }

         res += "\n";

         if (ob->inventory_visible() && !ob->query_hide_contents())
         {
            res += ob->inventory_recurse(depth);
         }
      }
   }

   return res == "" ? 0 : res;
}

varargs string inv_list_include_concealed(object array obs, int flag, int depth)
{
   string res = "";
   int j;

   depth++;

   foreach (object ob in obs)
   {
      if (!ob->is_visible()) { continue; }

      if (!ob->short()) { continue; }

      if (flag && !ob->test_flag(TOUCHED) && ob->untouched_long()) { continue; }

      if (ob->is_attached())
      {
         if (ob->inventory_visible() && !ob->query_hide_contents())
         {
            res += ob->inventory_recurse(depth);
         }

         continue;
      }

      if (!duplicatep(ob))
      {
         for (j = 0; j < depth; j++)
         {
            res += "  ";
         }

         if ((j = count(ob)) > 1)
         {
            res += j + " " + ob->plural_short();
         }
         else
         {
            if (ob->is_living())
            {
               res += ob->in_room_desc();
            }
            else
            {
               res += ob->a_short() + ob->get_attributes();
            }
         }

         if (ob->test_flag(F_CONCEALED))
         {
            res += " (concealed)";
         }

         res += "\n";

         if (ob->inventory_visible() && !ob->query_hide_contents())
         {
            res += ob->inventory_recurse(depth);
         }
      }
   }

   return res == "" ? 0 : res;
}

varargs string inv_list_plain(object array obs, int flag, int depth)
{
   string res = "";
   int j;

   depth++;

   foreach (object ob in obs)
   {
      if (!duplicatep(ob))
      {
         for (j = 0; j < depth; j++)
         {
            res += "  ";
         }

         if ((j = count(ob)) > 1)
         {
            res += j + " " + ob->plural_short();
         }
         else
         {
            if (ob->is_living())
            {
               res += ob->in_room_desc();
            }
            else
            {
               res += ob->a_short() + ob->get_attributes();
            }
         }

         res += "\n";

         if (ob->inventory_visible() && !ob->query_hide_contents())
         {
            res += ob->inventory_recurse(depth);
         }
      }
   }

   return res == "" ? 0 : res;
}

object owner(object ob)
{
   object env;

   env = environment(ob);

   while (env && !env->is_living())
   {
      env = environment(env);
   }

   return env;
}

mixed target(mixed target)
{
   mixed ret;

   ret = ({ });

   if (stringp(target))
   {
      if (target == "here")
      {
         target = environment(this_body());
      }
      else
      {
         if (!(target= load_object(evaluate_path(target))))
         {
            target = find_body(target);
         }
      }
   }

   if (objectp(target))
   {
      if (target->is_living())
      {
         ret += ({ target });
      }

       ret += all_inventory(target);
   }

   return ret;
}

string object_event_message(mixed msg)
{
   msg = M_MESSAGES->action(({}), msg, previous_object());

   return msg[0];
}

void object_event(mixed msg)
{
   tell_environment(previous_object(), object_event_message(msg));
}

mixed parser_root_environment(object ob)
{
   object tmp;

   while ((tmp = environment(ob)) && ob->parent_environment_accessible())
   {
      ob = tmp;
   }

   return ob;
}