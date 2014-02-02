#include <mudlib.h>
#include <config.h>

inherit CMD;

#include <guild_colors.h>

int sort_level_name(string first, string second);

private void main()
{
   object body = this_body();
   string primary_guild = body->query_primary_guild();
   string array guild_names = body->query_guild_names();
   mapping guild_levels = ([ ]);
   int experience = body->query_experience();

   if (!sizeof(guild_names))
   {
      out("You are not a member of any guilds.\n");

      return;
   }

   foreach (string guild in guild_names)
   {
      guild_levels[guild] = body->query_guild_level(guild);
   }

   guild_names -= ({ primary_guild });

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      guild_names[count] = sprintf("%02d", guild_levels[guild_names[count]]) + ":::" + guild_names[count];
   }

   guild_names = sort_array(guild_names, "sort_level_name");

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      guild_names[count] = explode(guild_names[count], ":::")[1];
   }

   if (strlen(primary_guild))
   {
      if (guild_levels[primary_guild] < 50)
      {
         int required_experience = can_advance_guild_level(primary_guild, primary_guild, guild_levels);

         if (experience < required_experience)
         {
            out("You need " + (required_experience - experience) + " more experience to advance " + get_guild_color(primary_guild) + title_capitalize(primary_guild) + "%^RESET%^ to level " + (guild_levels[primary_guild] + 1) + ".\n");
         }
         else
         {
            out("%^BOLD%^You can advance %^RESET%^" + get_guild_color(primary_guild) + title_capitalize(primary_guild) + "%^RESET%^ %^BOLD%^to level " + (guild_levels[primary_guild] + 1) + ".%^RESET%^\n");
         }
      }
      else
      {
         out("You cannot advance " + get_guild_color(primary_guild) + title_capitalize(primary_guild) + "%^RESET%^ any further.\n");
      }
   }

   foreach (string guild in guild_names)
   {
      int required_experience = can_advance_guild_level(guild, primary_guild, guild_levels);

      if (required_experience > 0)
      {
         if (experience < required_experience)
         {
            out("You need " + (required_experience - experience) + " more experience to advance " + get_guild_color(guild) + title_capitalize(guild) + "%^RESET%^ to level " + (guild_levels[guild] + 1) + ".\n");
         }
         else
         {
            out("%^BOLD%^You can advance %^RESET%^" + get_guild_color(guild) + title_capitalize(guild) + "%^RESET%^ %^BOLD%^to level " + (guild_levels[guild] + 1) + ".%^RESET%^\n");
         }
      }
      else
      {
         out("You cannot advance " + get_guild_color(guild) + title_capitalize(guild) + "%^RESET%^ any further.\n");
      }
   }
}

int sort_level_name(string first, string second)
{
   int first_level = to_int(first[0..1]);
   int second_level = to_int(second[0..1]);
   first = first[2..];
   second = second[2..];

   if (first_level < second_level) { return 1; }
   else if (first_level > second_level) { return -1; }

   return strcmp(first, second);
}