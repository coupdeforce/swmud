// Last edited by deforce on 10-07-2009
inherit VERB_OB;

void get_info(object body);

void do_bgcheck_liv(object living)
{
   object this_body = this_body();

   if (this_body->query_guild_level("diplomat"))
   {
      if (!this_body->has_learned_skill("background checking"))
      {
         write("You have not learned how to check out someone's background.\n");
      }
      else if (this_body->get_target())
      {
         write("You are unable to get information while in combat.\n");
      }
      else if (this_body->has_skill_delay())
      {
         write("You are too busy to conduct a background check.\n");
      }
      else
      {
         if (living == this_body)
         {
            write("You find out that your mother was a bantha.\n");
         }
         else
         {
            this_body->add_skill_delay(2);

            if (this_body->test_skill("bground_check", (this_body->query_guild_level("diplomat") * 20)))
            {
               get_info(living);
            }
            else
            {
               this_body->targetted_action("$N $vtry to get some information about $t and $vfail.\n", living);
            }
         }
      }
   }
   else
   {
      write("Only diplomats know how to check out someone's background.\n");
   }
}

void do_bgcheck()
{
   write("Get information about what?\n");
}

string compare_basic(int first, int second)
{
   switch (to_int(((first + 1) / ((second + 1) * 1.0)) * 100))
   {
      case 0..90: return "worse";
      case 91..110: return "about equal";
      default: return "better";
   }
}

string compare_advanced(int first, int second)
{
   switch (to_int(((first + 1) / ((second + 1) * 1.0)) * 100))
   {
      case 0..25: return "very insignificant";
      case 26..40: return "somewhat insignificant";
      case 41..60: return "much weaker";
      case 61..80: return "somewhat weaker";
      case 81..99: return "slightly weaker";
      case 100: return "exactly equal";
      case 101..120: return "slightly better";
      case 121..140: return "somewhat better";
      case 141..160: return "much greater";
      case 161..175: return "superior";
      default: return "far superior";
   }

   return "incomparable";
}

void get_info(object body)
{
   object this_body = this_body();
   int leve = this_body->query_guild_level("diplomat");
   int rank = to_int(floor(this_body->query_skill("bground_check") / 100.0));
   int health = this_body->query_max_health();
   int health_other = body->query_max_health();
   string output = "\nYou find out the following about " + body->short() + " in comparison to yourself:\n\n";

   if (this_body->test_skill("bground_check", (this_body->query_guild_level("diplomat") * 16)))
   {
      output += "Your vitality is " + compare_advanced(health, health_other)  + ".\n\n";
   }
   else
   {
      output += "Your vitality is " + compare_basic(health, health_other)  + ".\n\n";
   }

   if (this_body->test_skill("bground_check", (this_body->query_guild_level("diplomat") * 12)))
   {
      int array guilds = ({ });
      int array guilds_other = ({ });
      string array guild_list = ({ "assassin", "bounty hunter", "diplomat", "jedi", "mechanic", "mercenary", "merchant", "physician", "pilot", "slicer", "smuggler" });
      string array guild_desc = ({ "lethality", "tracking skill", "political acumen", "strength in the Force", "engineering aptitude", "combat training", "business sense", "medical knowledge", "piloting ability", "technological proficiency", "capacity for clandestine dealings" });

      foreach (string guild in guild_list)
      {
         guilds += ({ call_other(this_body, "query_guild_level", guild) });
         guilds_other += ({ call_other(body, "query_guild_level", guild) });
      }

      if (this_body->test_skill("bground_check", (this_body->query_guild_level("diplomat") * 8)))
      {
         for (int count = 0; count < sizeof(guild_desc); count++)
         {
            if (((rank >= 10) && (guilds_other[count] > 0))
               || (rank < 10))
            {
               output += "Your " + guild_desc[count] + " is " + compare_advanced(guilds[count], guilds_other[count])  + ".\n";
            }
         }

         if (this_body->test_skill("bground_check", (this_body->query_guild_level("diplomat") * 4)))
         {
            int array stats = ({ });
            int array stats_other = ({ });
            string array stat_list = ({ "str", "con", "agi", "dex", "per", "int", "cha", "luc", "for" });
            string array stat_desc = ({ "strength", "constitution", "agility", "dexterity", "perception", "intelligence", "charisma", "fortune", "connection to the force" });

            foreach (string stat in stat_list)
            {
               stats += ({ call_other(this_body, "query_" + stat) });
               stats_other += ({ call_other(body, "query_" + stat) });
            }

            output += "\n";

            if (this_body->test_skill("bground_check", this_body->query_guild_level("diplomat")))
            {
               for (int count = 0; count < sizeof(stat_desc); count++)
               {
                  if (stats_other[count] > 0)
                  {
                     output += "Your " + stat_desc[count] + " is " + compare_advanced(stats[count], stats_other[count])  + ".\n";
                  }
               }
            }
            else
            {
               for (int count = 0; count < sizeof(stat_desc); count++)
               {
                  output += "Your " + stat_desc[count] + " is " + compare_basic(stats[count], stats_other[count])  + ".\n";
               }
            }
         }
      }
      else
      {
         for (int count = 0; count < sizeof(guild_desc); count++)
         {
            output += "Your " + guild_desc[count] + " is " + compare_basic(guilds[count], guilds_other[count])  + ".\n";
         }
      }
   }

   this_body->other_action("$N $vfind out some interesting information about $O.\n", body);

   write(output);
}

void create()
{
   add_rules( ({ "", "LIV" }) );
}