#include <hooks.h>
#include <playerflags.h>

mixed call_hooks(string, int);
object query_weapons();
object query_target();
object get_target();
object get_ranged_target();
void add_event(object target, object weapon, mixed target_extra, mixed data, object attacker);
int badly_wounded();
int panic();
varargs void stop_fight(object);
int check_condition(int);
int query_stunned();
void simple_action(string);
void my_action(string);
mapping query_damage_bonuses();
int query_ghost();
int query_agi();
int test_skill(string skill);
void target_is_asleep();
void handle_events();
int has_skill_delay();
void perform_combat_action();
int start_fight(object who);
varargs int check_wimpy(object target);

mapping randomize_damage(mapping damage_types)
{
   foreach (string type in keys(damage_types))
   {
      damage_types[type] = random(damage_types[type]) + 1;
   }

   return damage_types;
}

void take_a_swing(object target)
{
   int slowed = this_body()->query_slowed() && (this_body()->query_agi() * 100 / 200 < random(100)) ? 1 : 0;
   int speed_bonus = 0;

//tell(this_object(), "Taking a swing at " + target->short() + ".\n");

//   if ((environment() != environment(target)) || check_wimpy() || this_object()->query_stunned())
   if (check_wimpy())
   {
      return;
   }
   else if (this_object()->query_stunned())
   {
      tell(this_object(), "You are stunned and unable to attack.\n");
      return;
   }

   if (!slowed)
   {
//      speed_bonus = random(((this_body()->query_agi() * 2) / 5) + 21) > 55 ? 1 : 0;
      speed_bonus = (query_agi() * (20 + call_hooks("combat_speed_bonus", HOOK_SUM))) > random(5000) ? 1 : 0;
   }

   speed_bonus += call_hooks("force_speed", HOOK_SUM);

   foreach (object weapon in query_weapons())
   {
      for (int count = 0; count < (slowed ? 1 : (weapon->query_weapon_speed()) + speed_bonus); count++)
      {
         if (query_ghost() || !(target = get_target()))
         {
            stop_fight();
            return;
         }

         if (weapon->requires_ammo() && (weapon->query_ammo() < 1) && !sizeof(weapon->query_out_of_ammo_damage_types()))
         {
            /*
            add_event(target, weapon, target->query_random_limb(), randomize_damage(([ "striking" : 1 ])), this_object());
            handle_events();
            start_fight(target);
            */
            continue;
         }

         if (!test_skill(weapon->query_skill_used()) && (random(200) > (weapon->query_to_hit_bonus() + call_hooks("to_hit_bonus", HOOK_SUM))))
         {
            add_event(target, weapon, target->query_random_limb(), "miss", this_object());
         }
         else if (this_object()->is_unjustified_ithorian_target(target))
         {
            if (random(this_object()->query_skill(weapon->query_skill_used()) + weapon->query_to_hit_bonus() + call_hooks("to_hit_bonus", HOOK_SUM)) > random(2000 - weapon->query_to_hit_bonus() - call_hooks("to_hit_bonus", HOOK_SUM)))
            {
               add_event(target, weapon, target->query_random_limb(), randomize_damage(weapon->query_damage_types()), this_object());
            }
            else
            {
               add_event(target, weapon, target->query_random_limb(), "miss", this_object());
            }
         }
         else
         {
            add_event(target, weapon, target->query_random_limb(), randomize_damage(weapon->query_damage_types()), this_object());
         }

         handle_events();

         if (weapon->requires_ammo())
         {
            weapon->adjust_ammo(-1);
         }

         start_fight(target);
      }
   }

   if (!this_body()->is_body() && !this_body()->has_skill_delay())
   {
      this_body()->perform_combat_action();
   }
}

int check_wimpy(object target)
{
   if (!target) { target = this_object(); }

   if (target->query_health() < target->query_wimpy_threshold())
   {
      string array room_exits = environment(target)->query_exit_directions(wizardp(target));
      string direction = target->query_wimpy_dir();
      object previous_target = query_target();

      if (!sizeof(room_exits)) { return; }

      if (direction != "out")
      {
         if (member_array(direction, environment(target)->query_exit_directions(0)) > -1)
         {
            target->do_game_command("go " + direction);
            target->query_target();
            previous_target->query_target();

            return 1;
         }
      }

      target->do_game_command("go " + choice(room_exits));
      target->query_target();
      previous_target->query_target();

      return 1;
   }

   return 0;
}

void attack()
{
   object target;
   mixed tmp;

   // check whether we are capable of attacking
   if (tmp = check_condition(1))
   {
      if (tmp[<1] == '\n') { write(tmp); }
      else { my_action(tmp + " and cannot attack."); }

      return;
   }

   target = get_target();

   // Any reason to continue the carnage?
   if (query_ghost() || !target)
   {
      stop_fight();
      return;
   }

   if (target->query_asleep())
   {
      // Our target is unconcious. We get to have our way with them *evil grin*
      target_is_asleep();
      return;
   }

   take_a_swing(target);
   handle_events();
}