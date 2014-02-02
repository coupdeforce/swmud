inherit CMD;
inherit CLASS_PLAYER_GUILD_DATA;

#include <jedi_ranks.h>

void sponsor(string player_name);
void list_sponsored();

void main(string player_name)
{
   if (this_body()->query_guild_level("jedi"))
   {
      if (this_body()->query_guild_rank("jedi") < 2)
      {
         out("Only those who no longer need a sponsor can sponsor a Padawan or apprentice.\n");
      }
      else if (player_name == this_body()->query_userid())
      {
         out("You sponsor yourself and take self-determination to a whole new level.\n");
      }
      else if (strlen(player_name))
      {
         int sponsored_count = JEDI_SPONSOR_D->count_sponsored(this_body()->query_userid());

         if (sponsored_count >= (this_body()->query_guild_rank("jedi") - 1))
         {
            out("You are already sponsoring " + sponsored_count + " " + (this_body()->query_jedi_alignment() > 0 ? "Padawan" : "apprentice") + (sponsored_count != 1 ? "s" : "") + " out of " + (this_body()->query_guild_rank("jedi") - 1) + " possible.\n");
         }
         else
         {
            sponsor(player_name);
         }
      }
      else
      {
         list_sponsored();
      }
   }
   else
   {
      out("Only Jedi can sponsor a Padawan or apprentice.\n");
   }
}

void sponsor(string player_name)
{
   object array players_in_room = filter_array(all_inventory(environment(this_body())), (: $1->query_userid() == $(player_name) :) );

   if (sizeof(players_in_room) == 1)
   {
      object player = players_in_room[0];
      mapping unsponsored = JEDI_SPONSOR_D->query_unsponsored(this_body()->query_userid());
      string force_message;

      if ((player->query_guild_rank("jedi") > 1) || (player->query_primary_guild() == "jedi"))
      {
         outf("%s does not require a sponsor.\n", capitalize(player_name));
         return;
      }

      if (sizeof(unsponsored))
      {
         if (unsponsored[player_name] && ((unsponsored[player_name] + 5184000) > time()))
         {
            string elapsed_time = convert_time((time() - unsponsored[player_name]) / 86400 * 86400);

            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsored[player_name]) / 3600 * 3600); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsored[player_name]) / 60 * 60); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsored[player_name])); }

            outf("You unsponsored %s on %s (%s ago).\n", capitalize(player_name), ctime(unsponsored[player_name]), elapsed_time);

            elapsed_time = convert_time((5184000 - (time() - unsponsored[player_name])) / 86400 * 86400);

            if (!strlen(elapsed_time)) { elapsed_time = convert_time((5184000 - (time() - unsponsored[player_name])) / 3600 * 3600); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((5184000 - (time() - unsponsored[player_name])) / 60 * 60); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((5184000 - (time() - unsponsored[player_name]))); }

            outf("You can sponsor %s again on %s (%s from now).\n", capitalize(player_name), ctime(unsponsored[player_name] + 5184000), elapsed_time);

            return;
         }
      }

      if (strlen(JEDI_SPONSOR_D->query_sponsor(player_name)))
      {
         if (JEDI_SPONSOR_D->query_sponsor(player_name) == this_body()->query_userid())
         {
            out("You don't recognize your own " + (this_body()->query_jedi_alignment() > 0 ? "Padawan" : "apprentice") + "?!?\n");
         }
         else
         {
            out(capitalize(player_name) + " already has a sponsor.\n");
         }

         return;
      }

      JEDI_SPONSOR_D->add_sponsored(player_name, this_body()->query_userid(), (this_body()->query_jedi_alignment() > 0 ? 1 : -1));

      this_body()->targetted_action("$N $vsponsor $n1 as $p " + (this_body()->query_jedi_alignment() > 0 ? "Padawan" : "apprentice") + ".", player);

      force_message = "%^BOLD%^%^RED%^You sense the " + (this_body()->query_jedi_alignment() < 0 ? "Dark" : "Light") + " Side of the Force grow stronger, as " + get_jedi_rank_title(this_body()->query_guild_rank("jedi"), this_body()->query_jedi_alignment()) + " " + capitalize(this_body()->query_userid()) + " sponsors " + capitalize(player->query_userid()) + " as " + this_body()->query_possessive() + " " + get_jedi_rank_title(1, this_body()->query_jedi_alignment()) + ".%^RESET%^\n";

      foreach (object jedi in filter_array(bodies(), (: $1->query_toggle("sense disturbance") == "on" :) ))
      {
         tell(jedi, force_message);
      }
   }
   else
   {
      out("The player you wish to sponsor must be in the same room.\n");
   }
}

void list_sponsored()
{
   mapping sponsored = JEDI_SPONSOR_D->query_sponsored(this_body()->query_userid());
   int possible = this_body()->query_guild_rank("jedi") - 1;

   if (sizeof(sponsored))
   {
      int level;
      string elapsed_time;

      out("You currently have " + sizeof(sponsored) + " " + (this_body()->query_jedi_alignment() > 0 ? "Padawan" : "apprentice") + (sizeof(sponsored) == 1 ? "" : "s") + " out of " + possible + " possible:\n");

      foreach (string name in keys(sponsored))
      {
         class player_guild_data guild_data = PLAYER_DATA_D->get_player_guild_data(name);

         if (guild_data)
         {
            level = 0;
            elapsed_time = convert_time((time() - sponsored[name]) / 86400 * 86400);

            if (guild_data->guild_levels["jedi"])
            {
               level = guild_data->guild_levels["jedi"];
            }

            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsored[name]) / 3600 * 3600); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsored[name]) / 60 * 60); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsored[name])); }

//            out("\n" + capitalize(name) + " since " + ctime(sponsored[name]) + "\n");
            outf("\n  %-12s since %s\n", capitalize(name), ctime(sponsored[name]));
//            out("Level " + level + " after " + elapsed_time + "\n");
            outf("  Level %-6i after %s\n", level, elapsed_time);
         }
      }
   }
   else
   {
      out("You currently have no " + (this_body()->query_jedi_alignment() > 0 ? "Padawans" : "apprentices") + ", out of " + possible + " possible.\n");
   }
}