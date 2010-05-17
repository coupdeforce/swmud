// Last edited by deforce on 11-25-2007
#include <mudlib.h>
#include <config.h>

inherit CMD;

int sort_level_name(string first, string second);

private void main()
{
   object body = this_body();
   string primary_guild = body->query_primary_guild();
   string array guild_names = body->query_guild_names();
   int primary_guild_level = 0;
   int experience = body->query_experience();
   int level_total = 0;
   int secondary_level_total = 0;

   if (!sizeof(guild_names))
   {
      out("You are not a member of any guilds.\n");

      return;
   }

   foreach (string name in guild_names)
   {
      level_total += body->query_guild_level(name);
   }

   guild_names -= ({ primary_guild });

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      int guild_level = body->query_guild_level(guild_names[count]);

      guild_names[count] = sprintf("%02d", guild_level) + ":::" + guild_names[count];
   }

   guild_names = sort_array(guild_names, "sort_level_name");

   for (int count = 0; count < sizeof(guild_names); count++)
   {
      string array name = explode(guild_names[count], ":::");

      guild_names[count] = name[1];
   }

   if (strlen(primary_guild))
   {
      primary_guild_level = body->query_guild_level(primary_guild);

      if (primary_guild_level < 50)
      {
         int next_level = primary_guild_level + 1;
         int required_experience;
         string experience_type = "primary";

         if (primary_guild == "jedi")
         {
            if (primary_guild_level < 20) { experience_type = "ronin jedi"; }
            else { experience_type = "secondary"; }
         }

         required_experience = EXP_D->get_required_exp(experience_type, next_level);

         if (experience < required_experience)
         {
            out("You need " + (required_experience - experience) + " more experience to advance " + title_capitalize(primary_guild) + " to level " + next_level + ".\n");
         }
         else
         {
            out("%^BOLD%^You can advance " + title_capitalize(primary_guild) + " to level " + next_level + ".%^RESET%^\n");
         }
      }
      else
      {
         out("You cannot advance " + title_capitalize(primary_guild) + " any further.\n");
      }
   }

   secondary_level_total = level_total - primary_guild_level;

   foreach (string name in guild_names)
   {
      int guild_level = body->query_guild_level(name);

      if ((guild_level == primary_guild_level) && (guild_level < 30))
      {
         out("You need to advance " + title_capitalize(primary_guild) + " before advancing " + title_capitalize(name) + " past level " + guild_level + ".\n");
      }
      else if ((guild_level < 10) && ((secondary_level_total - guild_level) >= 50))
      {
         int next_level = guild_level + 1;
         string experience_type = "quaternary";
         int required_experience;

         if (name == "jedi") { experience_type = "jedi quaternary"; }

         required_experience = EXP_D->get_required_exp(experience_type, next_level);

         if (experience < required_experience)
         {
            out("You need " + (required_experience - experience) + " more experience to advance " + title_capitalize(name) + " to level " + next_level + ".\n");
         }
         else
         {
            out("%^BOLD%^You can advance " + title_capitalize(name) + " to level " + next_level + ".%^RESET%^\n");
         }
      }
      else if ((guild_level == 10) && (secondary_level_total >= 60))
      {
         out("You cannot advance " + title_capitalize(name) + " any further.\n");
      }
      else if ((guild_level == 20) && (secondary_level_total > 40))
      {
         out("You cannot advance " + title_capitalize(name) + " any further.\n");
      }
      else if (guild_level < 30)
      {
         int next_level = guild_level + 1;
         string experience_type = "secondary";
         int required_experience;

         if (name == "jedi") { experience_type = "jedi"; }

         required_experience = EXP_D->get_required_exp(experience_type, next_level);

         if (experience < required_experience)
         {
            out("You need " + (required_experience - experience) + " more experience to advance " + title_capitalize(name) + " to level " + next_level + ".\n");
         }
         else
         {
            out("%^BOLD%^You can advance " + title_capitalize(name) + " to level " + next_level + ".%^RESET%^\n");
         }
      }
      else if (guild_level == 30)
      {
         out("You cannot advance " + title_capitalize(name) + " any further.\n");
      }
      else
      {
         out(name + "\n");
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