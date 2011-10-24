// Inheritable for aggressive monsters
// Onyx@Red Dragon
// NOTE: Someone who understands Lima vanilla combat should edit the
// line I indicated to make this compat.
//
// 12 May, 1998: Iizuka updated to support the new adversary code.

void add_hook(string tag, function hook);
void remove_hook(string tag, function hook);
void start_fight(object who);
void process_agro(object who);
void process_move();

function agro_func;
function move_func;
private object my_loc;
private int aggression_chance = 100;

//:FUNCTION handle_attack
// By default, this compares a random number to aggression_chance
// and starts attacking based upon them. This function might be
// overloaded to perform racial checks or something though.
protected void handle_attack(object who)
{
   if (random(100) < aggression_chance)
   {
      start_fight(who);
   }
}

// It checks to see which objects in the room are players and attacks them.
void process_agro(object who)
{
   if (who == this_object())
   {
      foreach(object thing in all_inventory(environment()))
      {
         if (thing->is_attackable() && (thing != this_object()))
         {
            handle_attack(thing);
         }
      }
   }
   else
   {
      handle_attack(who);
   }
}

// It moves the "object_arrived" hook (which is associated with a room) when the aggressive monster moves
void process_move()
{
   if (my_loc)
   {
      my_loc->remove_hook("object_arrived", agro_func);
   }

   my_loc = environment();
   agro_func = (: process_agro :);
   my_loc->add_hook("object_arrived", agro_func);
}

//:FUNCTION set_aggressive
// This function is used to set the aggression chance of a monster in the
// range 0 (never aggressive) to 100 (completely aggressive).
void set_aggressive(int a)
{
   if (!a)
   {
      if (my_loc)
      {
         my_loc->remove_hook("object_arrived", agro_func);
      }

      if (aggression_chance)
      {
         remove_hook("move", move_func);
      }
   }
   else
   {
      my_loc = environment();

      if (my_loc)
      {
         agro_func = (: process_agro :);
         my_loc->add_hook("object_arrived", agro_func);
      }

      if (!aggression_chance)
      {
         move_func = (: process_move :);
         add_hook("move", move_func);
      }
   }

   aggression_chance = a;
}

//:FUNCTION query_aggressive
// This function returns 0 for unaggressive monsters, or the chance of
// aggression for aggressive monsters.
int query_aggressive()
{
   return aggression_chance;
}