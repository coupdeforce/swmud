// Handle the ability of 'monsters' to 'target' other objects.
// One can rewrite this to, say, pick a random target instead of concentrating on one person,
// but be careful that query_target() must continually return the same object between get_target()'s

void stop_attacking();
varargs void stop_fight(object who);
int start_fight(object who);

private nosave object target;
private nosave object array other_targets = ({ });
private nosave object array unfinished_business = ({ });
private nosave mapping ranged_targets = ([ ]);
private nosave mapping ranged_unfinished_business = ([ ]);
private nosave object array backstabbers = ({ });

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

mapping query_ranged_targets()
{
   return ranged_targets;
}

mapping query_ranged_unfinished_business()
{
   return ranged_unfinished_business;
}

void backstabbed_by(object assassin)
{
   if (member_array(assassin, backstabbers) == -1)
   {
      backstabbers += ({ assassin });
   }
}

int has_been_backstabbed_by(object assassin)
{
   if (member_array(assassin, backstabbers) > -1)
   {
      return 1;
   }

   return 0;
}

object get_ranged_target()
{
   if (target && (environment(this_object()) != environment(target))
      && sizeof(filter_array(this_object()->query_weapons(), (: $1->query_range() :))))
   {
      object dir_room;
      int dir_count = 0;

      foreach (string dir in environment(this_object())->query_exit_directions(wizardp(this_object())))
      {
         dir_room = find_object(environment(this_object())->query_exit_destination(dir));
         dir_count = 0;

         while (dir_room)
         {
            dir_count++;

            if (member_array(target, all_inventory(dir_room)) > -1)
            {
               foreach (object weapon in this_object()->query_weapons())
               {
                  if (weapon->query_range() >= dir_count)
                  {
                     if (weapon->requires_ammo() && (weapon->query_ammo() > 0))
                     {
//                        tell(this_object(), "Your " + weapon->short() + " can fire from " + dir_count + " rooms away.");
                        return target;
                     }
                  }
               }

               return 0;
            }

            if (dir_room->query_exit_destination(dir))
            {
               dir_room = find_object(dir_room->query_exit_destination(dir));
            }
            else
            {
               dir_room = 0;
            }
         }
      }
   }

   return 0;
}

// Find someone to attack.  Return zero if we're dead or asleep or have no one to attack.
//:FUNCTION get_target
//Get someone to attack from our attackers list
object get_target()
{
   int count = sizeof(other_targets);

   if (get_ranged_target())
   {
      return target;
   }

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

      if (!who->is_justified_target())
      {
         this_object()->adjust_jedi_alignment(-1);
         this_object()->add_unjustified_ithorian_target(who);
      }
   }
   else if (!this_object()->is_body())
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
   this_object()->set_follow(target);
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