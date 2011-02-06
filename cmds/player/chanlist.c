// Last modified by deforce on 11-11-2009
#include <mudlib.h>
#include <channel.h>

inherit CMD;

#include <channel_associations.h>

#define WIDTH this_user()->query_screen_width()

void main()
{
   string array channels = sort_array(CHANNEL_D->query_channels(), 1);
   string array lines = ({ });
   float columns = 1;
   int line_count;

   if (wizardp(this_user())) { columns = floor(WIDTH / 33); }

   if (columns > 1) { columns = floor((WIDTH - ((columns - 1) * 4)) / 33); }

   if (sizeof(channels))
   {
      string array listening = this_user()->query_channel_list();
      string output;
      int flags;

      foreach (string chan in channels)
      {
         flags = CHANNEL_D->query_flags(chan);

         if (flags & CHANNEL_ADMIN_ONLY)
         {
            if (adminp(this_user()) || (SECURE_D->query_is_wizard(this_user()->query_userid()) > 2))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "admin" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
         }
         else if (flags & CHANNEL_WIZ_ONLY)
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "wiz" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
         }
         else if (flags & CHANNEL_TEAM_ONLY)
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "team" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
            else if (channel_teams[this_body()->query_team()] == chan)
            {
               output = sprintf("%-12s  %-6s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"));
            }
         }
         else if (flags & CHANNEL_GUILD_ONLY)
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "guild" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
            else if ((this_body()->query_primary_guild() == chan)
               || (channel_guilds[this_body()->query_primary_guild()] == chan)
               || (this_body()->query_guild_rank(guild_channels[chan]) >= 2))
            {
               output = sprintf("%-12s  %-6s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"));
            }
         }
         else if (flags & CHANNEL_RACE_ONLY)
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "race" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
            else if ((this_body()->query_race() == chan)
               || (channel_races[this_body()->query_race()] == chan))
            {
               output = sprintf("%-12s  %-6s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"));
            }
         }
         else if (flags & CHANNEL_PK_ONLY)
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  "pk" + ((flags & CHANNEL_PERMANENT) ? ", perm" : ", temp"));
            }
            else if (this_body()->is_player_killer())
            {
               output = sprintf("%-12s  %-6s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"));
            }
         }
         else
         {
            if (wizardp(this_user()))
            {
               output = sprintf("%-12s  %-6s  %-11s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"),
                  ((flags & CHANNEL_PERMANENT) ? "perm" : "temp")
                     + ((flags & PLAYER_SILENT) ? ", ps" : ""));
            }
            else
            {
               output = sprintf("%-12s  %-6s", chan,
                  ((member_array(chan, listening) > -1) ? "on" : "off"));
            }
         }

         if (strlen(output))
         {
            lines += ({ output });
            output = "";
         }
      }

//      lines = sort_array(lines, 1);
      line_count = ceil(sizeof(lines) / columns);

      if ((columns > 1) && (sizeof(lines) > 1) && wizardp(this_user()))
      {
         out(sprintf("\n\n%-12s  %-6s  %-11s", "Name", "Status", "Options") + repeat_string("     " + sprintf("%-12s  %-6s  %-11s", "Name", "Status", "Options"), columns - 1) + "\n");
         out(repeat_string("-", 33) + repeat_string("     " + repeat_string("-", 33), columns - 1) + "\n");
      }
      else
      {
         if (wizardp(this_user()))
         {
            out(sprintf("\n\n%-12s  %-6s  %-11s\n", "Name", "Status", "Options"));
            out(repeat_string("-", 33) + "\n");
         }
         else
         {
            out(sprintf("\n\n%-12s  %-6s\n", "Name", "Status"));
            out(repeat_string("-", 20) + "\n");
         }
      }

      for (int count = 0; count < line_count; count++)
      {
         for (int column_count = 0; column_count < columns; column_count++)
         {
            int index = count + (line_count * column_count);

            if (index < sizeof(lines))
            {
               out((column_count > 0 ? "     " : "") + lines[index]);
            }
         }

         out("\n");
      }
   }
}