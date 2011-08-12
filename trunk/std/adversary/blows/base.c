#include <combat_modules.h>
#include <hooks.h>
#include <playerflags.h>

inherit __DIR__ "class_" STRINGIZE(BLOW_MODULE);

varargs int hurt_us(int, string, int);
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
int check_anatomy_weapon(object weapon, string array types);

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

      if ((damage > 0) && check_anatomy_weapon(last_weapon_used, keys(evt->data)) && evt->attacker->query_guild_level("physician") && evt->attacker->has_learned_skill("anatomy") && evt->attacker->test_skill("anatomy", evt->attacker->query_guild_level("physician") * 15))
      {
         int level = evt->attacker->query_guild_level("physician");
         int rank = evt->attacker->query_skill("anatomy") / 100;
         int spec = evt->attacker->query_guild_specialization_rank("physician", "anatomy");
         int rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);
         int adjustment = level * (rank_spec * 5) / 100;
         int limb_damage = damage * random(50 + adjustment + 1) / 100;

         if ((damage < rank_spec) && (rank > 0))
         {
            damage += random(last_weapon_used->query_weapon_class() + 1) * level / (100 - (rank * 5));
         }

         if (damage < rank)
         {
            damage = rank;
         }

         if (limb_damage < 1)
         {
            limb_damage = 1;
         }

         damage = hurt_us(damage, evt->target_extra, limb_damage);
      }
      else
      {
         damage = hurt_us(damage, evt->target_extra);
      }

      if (evt->attacker->is_body())
      {
         if ((evt->target->query_race() != "ithorian") || !evt->target->is_body())
         {
            if (evt->attacker->query_race() != "ithorian")
            {
               evt->attacker->add_experience(damage * (50 - (evt->attacker->query_primary_level() - evt->target->query_primary_level())) / 100);
            }
         }
         else
         {
            if (evt->attacker->query_experience() >= (damage * 10))
            {
               evt->target->add_experience(damage * 10);
               evt->attacker->subtract_experience(damage * 10);
            }
            else
            {
               evt->target->add_experience(evt->attacker->query_experience());
               evt->attacker->subtract_experience(evt->attacker->query_experience());
            }
         }
      }

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

// This is the method that gets called in the target object.
// Before the event does any damage to us, make modifications.
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
            int rank = (evt->target->query_skill("jedi_defense") / 100) + (evt->target->query_skill("saber_defense") / 100);
            int spec = evt->target->query_guild_specialization_rank("jedi", "defense");
            int rank_spec;
            int attacker_advantage = 100;
            int roll;
            int attacker_roll = random(attacker_advantage / 2);

            if (spec <= 0) { spec = evt->target->query_guild_specialization_rank("jedi", "lightsaber"); }

            rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);
            roll = random(array_sum(evt->target->query_weapons()->query_deflection_bonus()) + (level / 2) + (force / 5) + rank_spec + 1);

            tell(evt->target, "Your deflection roll is " + roll + " against " + attacker_roll + ".\n");

            if (roll > attacker_roll)
            {
               mapping energy_damage = filter_array(evt->data, (: $1 == "energy" :) );
               object this_ob = this_object();
               evt->data = filter_array(evt->data, (: ($1 != "energy") && ($1 != "ion") && ($1 != "electrical") :) );
               attacker_roll += random(attacker_advantage);

               tell(evt->target, "Your reflection roll is " + roll + " against " + attacker_roll + ".\n");

               if (sizeof(this_ob->query_targets()) && (roll > attacker_roll))
               {
                  object reflect_target = evt->attacker;
                  string limb;

                  if (!reflect_target) { reflect_target = this_ob; }

                  limb = reflect_target->query_random_armor_slot();

                  if (!present("lightsaber_blaster_reflect", evt->target))
                  {
                     load_object("/d/obj/spec_damage");
                     new("/d/obj/spec_damage", "reflected blaster bolt", "lightsaber_blaster_reflect")->move(evt->target);
                     present("lightsaber_blaster_reflect", evt->target)->set_combat_messages("combat-blaster-reflect");
                     present("lightsaber_blaster_reflect", evt->target)->set_death_message("$N was killed by a blaster bolt reflected from $P1 lightsaber at $o1.");
                  }

                  evt->target->add_event(evt->attacker, present("lightsaber_blaster_reflect", evt->target), limb, energy_damage, evt->target);
               }
               else
               {
                  if (event_damage(evt))
                  {
                     evt->data = ({ "deflect", evt->data });
                  }
                  else
                  {
                     evt->data = "deflect";
                  }

                  return evt;
               }
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

         // Add reflected damage event here
         if (!present("force_reflect", evt->target))
         {
            new("/d/obj/spec_damage", "Force reflection shield", "force_reflect")->move(evt->target);
            present("force_reflect", evt->target)->set_combat_messages("combat-reflect");
         }

         present("force_reflect", evt->target)->set_death_message("$N was killed by reflected damage from $p " + (last_weapon_used == previous_object() ? "bare hands" : "own " + last_weapon_used->short()) + " after attacking $N1 at $o1.");

//         tell(previous_object(), "You should be taking " + damage_total(reflected_damage) + " damage.\n");
         add_event(previous_object(), present("force_reflect", evt->target), previous_object()->query_random_armor_slot(), reflected_damage, evt->target);
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

int check_anatomy_weapon(object weapon, string array types)
{
   if (weapon->is_lightsaber() || weapon->is_blaster())
   {
      return 1;
   }

   foreach (string type in types)
   {
      if (member_array(type, ({ "slashing", "striking" })) > -1)
      {
         return 1;
      }
   }

   return 0;
}

void handle_result(class event_info evt)
{
   if (mapp(evt->data))
   {
      handle_message(damage_message(event_damage(evt)), evt->target, evt->weapon, evt->target_extra);
      evt->target->do_damage_event(evt);
   }
   else if (arrayp(evt->data) && stringp(evt->data[0]))
   {
      if (evt->data[0] == "deflect")
      {
         evt->data = evt->data[1];

         handle_message("!deflect-" + damage_message(event_damage(evt))[1..], evt->target, evt->weapon, evt->target_extra);
         evt->target->do_damage_event(evt);
      }
      else if (evt->data[0] == "reflect")
      {
         evt->data = evt->data[1];

         handle_message("!reflect-" + damage_message(event_damage(evt))[1..], evt->target, evt->weapon, evt->target_extra);
         evt->target->do_damage_event(evt);
      }
   }
   else if (stringp(evt->data))
   {
      handle_message("!" + evt->data, evt->target, evt->weapon, evt->target_extra);
      evt->target->do_damage_event(evt);
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