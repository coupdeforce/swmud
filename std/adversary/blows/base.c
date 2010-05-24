// Last edited by deforce on 05-07-2010
#include <combat_modules.h>
#include <hooks.h>
#include <playerflags.h>

inherit __DIR__ "class_" STRINGIZE(BLOW_MODULE);

varargs int hurt_us(int, string);
varargs void attacked_by(object, int);
string query_random_limb();
void handle_message(string, object, object, string);
string damage_message(int);
object array query_targets();
object query_weapon();
varargs void unwield(string);
object event_get_armor(class event_info);
int test_flag(int);
mixed call_hooks(string, int);
void add_event(object target, object weapon, mixed target_extra, mixed data, object attacker);
void handle_events();
int damage_total(mapping data);

private nosave object last_weapon_used;

object query_last_weapon_used() { return last_weapon_used; }

int do_damage_event(class event_info evt)
{
   int damage = 0;
   last_weapon_used = evt->weapon;

   if (mapp(evt->data))
   {
      if (this_object()->has_buff("/d/buffs/force_stun"))
      {
         this_object()->cancel_buff("/d/buffs/force_stun");
      }

      foreach (string type in keys(evt->data))
      {
         damage += evt->data[type];
      }

      damage = hurt_us(damage, evt->target_extra);

      previous_object()->add_experience(damage / 2);

      // Chance for weapon to be damaged
      if (last_weapon_used->query_chance_to_be_damaged() > random(100))
      {
         last_weapon_used->decrease_class(1);
      }
   }

   if (member_array(previous_object(), query_targets()) == -1)
   {
      attacked_by(previous_object(), 1);
   }

   return damage;
}

// Modify us (as attacker). Include things such as racial modifiers, resistances, etc.
class event_info modify_our_event(class event_info evt)
{
   return evt;
}

class event_info health_modify_event(class event_info evt)
{
   return evt;
}

class event_info armors_modify_event(class event_info evt)
{
   object armor = event_get_armor(evt);

   if (armor)
   {
      evt = armor->sink_modify_event(evt);
   }

   return evt;
}

// This is the method that gets called in the target object. Before
// the event does any damage to us, make modifications.
class event_info modify_event(class event_info evt)
{
   if (mapp(evt->data))
   {
      if (evt->weapon->is_blaster() && evt->data["energy"]
         && sizeof(filter_array(evt->target->query_weapons(), (: $1->is_lightsaber() :) ))
         && evt->target->query_guild_level("jedi") && evt->target->has_learned_skill("lightsaber defense"))
      {
         int jedi_alignment = evt->target->query_jedi_alignment();

         if (evt->target->test_skill("saber_defense", jedi_alignment * (jedi_alignment < 0 ? -10 : 10)))
         {
            int level = evt->target->query_guild_level("jedi");
            int force = evt->target->query_for();
            int rank = evt->target->query_skill("jedi_defense") / 100;
            int spec = evt->target->query_guild_specialization_rank("jedi", "defense");
            int rank_spec;
            int roll;

            if (spec <= 0) { spec = evt->target->query_guild_specialization_rank("jedi", "lightsaber"); }

            rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);
            roll = random((level / 2) + (force / 5) + rank_spec + 1);

            if (roll > random(20))
            {
               if (roll > random(40))
               {
                  evt->data = "blaster-reflect";
                  return evt;
               }

               evt->data = "blaster-deflect";
               return evt;
            }
         }
      }

      if (test_flag(F_FORCE_REFLECT) && !last_weapon_used->id("force_reflect"))
      {
         mapping reflected_damage = ([ ]);

         foreach (string type in keys(evt->data))
         {
            reflected_damage[type] = to_int(ceil(evt->data[type] * call_hooks("force_reflect", HOOK_SUM) / 100.0));

            evt->data[type] -= reflected_damage[type];
         }

//         tell(this_object(), "Damage reflected: " + damage_total(reflected_damage) + "   Taken: " + damage_total(evt->data) + ".\n");

         // Add reflected damage event here, as force damage
         if (!present("force_reflect", evt->target))
         {
            new("/d/obj/force_damage", "Force reflection shield", "force_reflect")->move(evt->target);
            present("force_reflect", evt->target)->set_combat_messages("combat-reflect");
         }

         present("force_reflect", evt->target)->set_death_message("$N was killed by reflected damage from $p " + (last_weapon_used == previous_object() ? "bare hands" : "own " + last_weapon_used->short()) + " after attacking $N1 at $o1.");

//         tell(previous_object(), "You should be taking " + damage_total(reflected_damage) + " damage.\n");
         add_event(previous_object(), present("force_reflect", evt->target), previous_object()->query_random_armor_slot(), reflected_damage, evt->target);
         handle_events();
      }
      else if (call_hooks("force_shield", HOOK_SUM))
      {
         foreach (string type in keys(evt->data))
         {
            if (member_array(type, ({ "slashing", "piercing", "striking" })) > -1)
            {
               int damage = evt->data[type];
               int damage_reduction = to_int(ceil(damage * call_hooks("force_shield", HOOK_SUM) / 100.0));

               if (damage_reduction < 1) { damage_reduction = 1; }

               evt->data[type] -= damage_reduction;

//               tell(this_object(), "Damage: " + damage + "   Shielded: " + damage_reduction + "   Taken: " + evt->data[type] + "\n");
            }
         }
      }
   }

   evt = health_modify_event(evt);
   evt = armors_modify_event(evt);

   return evt;
}

int damage_total(mapping data)
{
   int damage = 0;

   foreach (int amount in values(data))
   {
      damage += amount;
   }

   return damage;
}

void handle_result(class event_info evt)
{
   if (mapp(evt->data))
   {
      int percent = event_damage(evt);
      handle_message(damage_message(percent), evt->target, evt->weapon, evt->target_extra);
      percent = evt->target->do_damage_event(evt);
   }
   else
   {
      handle_message("!miss", evt->target, evt->weapon, evt->target_extra);
      evt->target->do_damage_event(evt);
   }
}

void handle_events()
{
   int i = 0;

   // Walking over the queue is slightly more efficient than deleting the
   //   first element each time, causing them all to have to move
   while (i < sizeof(queue))
   {
      queue[i] = modify_our_event(queue[i]);

      if (!queue[i])
      {
         i++;
         continue;
      }

      if (queue[i]->weapon)
      {
         queue[i] = queue[i]->weapon->source_modify_event(queue[i]);

         if (!queue[i])
         {
            i++;
            continue;
         }
      }

      if (queue[i]->target)
      {
         queue[i] = queue[i]->target->modify_event(queue[i]);
      }

      if (!queue[i])
      {
         i++;
         continue;
      }

      handle_result(queue[i]);
      i++;
   }

   queue = ({ });
}