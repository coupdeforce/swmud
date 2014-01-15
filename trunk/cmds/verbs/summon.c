inherit VERB_OB;

void summon_bodyguard(object bodyguard, object player)
{
   bodyguard->move(environment(player));
   bodyguard->simple_action("$N $varrive.");
}

void do_summon_str(string str)
{
   if (str == "bodyguard")
   {
      object array bodyguards = objects((: base_name($1) == $("/d/mon/bodyguard") :));
      int index = member_array(this_body(), bodyguards->query_owner());

      if (index >= 0)
      {
         call_out("summon_bodyguard", 16, bodyguards[index], this_body());
         write("You summon your hired bodyguard.\n");
         return;
      }

      write("You have not hired a bodyguard.\n");
      return;
   }

   write("You don't know how to summon a \"" + str + "\".\n");
}

void do_summon()
{
   write("Summon whom?\n");
}

void create()
{
   add_rules( ({ "", "STR" }) );
}