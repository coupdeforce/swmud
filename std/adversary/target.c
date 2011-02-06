// Last edited by deforce on 04-04-2010
// Handle the ability of 'monsters' to 'target' other objects.
// One can rewrite this to, say, pick a random target instead of concentrating on one person,
// but be careful that query_target() must continually return the same object between get_target()'s

void stop_attacking();
varargs void stop_fight(object who);
int start_fight(object who);

private nosave object target;
private nosave object array other_targets = ({ });
private nosave object array unfinished_business = ({ });

object query_target()
{
   return target;
}

object array query_targets()
{
   return filter_array(({ target }) + other_targets, (: $1 :));
}

object array query_unfinished_business()
{
   return filter_array(unfinished_business, (: $1 :));
}

// Find someone to attack.  Return zero if we're dead or asleep or have no one to attack.
//:FUNCTION get_target
//Get someone to attack from our attackers list
object get_target()
{
   int count = sizeof(other_targets);

   if (member_array(target, all_inventory(environment(this_object()))) == -1)
   {
      unfinished_business += ({ target });
      target = 0;
   }

   foreach (object foe in other_targets)
   {
      if (member_array(foe, all_inventory(environment(this_object()))) == -1)
      {
         unfinished_business += ({ foe });
      }
   }

   if (target == this_object()) { target = 0; }

   // Make sure they are alive and in the same room as us.  If not, find someone else.
   while (!target || target->query_ghost()
      || (member_array(target, all_inventory(environment(this_object()))) == -1))
   {
      while (count >= sizeof(other_targets))
      {
         count--;
      }

      if (!count || (count < 0))
      {
         return (target = 0);
      }

      target = other_targets[count];
      other_targets -= ({ target });

      if (member_array(target, all_inventory(environment(this_object()))) == -1)
      {
         unfinished_business += ({ target });
         target = 0;
      }

      count--;
   }

   unfinished_business -= ({ target });
   unfinished_business -= ({ 0 });

   return target;
}

void initiate_combat(object who)
{
   if (this_object()->is_body() && (who != target) && (member_array(who, other_targets) == -1) && (member_array(who, unfinished_business) == -1))
   {
      start_fight(who);

      if (this_object()->query_jedi_alignment() > 0)
      {
         this_object()->adjust_jedi_alignment(-1);
      }
   }
   else
   {
      start_fight(who);
   }
}

//:FUNCTION switch_to
//Make the specified target the primary target
void switch_to(object who)
{
   if (who == target) { return; }

   if (target)
   {
      other_targets += ({ target });
   }

   other_targets -= ({ who });
   target = who;
}

//:FUNCTION stop_hitting_me
//remove the previous object from the target list
void stop_hitting_me()
{
   if (previous_object() == target)
   {
      target = 0;
   }

   other_targets -= ({ previous_object() });
}

// This stops attacking "who".  It also takes care of stoping them from attacking us.
// stop_fight() or stop_fight(0) stops all combat.
//:FUNCTION stop_fight
//stop fighting with object ob (handles making sure they stop fighting
//with us too).  stop_fight(0) stops all fights.
varargs void stop_fight(object who, int only_my_aggression)
{
   if (!who || undefinedp(who))
   {
      if (target)
      {
         target->stop_hitting_me();
      }

      other_targets->stop_hitting_me();
      target = 0;
      other_targets = ({ });
      stop_attacking();
      return;
   }

   if (who == target)
   {
      target = 0;
   }
   else
   {
      other_targets -= ({ who });
      unfinished_business -= ({ who });
   }

   if (!only_my_aggression) { who->stop_hitting_me(); }

   // If there are no targets left, remove the heart_beat().
   if (query_targets() == ({ 0 }))
   {
      stop_attacking();
   }
}