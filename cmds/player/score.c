// Last edited by deforce on 11-25-2007
#include <mudlib.h>
#include <config.h>

inherit CMD;

#include <guild_plurals.h>

void list_guilds();
int sort_level_name(string first, string second);

object body;
string primary_guild;

private void main(string arg)
{
   if (wizardp(this_body()) && find_body(arg)) { body = find_body(arg); }
   else { body = this_body(); }

   primary_guild = body->query_primary_guild();

   out("You are: " + body->query_title() + (wizardp(body) ? (adminp(body) ? " (Admin Wizard)" : " (Wizard)") : "") + "\n");

   outf("%s %s\n", capitalize(body->query_gender_string()), capitalize(body->query_race()));

   outf("Primary guild: %s\n", (strlen(primary_guild) ? title_capitalize(primary_guild) : "none"));

   outf("You have %%^BOLD%%^%%^CYAN%%^%i%%^RESET%%^ experience.\n", body->query_experience());

   outf("You have %i / %i hit points and %i / %i social points.\n",
      body->query_health(), body->query_max_health(),
      body->query_social_points(), body->query_max_social_points());

   outf("You have %%^BOLD%%^%%^RED%%^%i %%^WHITE%%^credits on hand%%^RESET%%^ and %%^BOLD%%^%%^RED%%^%i %%^WHITE%%^credits in the bank%%^RESET%%^.\n",
      body->query_amt_money("credits"), body->query_amt_money("bank_credits"));

   list_guilds();
}

void list_guilds()
{
   mapping guilds = body->query_guilds();
   string array guild_names = body->query_guild_names();

   if (strlen(primary_guild))
   {
      outf("%-14s : %d\n", title_capitalize(primary_guild), guilds[primary_guild]);
   }

   guild_names -= ({ primary_guild });

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      int guild_level = guilds[guild_names[count]];

      guild_names[count] = sprintf("%02d", guild_level) + ":::" + guild_names[count];
   }

   guild_names = sort_array(guild_names, "sort_level_name");

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      string array name = explode(guild_names[count], ":::");

      guild_names[count] = name[1];
   }

   foreach (string guild in guild_names)
   {
      outf("%-14s : %d\n", title_capitalize(guild), guilds[guild]);
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