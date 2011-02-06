// Last edited by deforce on 03-28-2010
#include <verbs.h>

inherit VERB_OB;
inherit M_GRAMMAR;

string inspect_corpse(object ob);

void do_look()
{
   this_body()->force_look(1);

   if (this_body()->has_status_line()) { this_body()->update_status_line(); }
}

void do_look_at_obj(object ob, string name)
{
   string str;

   if (!(str = ob->get_item_desc(name))) { str = ob->long(); }

   if (sizeof(str) && (str[<1] != '\n')) { str += "\n"; }

   if (ob->is_corpse() && this_body()->query_guild_level("bounty hunter")
      && this_body()->has_learned_skill("inspect corpse") && !ob->has_been_inspected())
   {
      str += inspect_corpse(ob);
   }

   write(str);
}

void do_look_at_obs(array info, string name)
{
   handle_obs(info, (: do_look_at_obj :), name);
}

void do_look_in_obj(object ob)
{
   write(ob->look_in("in") + "\n");
}

void do_look_on_obj(object ob)
{
   write(ob->look_in("on") + "\n");
}

void do_look_obj(object ob, string name)
{
   if (ob == this_body())
   {
      this_body()->other_action("$N $vlook at $r.");
   }
   else if (ob->is_living() && !wizardp(ob))
   {
      string sex_race = ob->query_gender_string() + " " + title_capitalize(ob->query_race());

      this_body()->my_action("$N $vlook over the " + sex_race + ".");
      ob->my_action("$O looks at $n.", this_body());

      tell(all_inventory(environment(this_body())) - ({ this_body(), ob }), "\n" + this_body()->short() + " looks at " + ob->short() + ".\n");
   }
   else
   {
      if (ob->is_living())
      {
         this_body()->targetted_action("$N $vlook at $t.", ob);
      }
      else
      {
         this_body()->other_action("$N $vlook at $o.", ob);
      }
   }

   do_look_at_obj(ob, name);
}

void do_look_obs(array info, string name)
{
   if (name == "all")
   {
      write("You get confused trying to look at everything all at once.\n");
      return;
   }

   handle_obs(info, (: do_look_at_obj :), name);
}

void do_look_at_obj_with_obj(object o1, object o2)
{
   // o2->indirect_look_at_obj_with_obj() has already indicated this is ok.
   o2->use("look", o1);
}

void do_look_at_obs_with_obj(array info, object o2)
{
   handle_obs(info, (: do_look_at_obj_with_obj :), o2);
}

void do_look_obj_with_obj(object o1, object o2)
{
   do_look_at_obj_with_obj(o1, o2);
}

void do_look_obs_with_obj(array info, object o2)
{
   handle_obs(info, (: do_look_at_obj_with_obj :), o2);
}

void do_look_for_obj(object ob)
{
   object env = environment(ob);
   string relation;

   if (!env)
   {
      // The room
      write("You're on it!\n");
      return;
   }

   if (ob == this_body())
   {
      write("Trying to find yourself?\n");
      return;
   }

   if (environment(this_body()) == env)
   {
      // We need a way to conjugate this
      this_body()->my_action("The $o0 is right here!", ob);
      return;
   }

   if(env->is_living())
   {
      this_body()->my_action("$O $vis carrying it.", env);
      return;
   }
   else
   {
      relation = env->query_prep(ob);

      // printf("%O %O\n", ob, env);
      this_body()->my_action("The $o0 is " + relation + " the $o1.", ob, env);
   }
}

string look_for_phrase(object ob)
{
   object env = environment(ob);

   if (env == environment(this_body())) { return "on the ground"; }
   else if (env->is_living()) { return "carried by " + env->the_short(); }

   return env->query_prep(ob) + " " + env->the_short();
}

void do_look_for_obs(array info)
{
   mixed ua;
   int i, n;
   string res;

   // Ignore errors
   info = filter(info, (: objectp :));
   ua = unique_array(info, (: look_for_phrase :));

   n = sizeof(ua);
   res = "There " + (sizeof(ua[0]) > 1 ? "are " : "is ");
   for (i = 0; i < n; i++)
   {
      if (i != 0)
      {
         if (i == (n - 1))
         {
            res += " and ";
         }
         else
         {
            res += ", ";
         }

         res += number_word(sizeof(ua[i])) + " " + look_for_phrase(ua[i][0]);
      }
   }

   write(res + ".\n");
}

string inspect_corpse(object ob)
{
   object this_body = this_body();
   string text = "";

   if (this_body->test_skill("inspect_corpse", this_body->query_guild_level("bounty hunter") * 12))
   {
      text += "It looks like they were killed " + convert_time(time() - ob->query_death_time()) + " ago.\n";

      if (this_body->test_skill("inspect_corpse", this_body->query_guild_level("bounty hunter") * 8))
      {
         if (ob->query_killer_weapon() == "bare hands")
         {
            text += "It looks like the killer used their bare hands.\n";
         }
         else
         {
            text += "It looks like they were killed with " + add_article(ob->query_killer_weapon()) + ".\n";
         }

         if (this_body->test_skill("inspect_corpse", this_body->query_guild_level("bounty hunter") * 4))
         {
            text += "This murder looks like the work of " +  ob->query_killer() + ".\n";
         }
         else
         {
            text += "This murder doesn't seem to match anyone's modus operandi.\n";
         }
      }
   }
   else if (random(this_body->query_skill("inspect_corpse")) < (110 - this_body->query_per()))
   {
      ob->set_inspected();

      text += "You gain nothing from inspection, and the evidence is ruined.\n";
   }
   else
   {
      text += "Upon inspection, you discover nothing related to the murder.\n";
   }

   return text;
}

void create()
{
   clear_flag(NEED_TO_BE_ALIVE);

   add_rules( ({ "", "in OBJ", "on OBJ", "at OBJ", "for OBS", "at OBS with OBJ" }) );
   add_rules( ({ "OBS", "OBS with OBJ" }) );
}