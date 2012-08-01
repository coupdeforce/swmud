// Note: updates only when an /std/race/ object is updated.
//### lots of functions in here need to be autodoc'd

// /std/player.c  Written after login.c 1-12-94
// Rust@ZorkMUD
// with mods by alot of us:
// including:   Nevyn@ZorkMUD (GUE)
//              Peregrin @ZorkMUD (GUE)
// Beek ... Deathblade ... The rest of the Zorkmud staff ...
//
#include <security.h>
#include <setbit.h>
#include <playerflags.h>
#include <commands.h>
#include <move.h>
#include <hooks.h>
#include <log.h>
#include <size.h>
#include <clean_up.h>

// Files we need to inherit --
inherit ADVERSARY;
inherit M_ACCESS;
//inherit M_SMARTMOVE;
inherit M_FOLLOW;
//inherit M_ACTIONS;

//#ifndef EVERYTHING_SAVES
//private inherit M_SAVE; // don't want people calling load_from_string() externally
//#endif

inherit __DIR__ "body/cmd";
inherit __DIR__ "body/deaths";
inherit __DIR__ "body/guild_specializations";
inherit __DIR__ "body/help";
//inherit __DIR__ "body/jedi";
inherit __DIR__ "body/money";
inherit __DIR__ "body/naming";
inherit __DIR__ "body/quests";
inherit __DIR__ "body/skills";
inherit __DIR__ "body/spouse";
inherit __DIR__ "body/start";
inherit __DIR__ "body/status_line";
inherit __DIR__ "body/time";
inherit __DIR__ "body/title";
inherit __DIR__ "body/toggles";
inherit __DIR__ "body/wizfuncs";
inherit __DIR__ "body/wiz_position";

int query_jedi_alignment();

// Global variables --
private string plan;
private nosave object link;
private mixed saved_items;
private string location;

// interfaces for other objects to manipulate our global variables

//:FUNCTION is_body
//Is this a body object?
int is_body() { return 1; }

//:FUNCTION query_link
//Return our link object
nomask object query_link()
{
   return link;
}

void mudlib_setup(mixed args...)
{
   adversary::mudlib_setup(args...);
   m_follow::mudlib_setup(args...);
}

nomask string query_player_location()
{
   return location;
}

void set_player_location(string new_location)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s player location to " + new_location, previous_object(-1)); }

   location = new_location;
   save_me();
}

#ifdef EVERYONE_HAS_A_PLAN
//:FUNCTION query_plan
//Returns our plan
nomask string query_plan()
{
   return plan;
}

//:FUNCTION set_plan
//Sets our plan
nomask void set_plan(string new_plan)
{
   if (this_body() != this_object())
   {
      error("illegal attempt to set plan\n");
   }

   plan = new_plan;
   save_me();
}
#endif // EVERYONE_HAS_A_PLAN

protected void update_for_new_body(mapping tmp)
{
   // nothing for now; can be overloaded for races that need it
}
//:FUNCTION update_containers
// Update containers on loading - fix relationships
private nomask void update_container(object ob)
{
   foreach (object inv in all_inventory(ob))
   {
      if (inv->is_container())
      {
         update_container(inv);
      }

      ob->receive_object(inv);
   }
}

void check_health()
{
   if (this_object()->query_health() > this_object()->query_max_health())
   {
      this_object()->set_health(this_object()->query_max_health());
   }

   foreach (string limb in this_object()->query_limbs())
   {
      if (this_object()->query_limb_health(limb) > this_object()->query_max_health(limb))
      {
         this_object()->set_limb_health(limb, this_object()->query_max_health(limb));
      }
   }
}

// Added to complete initialisation of stuff loaded
private nomask void init_stuff()
{
   this_object()->reset_body_slots();
   this_object()->set_combat_messages("combat-unarmed");

   foreach (object inv in all_inventory())
   {
      if (inv->query_wielding() && sizeof(inv->query_wielding()))
      {
         this_object()->wield(inv, inv->query_wielding()[0]);
      }

      if (inv->test_flag(F_WORN))
      {
         inv->do_wear();
      }

      if (inv->test_flag(F_LIGHTED))
      {
         inv->set_light(inv->query_light_class());

         if (inv->is_flashlight())
         {
            inv->start_charge_drain();
         }
      }
   }

   update_container(this_object());
}

// Initialize various internal things
//### needs a new name
private nomask void init_cmd_hook()
{
   link = previous_object();

#ifdef USE_MASS
   set_mass(70000);
#endif
#ifdef USE_SIZE
   set_size(VERY_LARGE);
#endif
// set_max_capacity(400);  // was VERY_LARGE
//   set_max_capacity(query_str() * 3);  // new_cap

   if (saved_items)
   {
      string e;

      if (e = catch(load_from_string(saved_items, 1)))
      {
         mapping tmp = restore_variable(saved_items);

         if (tmp["#base_name#"] != base_name(this_object()))
         {
            update_for_new_body(tmp);
            tmp["#base_name#"] = base_name(this_object());
            load_from_string(save_variable(tmp), 1);
         }
         else
         {
            error("Rethrown: " + e); 
         }
      }

      saved_items = 0;
   }

   set_heart_beat(1);
   init_stuff();
}

nomask void su_enter_game(object where)
{
   init_cmd_hook();
   move(where);
}

void enter_game(int state)
{
   switch (state)
   {
      case 1:
         // FALLTHROUGH
      case 0:
         // existing user
         init_cmd_hook();
         CHANNEL_D->deliver_emote("announce", query_name(), sprintf("enters %s.", mud_name()));
         // move the body.  make sure this comes before the simple_action
         if (!move_to_start())
         {
            write("Uh-oh, you have no environment.\n");
         }
         else
         {
            // we don't want other people to get the extra newlines
            write("\n");

            if (is_visible()) { simple_action("$N $venter "+mud_name()+"."); }

            write("\n");
         }
         // FALLTHROUGH
      case 2:
         do_game_command("look");
         refresh_stats();
         check_health();
         update_health();
   }
}

//:FUNCTION save_me
//Saves us :-)
void save_me()
{
   object shell_ob = link && link->query_shell_ob();
   string userid = query_userid();

   // save the shell information
   if (shell_ob) { shell_ob->save_me(); }

   //### This check is bogus.  What should it be?
   // This check also doesn't work for su's -- John
   //    if (previous_object()==this_object())
   saved_items = save_to_string(1); // 1 meaning it is recursive.

   unguarded(1, (: save_object, USER_PATH(userid) :));
   saved_items = 0;
}

//:FUNCTION remove
//Handle mailboxes and the last login daemon, as well as the normal stuff
void remove()
{
   if (!clonep())
   {
      ::remove();
      return;
   }

#ifdef PLAYERS_START_WHERE_THEY_QUIT
   if (environment() && !wizardp(link))
   {
      set_start_location(file_name(environment()));
   }
#endif

   foreach (object thing in filter_array(all_inventory(), (: $1->id("spec_damage") :)))
   {
      destruct(thing);
   }

   if (!wizardp(link) && !GROUP_D->member_group(link->query_userid(), "testchars"))
   {
      foreach (object thing in filter_array(all_inventory(), (: !$1->is_autoload() :)))
      {
         thing->do_remove();
         thing->do_unwield();
         thing->clear_flag(F_WORN);
         thing->clear_flag(F_WIELDED);
         thing->clear_flag(F_HELD);
         thing->move(environment());
         thing->simple_action("$N $vfall to the ground.");
      }
   }

   set_total_connected_time(query_total_connected_time() + (time() - LAST_LOGIN_D->query_last(query_userid())[0]));
   save_me();

   LAST_LOGIN_D->register_last(query_userid());
   SNOOP_D->bye();
   ::remove();
}

//### This should be protected.
//:FUNCTION quit
//Quit the game.
void quit()
{
   if (!clonep())
   {
      ::remove();
      return;
   }

   if (is_visible()) { simple_action("$N $vhave left " + mud_name() + "."); }

   CHANNEL_D->deliver_emote("announce", query_name(), sprintf("has left %s.", mud_name()));

   remove();
}

void do_receive(string msg, int msg_type)
{
   if (link) { link->do_receive(msg, msg_type); }
}

//:FUNCTION net_dead
//This function is called when we lose our link
void net_dead()
{
   //### add security here?

   if (is_visible()) { simple_action("$N $vhave gone link-dead."); }

   CHANNEL_D->deliver_emote("announce", query_name(), sprintf("has gone link-dead.", mud_name()));
   SNOOP_D->bye(this_object());
}

//:FUNCTION reconnect
//This function is called when we get our link back
void reconnect(object new_link)
{
   //### add security here?

   link = new_link;

   receive_private_msg("\n\n", 0, 0);

   if (is_visible()) { simple_action("$N $vhave reconnected."); }

   CHANNEL_D->deliver_emote("announce", query_name(), sprintf("has reconnected.", mud_name()));
}

protected void die()
{
   object killer = previous_object();
   object weapon = query_last_weapon_used();
   string weapon_name;
   object location;
   string death_message;

   if (!weapon) { return; }

   weapon_name = weapon->is_adversary() ? "bare hands" : weapon->short();
   location = environment(this_object());
   death_message = weapon->query_death_message();

   if (killer && (killer != this_object()) && (environment(killer) != this_object()))
   {
      death_message = replace_string(action(({ this_object(), killer }), death_message, weapon, location)[2], "\n", "");
   }
   else
   {
      death_message = capitalize(short()) + " inadvertently killed " + query_reflexive() + " in a moment of carelessness with " + query_possessive() + " " + weapon_name + " at " + location->short() + ".";
   }

   if (wizardp(link))
   {
      if (is_visible()) { simple_action("If $n $vwere mortal, $n would now no longer be mortal."); }

      call_out((: reincarnate :), 1);
      this_object()->set_health(this_object()->query_max_health());
      return;
   }

   if (is_visible()) { other_action("$N $vfall to the ground, dead."); }
   receive_private_msg("\n\n***  You are dead  ***\n\nFortunately, you'll return to life in a moment.\n\n", 0, 0);

   // Alignment adjustment for jedi
   if ((query_jedi_alignment() != 0) && killer->is_body() && (killer->query_guild_level("jedi")))
   {
      if (killer->query_jedi_alignment() > 0)
      {
         int my_alignment = query_jedi_alignment();

         if (my_alignment > 0)
         {
            killer->adjust_jedi_alignment(to_int(floor(my_alignment / 1.25)) * -1);
         }
         else
         {
            killer->adjust_jedi_alignment(to_int(floor(my_alignment / 2.0)));
         }
      }
      else if ((killer->query_jedi_alignment() < 0) && (query_jedi_alignment() > 0))
      {
         killer->adjust_jedi_alignment(to_int(floor(query_jedi_alignment() / 4.0)) * -1);
      }
   }

   add_a_death();
//   drop_corpse(killer->short(), weapon_name);
   move(MORGUE_D->query_morgue(file_name(environment(killer))));
   this_object()->reset_unjustified_ithorian_targets();
   death_penalty();
//   reset_armors();

   call_out((: reincarnate :), 10);

   if (killer->is_body())
   {
      CHANNEL_D->deliver_channel("death-pk", death_message);
   }
   else
   {
      CHANNEL_D->deliver_channel("death", death_message);
   }

   set_killed_by(time(), killer->short(), location->short());
}

int clean_up()
{
   return NEVER_AGAIN;
}

//:FUNCTION id
//id(s) returns 1 if we respond to the name 's'
int id(string arg)
{
   if (!is_visible() && arg == lower_case(query_invis_name())) { return 1; }

   return ::id(arg);
}

string stat_me()
{
   string result = short() + "\n" + "Userid: " + query_userid() + "\n" + ::stat_me();

   if (link) { result += link->stat_me(); }

   return result;
}

void create(string userid)
{
   if (!clonep()) { return; }

   if (base_name(previous_object()) != USER_OB)
   {
      error("security violation: illegal attempt to change name\n");
   }

   messages = ([]);

   adversary::create();

   // Make some of the flags non-persistent (they won't be saved).
   configure_set(PLAYER_NP_FLAGS, 1);

   set_long((: our_description :));
   set_name(userid);

   unguarded(1, (: restore_object, USER_PATH(userid), 1 :));

   // up to the player
   set_attack_speed(0);
}

//### temp hack. be both user and body
nomask object query_body()
{
   return this_object();
}

// verb interaction
//mixed indirect_give_obj_to_liv(object ob, object liv)
//{
//   if ((previous_object() == liv) && ob->is_in(liv))
//   {
//      return "You already have that.\n";
//   }
//
//   return 1;
//}

int ob_state()
{
   return -1;
}

void force_look(int force_long_desc)
{
   environment(this_object())->do_looking(force_long_desc, this_object());
}

// Called when our environment destructs.  If we don't move, we get dested too.
void move_or_destruct(object suggested_dest)
{
   mixed err;
   object dested_env = environment();
   mixed destination;

   if (!query_link()) { return; }

   // Might want to add another failsafe room on the end of this list
   // that doesn't inherit room.c and is guaranteed to load/accept people.
   foreach (destination in ({ suggested_dest, VOID_ROOM, this_body()->query_start_location(), START, WIZARD_START  }))
   {
      err = catch
      {
         if (stringp(destination)) { destination = load_object(destination); }

         if (destination != dested_env)
         {
            err = move(destination);

            if (stringp(err)) { throw(err); }
         }
         else
         {
            throw("Being destructed.\n");
         }
      };

      if (destination && !err)
      {
         receive_private_msg(dested_env->short() + " being destructed: You have been moved to " + destination->short() + ".\n");
         return;
      }
      else
      {
         if (destination)
         {
            receive_private_msg("Cannot move to " + destination->short() + ": " + err);
         }
      }
   }

   receive_private_msg("Uh oh..., couldn't move you anywhere.  Goodbye.\n");
   (this_object()->query_link())->remove();
}


string living_query_name()
{
   return ::query_name();
}

string query_name()
{
   if (!is_visible()) { return "Someone"; }

   if (!::query_name()) { return ""; }

   return capitalize(::query_name());
}

// These are overrides from our ancestor (MONSTER)
string short() { return query_name(); }
string a_short() { return query_name(); }
string the_short() { return query_name(); }
string in_room_desc() { return base_in_room_desc() + query_idle_string(); }
// end of naming overrides

// m_conversation will override
void begin_conversation()
{
   tell(this_user(), "You have a very interesting conversation with " + capitalize(query_name()) + ".\n");
}

varargs int check_wizard_set(string action, object array prev_objects, int min_wiz_level)
{
   if (!min_wiz_level) { min_wiz_level = 3; }

   foreach (object thing in prev_objects)
   {
      if ((cannonical_form(thing) == "/secure/user") && (thing != link)
         && wizardp(thing) && (wizardp(thing) < min_wiz_level))
      {
         error("Invalid attempt by " + thing->query_userid() + " to " + action + ".\n");
      }
   }

   return 1;
}

int allow(string what)
{
   if (this_body() == this_object())
   {
      return 1;
   }

   return 0;
}