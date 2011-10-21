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

            if (this_body->test_skill("bground check", (this_body->query_guild_level("diplomat") * 20)))
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

void get_info(object body)
{
   object this_body = this_body();
   int level = this_body->query_guild_level("diplomat");
   int rank = this_body->query_skill("bground check") / 100;
   int spec = this_body->query_guild_specialization_rank("diplomat", "something");
   int rank_spec = (rank + spec) < 0 ? 0 : (rank + spec);
   int health = this_body->query_max_health();
   int health_other = body->query_max_health();
   int array guilds = ({ });
   int array guilds_other = ({ });
   string array guild_list = ({ "assassin", "bounty hunter", "diplomat", "jedi", "engineer", "mercenary", "merchant", "physician", "pilot", "slicer", "smuggler" });
   string array guild_desc = ({ "lethality", "tracking skill", "political acumen", "strength in the Force", "engineering aptitude", "combat training", "business sense", "medical knowledge", "piloting ability", "technological proficiency", "capacity for clandestine dealings" });
   int array stats = ({ });
   int array stats_other = ({ });
   string array stat_list = ({ "str", "con", "agi", "dex", "per", "int", "cha", "luc", "for" });
   string array stat_desc = ({ "strength", "constitution", "agility", "dexterity", "perception", "intelligence", "charisma", "luck", "connection to the force" });
   string output = "\nYou find out the following about " + body->short() + " in comparison to yourself:\n\n";
   output += "Your vitality is ";

   switch (health * 100 / (health_other * (level + rank_spec + 70) / 140))
   {
      case 0..25: output += "very insignificant";  break;
      case 26..40: output += "somewhat insignificant";  break;
      case 41..60: output += "much weaker";  break;
      case 61..80: output += "somewhat weaker";  break;
      case 81..99: output += "slightly weaker";  break;
      case 100: output += "exactly equal";  break;
      case 101..140: output += "slightly better";  break;
      case 141..180: output += "somewhat better";  break;
      case 181..220: output += "much greater";  break;
      case 221..260: output += "superior";  break;
      default: output += "far superior";
   }

   output += ".\n\n";

   foreach (string guild in guild_list)
   {
      guilds += ({ call_other(this_body, "query_guild_level", guild) });
      guilds_other += ({ call_other(body, "query_guild_level", guild) });
   }

   for (int count = 0; count < sizeof(guild_desc); count++)
   {
      if (guilds[count] || guilds_other[count])
      {
         output += "Your " + guild_desc[count] + " is ";

         switch (guilds[count] - (guilds_other[count] * (level + rank_spec + 30) / 100))
         {
            case -100..-31: output += "very insignificant";  break;
            case -30..-21: output += "somewhat insignificant";  break;
            case -20..-11: output += "much weaker";  break;
            case -10..-4: output += "weaker";  break;
            case -3..-2: output += "somewhat weaker";  break;
            case -1: output += "slightly weaker";  break;
            case 0: output += "exactly equal";  break;
            case 1: output += "slightly better";  break;
            case 2..3: output += "better";  break;
            case 4..10: output += "somewhat better";  break;
            case 11..20: output += "much greater";  break;
            case 21..30: output += "superior";  break;
            default: output += "far superior";
         }

         output += ".\n";
      }
   }

   foreach (string stat in stat_list)
   {
      stats += ({ call_other(this_body, "query_" + stat) });
      stats_other += ({ call_other(body, "query_" + stat) });
   }

   output += "\n";

   for (int count = 0; count < sizeof(stat_desc); count++)
   {
      output += "Your " + stat_desc[count] + " is ";

      switch ((stats[count] - (stats_other[count] * (level + rank_spec + 130) / 200)) / 5)
      {
         case -100..-19: output += "very insignificant";  break;
         case -18..-14: output += "somewhat insignificant";  break;
         case -13..-9: output += "much weaker";  break;
         case -8..-4: output += "weaker";  break;
         case -3..-2: output += "somewhat weaker";  break;
         case -1: output += "slightly weaker";  break;
         case 0: output += "exactly equal";  break;
         case 1: output += "slightly better";  break;
         case 2..3: output += "better";  break;
         case 4..8: output += "somewhat better";  break;
         case 9..13: output += "much greater";  break;
         case 14..18: output += "superior";  break;
         default: output += "far superior";
      }

      output += ".\n";
   }

   this_body->other_action("$N $vfind out some interesting information about $O.\n", body);

   write(output);
}

void create()
{
   add_rules( ({ "", "LIV" }) );
}