// Last edited by deforce on 10-21-2009
#include <flags.h>
#include <hooks.h>

void simple_action(string, object);
int query_health(string);
string query_dominant_hand();
string query_other_hand();
varargs mixed call_hooks(string, mixed, mixed, array...);

private mapping weapons = ([ ]);

string array query_wielding_limbs();

string find_an_open_limb()
{
   foreach(string limb in query_wielding_limbs())
   {
      if (!weapons[limb] && query_health(limb)) { return limb; }
   }

   return 0;
}

varargs void wield(object ob, string limb)
{
   if (weapons[limb] == ob) { return; }

   // If limb is zero, we use an open limb, or a random one if we are "full"
   if (!limb) { limb = find_an_open_limb(); }

   if (!limb)
   {
      array limbs = filter(query_wielding_limbs(), (: query_health($1) > 0 :));

      if (!sizeof(limbs))
      {
         // Houston, we have a problem.
         if (this_object()->query_link()) { write("You have no way of wielding that!\n"); }

         return;
      }

      limb = choice(limbs);
   }

   if (weapons[limb])
   {
      weapons[limb]->mark_wielded_by(0);
   }

   weapons[limb] = ob;

   if (ob->query("two_hands"))
   {
      ob->mark_wielded_by(this_object(), query_dominant_hand() + " hand", query_other_hand() + " hand");
   }
   else
   {
      ob->mark_wielded_by(this_object(), limb);
   }

   if (ob->test_flag(F_CONCEALED))
   {
      ob->clear_flag(F_CONCEALED);
      this_body()->simple_action("$N $vpull out " + ob->a_short() + " from the folds of $p clothing.\n");
   }

   call_hooks("on_wield", HOOK_IGNORE);
   simple_action(ob->query_wield_message(), ob);
}

varargs void unwield(string limb)
{
   if (!limb)
   {
      foreach (limb in query_wielding_limbs())
      {
         unwield(limb);
      }

      return;
   }

   if (weapons[limb])
   {
      weapons[limb]->mark_wielded_by(0);
   }

   map_delete(weapons, limb);
   call_hooks("on_unwield", HOOK_IGNORE);
}

varargs object query_weapon(string limb)
{
   if (!limb)
   {
      array limbs = filter(query_wielding_limbs(), (: weapons[$1] :));

      if (!sizeof(limbs))
      { 
         return this_object();  // return 0? 
      }

      if (weapons[query_dominant_hand() + " hand"])
      {
         return weapons[query_dominant_hand() + " hand"];
      }
      else if (weapons[query_other_hand() + " hand"])
      {
         return weapons[query_other_hand() + " hand"];
      }

      limb =  choice(limbs);
   }

   return weapons[limb];
}

object* query_weapons()
{
   string limb;
   array ret = ({ });
   array limbs = filter(query_wielding_limbs(), (: weapons[$1] :));

   if (!sizeof(limbs))
   {
      return ({ this_object() });
   }

   foreach (limb in limbs)
   {
      ret += ({ weapons[limb] });
   }

   return ret;
}

varargs int do_wield(object ob, string limb)
{
   if (!(ob->valid_wield())) { return 0; }

   if (ob->query_wielded_by() == this_object()) { return 0; }

   wield(ob, limb);

   return 1;
}

int do_unwield(mixed limb)
{
   if (objectp(limb))
   {
      array limbs = filter(query_wielding_limbs(), (: weapons[$1] == $(limb) :));

      if (!sizeof(limbs)) { return 0; }

      limb = limbs[0];
   }

   if (!weapons[limb] || (weapons[limb] == this_object())) { return 0; }

   if (!(weapons[limb]->valid_unwield())) { return 0; }

   simple_action(weapons[limb]->query_unwield_message(), weapons[limb]);
   unwield(limb);

   return 1;
}

// There's probably a better way to do this.. also, M_MESSAGES should probably be used here soon. (Iizuka)
string get_wield_attributes()
{
   string ret = "";
   string array wielding_limbs = previous_object()->query_wielding();

   if (previous_object()->query("two_hands"))
   {
      return sprintf(" (wielded in both hands)");
   }
   else if (sizeof(wielding_limbs))
   {
      return sprintf(" (wielded in %s)", wielding_limbs[0]);
   }

   return " (wielded)";
}