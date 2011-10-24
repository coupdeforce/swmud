// This is a standard verb handler.  It handles adding and removing verb rules (to/from the parser efuns)
#include <verbs.h>

string verb = split_path(file_name())[1];
int flags = NEED_TO_SEE | NEED_TO_BE_ALIVE | NEED_TO_THINK;

protected varargs void add_rules(array rules, array syns)
{
   parse_init();

   foreach (string rule in rules)
   {
      parse_add_rule(verb, rule);

      if (syns)
      {
         foreach (string syn in syns)
         {
            parse_add_synonym(syn, verb, rule);
         }
      }
   }
}

protected void set_flags(int f)
{
   flags = f;
}

protected void add_flag(int f)
{
   flags |= f;
}

protected void clear_flag(int f)
{
   flags &= ~f;
}

string refer_to_object(object ob)
{
   // In the future, this should be smarter.  Ideally it would generate
   // something unique like 'my first sword' or 'the third sword on the table'
   return ob->query_primary_name();
}

mixed try_to_acquire(object ob)
{
   // for things that can be used in certain conditions without actually being in the player's inventory
   if (ob->always_usable()) { return 1; }

   if (environment(ob) == this_body()) { return 1; }

   write("(Taking " + ob->the_short());

   if (!environment(ob))
   {
      write("What a quaint idea.\n");
      return 0;
   }

   if (environment(ob) != environment(this_body()))
   {
      write(" from " + environment(ob)->the_short());
   }

   write(" first)\n");
   this_body()->do_game_command("get " + refer_to_object(ob));
   return environment(ob) == this_body();
}

mixed check_ghost()
{
   if (this_body()->query_ghost())
   {
      return "But you're a ghost!\n";
   }

   return 1;
}

varargs mixed check_vision(object ob)
{
   int light = environment(this_body())->query_light();

   if (ob && ob->is_light_source())
   {
      return 1;
   }

   if (light >= 10000)
   {
      return "It's impossible to see in the ultra-violet light.\n";
   }
   else if (light >= 1000)
   {
      return "It's too bright to see.\n";
   }

   if (light > 0)
   {
      return 1;
   }

   if (environment(this_body())->parent_environment_accessible())
   {
      light = environment(environment(this_body()))->query_light();

      if (light >= 10000)
      {
         return "It's impossible to see in the ultra-violet light.\n";
      }
      else if (light >= 1000)
      {
         return "It's too bright to see.\n";
      }

      if (light > 0)
      {
         return 1;
      }
   }

   return "It's too dark to see.\n";
}

mixed check_condition()
{
   mixed tmp;

   if (tmp = this_body()->check_condition(0))
   {
      return tmp;
   }

   return 1;
}

// All (most) can_* functions should call this
varargs mixed default_checks(object ob)
{
   mixed tmp;

   if ((flags & NEED_TO_SEE) && (tmp = check_vision(ob)) != 1)
   {
      return tmp;
   }

   if ((flags & NEED_TO_BE_ALIVE) && (tmp = check_ghost()) != 1)
   {
      return tmp;
   }

   // This checks stunned, asleep
   if ((flags & NEED_TO_THINK))
   {
      return check_condition();
   }

   return 1;
}

void handle_obs(array info, function callback, mixed extra...)
{
   foreach (mixed ob in info)
   {
      if (stringp(ob))
      {
         write(ob);
      }
      else
      {
//         write(ob->short() + ": ");
         evaluate(callback, ob, extra...);
      }
   }
}

// we defined the rule, so assume by default we allow it
mixed can_verb_rule(string verb, string rule)
{
   return default_checks();
}

// default behavior for OBJ rules
void do_verb_obj(string verb, object ob)
{
   if ((flags & TRY_TO_ACQUIRE) && !try_to_acquire(ob))
   {
      return;
   }

   call_other(ob, "do_" + verb);
}