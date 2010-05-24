// Last edited by deforce on 05-11-2010
inherit WEAPON;
//inherit M_LIGHT_SOURCE;

#include <lightsaber.h>

string color = choice(({ "blue", "green", "yellow", "red" }));
int deflection_bonus = 0;
int alignment = 0;

void mudlib_setup()
{
   add_id("lightsaber", "saber");
   set_combat_messages("combat-blade");
   set_skill_used("saber_combat");
   add_save(({ "color", "deflection_bonus", "alignment" }));
   ::mudlib_setup();
}

int is_lightsaber() { return 1; }

string query_wield_message()
{
//   light();

   clear_adj();
   call_out("add_adj", 1, get_lightsaber_color_code(color) + "%^RESET%^");

   return "$N $vactivate $p $o, which produces a glowing " + get_lightsaber_color_code(color) + "%^RESET%^ blade.";
}

string query_unwield_message()
{
//   extinguish();

   remove_adj(get_lightsaber_color_code(color) + "%^RESET%^");

   return "$N $vshut down $p $o.";
}

void set_lightsaber_color(string lightsaber_color)
{
   color = lightsaber_color;
}

void add_deflection_bonus(int amount)
{
   deflection_bonus += amount;
}

int query_deflection_bonus()
{
   return deflection_bonus;
}

void reset_deflection_bonus()
{
   deflection_bonus = 0;
}

void set_lightsaber_alignment(int align)
{
   alignment = align;
}

mixed direct_attach_obj_to_obj() { return 1; }
mixed direct_detach_obj_from_obj() { return 1; }
mixed direct_holster_obj_to_obj() { return 0; }
mixed direct_unholster_obj_from_obj() { return 0; }

int valid_wield()
{
   if (!strlen(color))
   {
      tell(owner(this_object()), "You are unable to wield your " + this_object()->short() + " while it has no color crystal.\n");

      return 0;
   }
   else if ((alignment > 0) && (owner(this_object())->query_jedi_alignment() < 0))
   {
      tell(owner(this_object()), "You are unable to wield your " + this_object()->short() + " while you are too far from the Light side of the Force.\n");

      return 0;
   }
   else if ((alignment < 0) && (owner(this_object())->query_jedi_alignment() > 0))
   {
      tell(owner(this_object()), "You are unable to wield your " + this_object()->short() + " while it is tainted by the Dark side of the Force.\n");

      return 0;
   }

   return 1;
}

class event_info source_modify_event(class event_info evt)
{
   object this_body = query_wielded_by();
   int level = this_body->query_guild_level("jedi");

   if (stringp(evt->data)) { return evt; }

   if ((level > 0) && this_body->has_learned_skill("lightsaber combat") && this_body->can_feel_force())
   {
      int rank = this_body->query_skill("saber_combat") / 100;
      int spec = this_body->query_guild_specialization_rank("jedi", "lightsaber");
      int rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);
      int min_damage = 0 + (level / (15 - rank)) + rank_spec;
      int damage = evt->data["energy"];
      int max_skill_damage = damage * (40 + (rank * 6)) / 100;

      if (min_damage < 0) { min_damage = 0; }

//      tell(this_body, sprintf("Original Damage: %i\nMin Damage: %i\nMax Skill Damage: %i\n", evt->data["energy"], min_damage, max_skill_damage));

      if (damage < min_damage)
      {
         damage = min_damage;
      }
      else
      {
         max_skill_damage = (max_skill_damage >= min_damage ? max_skill_damage : min_damage);

         if (damage > max_skill_damage)
         {
            damage = max_skill_damage;
         }
      }

//      tell(this_body, sprintf("Damage: %i\n", damage));

      if (this_body->has_learned_skill("combat sense") && this_body->test_skill("combat_sense"))
      {
         int force = this_body->query_for();
         rank += this_body->query_skill("combat_sense") / 100;

         if (spec <= 0) { spec = this_body->query_guild_specialization_rank("jedi", "sense"); }

         rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);

         if (((level * 4) + force + (rank_spec * 10)) > random(1500))
         {
            damage += damage * level * force * rank_spec / 75000;

            this_body->simple_action("$N $vdeliver a critical strike with $p lightsaber!");

//            tell(this_body, sprintf("Critical Damage: %i\n", damage));
         }
      }
/*
      if (damage != evt->data["energy"])
      {
         tell(this_body, sprintf("Damage adjusted by %i.\n", (damage - evt->data["energy"])));
      }
*/
      evt->data["energy"] = damage;
   }
   else if ((this_body->query_agi() + this_body->query_dex() + this_body->query_per() + this_body->query_luc() + this_body->query_for()) > random(500))
   {
      evt->data["energy"] = evt->data["energy"] * (this_body->query_agi() + this_body->query_dex() + this_body->query_per() + this_body->query_luc() + this_body->query_for()) / 500;
   }
   else
   {
      evt->data = "miss";
   }

   return evt;
}