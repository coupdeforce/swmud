// Last edited by deforce on 10-02-2009
inherit __DIR__ "death_messages";

#include <flags.h>

void remove();
void simple_action(string);
string query_name();
varargs void stop_fight(object);
void stop_attacking();
void clear_flag(int);
object query_last_weapon_used();

private nosave string corpse_long;
private nosave string corpse_filename = CORPSE;

nomask string query_default_death_message()
{
   return "$N $vbreathe $p last breath and $vslump to the ground, dead.";
}

//:FUNCTION set_corpse_long
// void set_corpse_long(string str);
// Sets the long description of this adversary's corpse.
void set_corpse_long(string str)
{
   corpse_long = str;
}

//:FUNCTION set_corpse_filename
// void set_corpse_filename(string str);
// Sets the corpse to be cloned to 'str'.
void set_corpse_filename(string str)
{
   corpse_filename = str;
}

//:FUNCTION on_death
// Overload this for doing things after death that you want to do before drop_corpse().
void on_death()
{
}

//:FUNCTION drop_corpse
// Replace us with a corpse.
void drop_corpse(string killer, string killer_weapon)
{
   object corpse = new(corpse_filename, query_name(), corpse_long, this_object(), killer, killer_weapon);

   foreach (object thing in all_inventory())
   {
      thing->do_remove();
      thing->do_unwield();
      thing->clear_flag(F_WORN);
      thing->clear_flag(F_WIELDED);
      thing->clear_flag(F_HELD);
      thing->move(corpse);
   }

   corpse->move(environment());
}

protected void die()
{
   object this_object = this_object();
   object killer = previous_object();
   string weapon = this_object->query_last_weapon_used()->is_adversary() ? "bare hands" : this_object->query_last_weapon_used()->short();
   string killer_name = killer->short();
   string location = environment(this_object())->short();
   string death_message;

   if (killer && (killer != this_object()) && (environment(killer) != this_object()))
   {
      death_message = capitalize(this_object->short()) + " was killed by " + killer_name + " with " + killer->query_possessive() + " " + weapon + " at " + location + ".";
   }
   else
   {
      death_message = capitalize(this_object()->short()) + " inadvertently killed " + this_object()->query_reflexive() + " in a moment of carelessness with " + this_object()->query_possessive() + " " + weapon + " at " + location + ".";
   }

   stop_fight(0);
   stop_attacking();
   simple_action(query_room_death_message());

   CHANNEL_D->deliver_channel("death-npc", death_message);

   killer->add_experience(this_object()->query_experience());

   // Alignment adjustment for jedi
   if ((this_object()->query_jedi_alignment() != 0) && (killer->query_guild_level("jedi")))
   {
      if (killer->query_jedi_alignment() > 0)
      {
         int my_alignment = this_object()->query_jedi_alignment();

         if (my_alignment > 0)
         {
            killer->adjust_jedi_alignment(to_int(floor(my_alignment / 1.25)) * -1);
         }
         else
         {
            killer->adjust_jedi_alignment(to_int(floor(my_alignment / 2.0)));
         }
      }
      else if ((killer->query_jedi_alignment() < 0) && (this_object()->query_jedi_alignment() > 0))
      {
         killer->adjust_jedi_alignment(to_int(floor(this_object()->query_jedi_alignment() / 4.0)) * -1);
      }
   }

   on_death();
   drop_corpse(killer->short(), weapon); // subtle pun
   call_out("remove", 1);
}