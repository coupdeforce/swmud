void remove_guild(string guild_name);
int query_experience();

mapping guild_levels = ([ ]);
mapping guild_ranks = ([ ]);
string primary_guild = "";
int primary_level = 0;

//:FUNCTION query_primary_guild
//Returns string with primary guild (ex., "jedi", "diplomat")
string query_primary_guild()
{
   return primary_guild;
}

//:FUNCTION set_primary_guild
//Parameter is guild name (ex., "jedi", "diplomat")
void set_primary_guild(string value)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s primary guild from " + primary_guild + " to " + value, previous_object(-1)); }

   if (strlen(value))
   {
      value = lower_case(value);
      primary_guild = value;
      primary_level = guild_levels[primary_guild];
   }
}

//:FUNCTION reset_primary_guild
//Clears primary guild name
void reset_primary_guild()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s primary guild", previous_object(-1)); }

   primary_guild = "";
   primary_level = 0;
}

//:FUNCTION query_primary_level
//Returns primary level (ex., 1, 50)
int query_primary_level()
{
   if (strlen(primary_guild) && guild_levels[primary_guild])
   {
      return guild_levels[primary_guild];
   }

   return 0;
}

//:FUNCTION set_primary_level
//Parameter is from 0 to 50
void set_primary_level(int value)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s primary level to " + value, previous_object(-1)); }

   if ((value >= 0) && (value <= 50))
   {
      int primary_level = 0;

      if (strlen(primary_guild) && guild_levels[primary_guild])
      {
         primary_level = guild_levels[primary_guild];
      }

      if (primary_level > 0)
      {
         if (value > 0)
         {
            guild_levels[primary_guild] = value;
         }
         else if (value < 1)
         {
            foreach (string guild_name in keys(guild_levels))
            {
               remove_guild(guild_name);
            }
         }
      }
   }
}

//:FUNCTION query_guild_names
//Returns array of all guild names
string array query_guild_names()
{
   return keys(guild_levels);
}

//:FUNCTION query_guild_levels
//Returns array of all guild levels
int array query_guild_levels()
{
   return values(guild_levels);
}

//:FUNCTION query_guilds
//Returns mapping of all guild names and levels
mapping query_guilds()
{
   return guild_levels;
}

//:FUNCTION query_guild_level
//Returns guild level (ex., 1, 50) with guild name as parameter
int query_guild_level(string guild_name)
{
   if (guild_levels[guild_name])
   {
      return guild_levels[guild_name];
   }

   return 0;
}

//:FUNCTION query_guild_rank
//Returns guild rank (ex., 1, 2, 3, 4) with guild name as parameter
int query_guild_rank(string guild_name)
{
   if (guild_name == primary_guild)
   {
      if (guild_levels[guild_name] > 20)
      {
         if (guild_ranks[guild_name] > 2)
         {
            if (guild_ranks[guild_name] >= 4)
            {
               if ((guild_name == "jedi") && this_object()->is_body())
               {
                  if (!JEDI_RANK_D->is_highest_rank(this_object()->query_userid(), this_object()->query_jedi_alignment()))
                  {
                     return 3;
                  }
               }

               return 4;
            }

            return guild_ranks[guild_name];
         }

         return 2;
      }

      if (guild_ranks[guild_name] >= 1)
      {
         return guild_ranks[guild_name];
      }

      guild_ranks[guild_name] = 1;

      return 1;
   }
   else if (guild_levels[guild_name])
   {
      if ((guild_ranks[guild_name] >= 2) || (guild_levels[guild_name] > 20))
      {
         return 2;
      }

      return 1;
   }

   return 0;
}

//:FUNCTION set_guild_level
//Parameters are guild name, guild level
void set_guild_level(string guild_name, int guild_level)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s " + guild_name + " level from " + guild_levels[guild_name] + " to " + guild_level, previous_object(-1)); }

   if (strlen(guild_name) && (guild_level > 0) && (guild_level <= 50))
   {
      guild_name = lower_case(guild_name);
      guild_levels[guild_name] = guild_level;

      if (guild_name == primary_guild)
      {
         primary_level = guild_level;
      }
   }
}

//:FUNCTION set_guild_rank
//Parameters are guild name, guild rank
void set_guild_rank(string guild_name, int guild_rank)
{
   if (strlen(guild_name) && (member_array(guild_name, keys(guild_levels)) > -1))
   {
      if (guild_rank < 1)
      {
         guild_rank = 1;
      }
      else if (guild_rank > 4)
      {
         guild_rank = 4;
      }

      guild_name = lower_case(guild_name);
      guild_ranks[guild_name] = guild_rank;
   }
}

//:FUNCTION advance_guild_level
//Parameter is guild name
void advance_guild_level(string guild_name)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("advance " + this_object()->short() + "'s level in " + guild_name + " from " + guild_levels[guild_name] + " to " + (guild_levels[guild_name] + 1), previous_object(-1)); }

   if (strlen(guild_name))
   {
      guild_name = lower_case(guild_name);

      if (guild_levels[guild_name])
      {
         guild_levels[guild_name] += 1;
      }
      else
      {
         guild_levels[guild_name] = 1;
      }
   }
}

//:FUNCTION remove_guild
//Parameter is guild name
void remove_guild(string guild_name)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("remove " + this_object()->short() + "'s guild membership in " + guild_name, previous_object(-1)); }

   if (strlen(guild_name))
   {
      guild_name = lower_case(guild_name);
      map_delete(guild_levels, guild_name);
      map_delete(guild_ranks, guild_name);

      if (primary_guild == guild_name)
      {
         primary_guild = "";
      }

      foreach (string secondary_guild in keys(guild_levels))
      {
         if (guild_levels[secondary_guild] > guild_levels[primary_guild])
         {
            primary_guild = secondary_guild;
            primary_level = guild_levels[secondary_guild];
         }
      }
   }
}

//:FUNCTION reset_guilds
void reset_guilds()
{
   guild_levels = ([ ]);
   guild_ranks = ([ ]);
   primary_guild = "";
}