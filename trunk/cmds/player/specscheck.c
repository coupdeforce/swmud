// Last edited by deforce on 05-08-2010
inherit CMD;

private void main()
{
   object body = this_body();
   string primary_guild = body->query_primary_guild();
   string primary_guild_specialization = body->query_guild_specialization(primary_guild);
   int primary_guild_level = body->query_guild_level(primary_guild);
   int primary_guild_rank = body->query_guild_specialization_rank(primary_guild, primary_guild_specialization);
   string array guild_names = sort_array(body->query_guild_names() - ({ primary_guild }), 1);
   string array guild_specializations = ({ });
   int array guild_ranks = ({ });
   int experience = body->query_experience();
   int count;

   if (!strlen(primary_guild) && !sizeof(guild_names))
   {
      out("You are not a member of any guilds.\n");

      return;
   }

   for (count = 0; count < sizeof(guild_names); count++)
   {
      guild_specializations += ({ body->query_guild_specialization(guild_names[count]) });
      guild_ranks += ({ body->query_guild_specialization_rank(guild_names[count], guild_specializations[count]) });
   }

   out("\n Guild          Specialization     Rank    Experience needed for next rank\n" + repeat_string("-", 75) + "\n");

   if (strlen(primary_guild))
   {
      if (strlen(primary_guild_specialization))
      {
         if (primary_guild_rank < 10)
         {
            int next_rank = primary_guild_rank + 1;
            int required_experience = 500000 - (primary_guild_level * 50000) + (primary_guild_level * next_rank * 500);

            if (experience < required_experience)
            {
               outf("  %-13s  %-18s %-2i      %i\n", title_capitalize(primary_guild), title_capitalize(primary_guild_specialization), primary_guild_rank, (required_experience - experience));
            }
            else
            {
               outf("%%^BOLD%%^  %-13s  %-18s %-2i      0%%^RESET%%^\n", title_capitalize(primary_guild), title_capitalize(primary_guild_specialization), primary_guild_rank);
            }
         }
         else
         {
            outf("  %-13s  %-17s  %-2i\n", title_capitalize(primary_guild), title_capitalize(primary_guild_specialization), primary_guild_rank);
         }
      }
      else
      {
         int required_experience = 500000 - (primary_guild_level * 50000) + (primary_guild_level * 500);

         if (experience < required_experience)
         {
            outf("  %-13s  %-17s  0       %i\n", title_capitalize(primary_guild), "None", (required_experience - experience));
         }
         else
         {
            outf("%%^BOLD%%^  %-13s  %-17s  0       0%%^RESET%%^\n", title_capitalize(primary_guild), "None");
         }
      }
   }

   for (count = 0; count < sizeof(guild_names); count++)
   {
      string guild_name = guild_names[count];
      string guild_specialization = guild_specializations[count];
      int guild_level = body->query_guild_level(guild_name);
      int guild_rank = guild_ranks[count];

      if (strlen(guild_specialization))
      {
         if (guild_rank < 10)
         {
            int next_rank = guild_rank + 1;
            int required_experience = 2500000 - (guild_level * 50000) + (guild_level * next_rank * 500);

            if (experience < required_experience)
            {
               outf("  %-13s  %-17s  %-2i      %i\n", title_capitalize(guild_name), title_capitalize(guild_specialization), guild_rank, (required_experience - experience));
            }
            else
            {
               outf("%%^BOLD%%^  %-13s  %-17s  %-2i      0%%^RESET%%^\n", title_capitalize(guild_name), title_capitalize(guild_specialization), guild_rank);
            }
         }
         else
         {
            outf("  %-13s  %-17s  %-2i\n", title_capitalize(guild_name), title_capitalize(guild_specialization), guild_rank);
         }
      }
      else
      {
         int required_experience = 2500000 - (guild_level * 50000) + (guild_level * 500);

         if (experience < required_experience)
         {
            outf("  %-13s  %-17s  0       %i\n", title_capitalize(guild_name), "None", (required_experience - experience));
         }
         else
         {
            outf("%%^BOLD%%^  %-13s  %-17s  0       0%%^RESET%%^\n", title_capitalize(guild_name), "None");
         }
      }
   }
}