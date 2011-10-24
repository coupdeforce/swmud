// Last edited by deforce on 03-26-2010
// Created by Deathblade 08-13-1995

#include <classes.h>
#include <flags.h>
#include <hooks.h>

mixed call_hooks(string, int);
int test_flag(int);
void drop_object(object thing);

private int stat_str; // Strength
private int stat_con; // Constitution
private int stat_agi; // Agility
private int stat_dex; // Dexterity
private int stat_per; // Perception
private int stat_int; // Intelligence
private int stat_cha; // Charisma
private int stat_luc; // Luck
private int stat_for; // Force

private nosave int cur_str = 40;
private nosave int cur_con = 40;
private nosave int cur_agi = 40;
private nosave int cur_dex = 40;
private nosave int cur_per = 40;
private nosave int cur_int = 40;
private nosave int cur_cha = 40;
private nosave int cur_luc = 40;
private nosave int cur_for = 40;

private int points_remaining; // Used for character creation

void refresh_stats();

nomask int query_points_remaining()
{
   return points_remaining;
}

nomask void set_points_remaining(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set creation points remaining for " + this_object()->short() + " to " + n, previous_object(-1)); }

   points_remaining = n;
}

// query_xxx_pure() returns the "pure" unadulterated statistic.
// query_xxx() returns the statistic adjusted for all temporary effects
nomask int query_str_pure() { return stat_str; }
nomask int query_con_pure() { return stat_con; }
nomask int query_agi_pure() { return stat_agi; }
nomask int query_dex_pure() { return stat_dex; }
nomask int query_per_pure() { return stat_per; }
nomask int query_int_pure() { return stat_int; }
nomask int query_cha_pure() { return stat_cha; }
nomask int query_luc_pure() { return stat_luc; }
nomask int query_for_pure() { return stat_for; }

nomask int query_str() { return cur_str; }
nomask int query_con() { return cur_con; }
nomask int query_agi() { return cur_agi; }
nomask int query_dex() { return cur_dex; }
nomask int query_per() { return cur_per; }
nomask int query_int() { return cur_int; }
nomask int query_cha() { return cur_cha; }
nomask int query_luc() { return cur_luc; }
nomask int query_for()
{
   return cur_for + to_int(ceil(environment()->query_force_bonus(0) * 100 / cur_for));
}

nomask void set_str(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set strength for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_str = n;
   refresh_stats();
}

nomask void set_con(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set constitution for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_con = n;
   refresh_stats();
}

nomask void set_agi(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set agility for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_agi = n;
   refresh_stats();
}

nomask void set_dex(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set dexterity for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_dex = n;
   refresh_stats();
}

nomask void set_per(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set perception for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_per = n;
   refresh_stats();
}

nomask void set_int(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set intelligence for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_int = n;
   refresh_stats();
}

nomask void set_cha(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set charisma for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_cha = n;
   refresh_stats();
}

nomask void set_luc(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set luck for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_luc = n;
   refresh_stats();
}

nomask void set_for(int n)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set force for " + this_object()->short() + " to " + n, previous_object(-1)); }

   stat_for = n;
   refresh_stats();
}

//:FUNCTION refresh_stats
//refresh_stats() recalculates all the stats and requery's all the bonuses.
//Combat calls this once a round.  If you are using stats in a non-combat
//setting, you might want to call this first.
void refresh_stats()
{
//:HOOK str_bonus
//Used to modify strength
//:HOOK con_bonus
//Used to modify constitution
//:HOOK agi_bonus
//Used to modify agility
//:HOOK dex_bonus
//Used to modify dexterity
//:HOOK per_bonus
//Used to modify perception
//:HOOK int_bonus
//Used to modify intelligence
//:HOOK cha_bonus
//Used to modify charisma
//:HOOK luc_bonus
//Used to modify luck
//:HOOK for_bonus
//Used to modify force

   int adj_str, adj_con, adj_agi, adj_dex, adj_per, adj_int, adj_cha, adj_luc, adj_for;

   cur_str = stat_str + (adj_str = call_hooks("str_bonus", HOOK_SUM));
   cur_con = stat_con + (adj_con = call_hooks("con_bonus", HOOK_SUM));
   cur_agi = stat_agi + (adj_agi = call_hooks("agi_bonus", HOOK_SUM));
   cur_dex = stat_dex + (adj_dex = call_hooks("dex_bonus", HOOK_SUM));
   cur_per = stat_per + (adj_per = call_hooks("per_bonus", HOOK_SUM));
   cur_int = stat_int + (adj_int = call_hooks("int_bonus", HOOK_SUM));
   cur_cha = stat_cha + (adj_cha = call_hooks("cha_bonus", HOOK_SUM));
   cur_luc = stat_luc + (adj_luc = call_hooks("luc_bonus", HOOK_SUM));
   cur_for = stat_for + (adj_for = call_hooks("for_bonus", HOOK_SUM));

   if (cur_str < this_object()->query_race_str_min()) { cur_str = this_object()->query_race_str_min(); }
   if (cur_con < this_object()->query_race_con_min()) { cur_con = this_object()->query_race_con_min(); }
   if (cur_agi < this_object()->query_race_agi_min()) { cur_agi = this_object()->query_race_agi_min(); }
   if (cur_dex < this_object()->query_race_dex_min()) { cur_dex = this_object()->query_race_dex_min(); }
   if (cur_per < this_object()->query_race_per_min()) { cur_per = this_object()->query_race_per_min(); }
   if (cur_int < this_object()->query_race_int_min()) { cur_int = this_object()->query_race_int_min(); }
   if (cur_cha < this_object()->query_race_cha_min()) { cur_cha = this_object()->query_race_cha_min(); }
   if (cur_luc < this_object()->query_race_luc_min()) { cur_luc = this_object()->query_race_luc_min(); }
   if (cur_for < this_object()->query_race_for_min()) { cur_for = this_object()->query_race_for_min(); }

//   Now handled with m_damage_source_body
//   this_object()->set_weapon_class(6 + to_int(floor((this_object()->query_primary_level() / 10.0) + (pow(cur_str / 100.0, 2) * (this_object()->query_primary_level() / 2.0)))));
//   this_object()->set_to_hit_base(to_int(ceil(cur_agi / 2)) - 35);  // Unarmed to-hit penalty or bonus

//   Now handled with m_damage_sink_body
//   this_object()->set_armor_class(to_int(floor(this_object()->query_primary_level() / 5) + floor((this_object()->query_primary_level() / 50) * (cur_con / 5))));

   if (this_object()->is_body())
   {
      int max_health = 19 + this_object()->query_body_size() + floor((this_object()->query_primary_level() + 1) * (cur_con / 5.0));
      int limb_health = 9 + this_object()->query_body_size() + floor((this_object()->query_primary_level() + 1) * (cur_con / 10.0));

      foreach (string guild in this_object()->query_guild_names())
      {
         if (guild != this_object()->query_primary_guild())
         {
            max_health += this_object()->query_body_size() + floor(this_object()->query_guild_level(guild) * (cur_con / 10.0));
            limb_health += this_object()->query_body_size() + floor(this_object()->query_guild_level(guild) * (cur_con / 20.0));
         }
      }

      this_object()->set_max_health(max_health);

      foreach (string limb in this_object()->query_limbs())
      {
         this_object()->set_max_limb_health(limb, limb_health);
      }

      this_object()->set_max_fatigue_amount((max_health / 10) + this_object()->query_primary_level() + (this_object()->query_body_size() * cur_con / 5));

//      this_object()->set_max_capacity(cur_str * 3);
      this_object()->set_max_capacity(to_int(floor((cur_str + 100) * (1.25 * (cur_str / 100.0)))) + 50);

      if (this_object()->query_max_capacity() < this_object()->query_capacity())
      {
         this_object()->simple_action("$N $vbecome overburdened with the weight of $p equipment.");

         foreach (object thing in all_inventory(this_object()))
         {
            if (!thing->test_flag(F_WORN) && !thing->test_flag(F_WIELDED))
            {
               drop_object(thing);
            }

            if (this_object()->query_max_capacity() >= this_object()->query_capacity())
            {
               break;
            }
         }
      }
   }
}

// This is later modified by race
nomask void init_stats()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s stats to racial minimums", previous_object(-1)); }

   stat_str = this_object()->query_race_str_min();
   stat_con = this_object()->query_race_con_min();
   stat_agi = this_object()->query_race_agi_min();
   stat_dex = this_object()->query_race_dex_min();
   stat_per = this_object()->query_race_per_min();
   stat_int = this_object()->query_race_int_min();
   stat_cha = this_object()->query_race_cha_min();
   stat_luc = this_object()->query_race_luc_min();
   stat_for = this_object()->query_race_for_min();

   refresh_stats();
}

varargs int get_hp_gain(int primary)
{
   int bonus = (primary ? 3 : 2);
   int gain = 0;

   switch (stat_con)
   {
      case 0..9: bonus *= 10; break;
      case 10..14: bonus *= 11; break;
      case 15..19: bonus *= 12; break;
      case 20..24: bonus *= 13; break;
      case 25..29: bonus *= 14; break;
      case 30..34: bonus *= 15; break;
      case 35..39: bonus *= 16; break;
      case 40..44: bonus *= 18; break;
      case 45..49: bonus *= 20; break;
      case 50..54: bonus *= 22; break;
      case 55..59: bonus *= 24; break;
      case 60..64: bonus *= 26; break;
      case 65..69: bonus *= 28; break;
      case 70..74: bonus *= 31; break;
      case 75..79: bonus *= 34; break;
      case 80..84: bonus *= 37; break;
      case 85..89: bonus *= 40; break;
      case 90..94: bonus *= 43; break;
      case 95..99: bonus *= 46; break;
      case 100..999999: bonus *= 50;
   }

   gain += (primary ? 8 : 2);
   gain += random(((primary ? 6 : 3) * (stat_luc / 100.0)) + 1);
   gain += random((8 * (bonus / 100.0)) + 1);

   if (primary && (gain > 20)) { gain = 20; }
   else if (!primary && (gain > 10)) { gain = 10; }

   return gain;
}