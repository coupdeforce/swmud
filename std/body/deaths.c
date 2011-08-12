#include <daemons.h>

int query_max_health();
int array query_guild_levels();
int query_experience();
void set_experience(int value);
void set_primary_level(int value);
int query_primary_level();
int get_experience_to_advance_guild(string guild_name);
string query_primary_guild();
string array query_guild_names();
int query_guild_level(string guild_name);
void set_guild_level(string guild_name, int guild_level);
void remove_guild(string guild_name);
string query_name();
string query_race();
void receive_private_msg(string message);
void refresh_stats();

int deaths;
mapping killed_by = ([ ]);
mapping killed_where = ([ ]);

void save_me();

protected void add_a_death()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add a death to " + this_object()->short(), previous_object(-1)); }

   deaths++;
   save_me();
}

protected void death_penalty()
{
   int experience = query_experience();
   int new_exp = experience;
   int level = query_primary_level();
   object killer = previous_object();
   string killed_message = "";

   if (killer && killer->is_body() && (killer != this_object()) && (environment(killer) != this_object()))
   {
      int killer_level = killer->query_primary_level();
      string killer_message = "You have killed " + query_name() + ", who is ";
      killed_message += "You were killed by " + killer->query_name() + ", who is ";

      if (killer_level > level)
      {
         int percent_lost = 50 - ((killer_level - level) * 5);
         int required_experience;

         killed_message += (killer_level - level) + " levels higher than you.\n";
         killer_message += (killer_level - level) + " levels lower than you.\n";

         if (query_race() != "ithorian")
         {
            if (percent_lost < 1) { percent_lost = 1; }

            new_exp = floor(experience * ((100.0 - percent_lost) / 100.0));

            if (query_primary_guild() == "jedi")
            {
               if (level < 20)
               {
                  required_experience = EXP_D->get_required_exp("ronin jedi", level + 1);
               }
               else
               {
                  required_experience = EXP_D->get_required_exp("secondary", level + 1);
               }
            }
            else
            {
               required_experience = EXP_D->get_required_exp("primary", level + 1);
            }

            if ((experience >= floor(required_experience * 0.5))
               && (new_exp < floor(required_experience * 0.5)))
            {
               new_exp = floor(required_experience * 0.5);
            }
            else if (experience < floor(required_experience * 0.5))
            {
               new_exp = experience;
            }

            killer->add_experience(experience - new_exp);
         }
         else if (this_object()->is_unjustified_ithorian_target(killer))
         {
            new_exp = 0;
            killer->add_experience(experience - new_exp);
         }
      }
      else if (killer_level < level)
      {
         int percent_lost = 50 + ((level - killer_level) * 5);

         killed_message += (level - killer_level) + " levels lower than you.\n";
         killer_message += (level - killer_level) + " levels higher than you.\n";

         if (query_race() != "ithorian")
         {
            if (percent_lost > 50) { percent_lost = 50; }

            new_exp = floor(experience * ((100.0 - percent_lost) / 100.0));

            if (level < 50)
            {
               int required_experience;

               if (query_primary_guild() == "jedi")
               {
                  if (level < 20)
                  {
                     required_experience = EXP_D->get_required_exp("ronin jedi", level + 1);
                  }
                  else
                  {
                     required_experience = EXP_D->get_required_exp("secondary", level + 1);
                  }
               }
               else
               {
                  required_experience = EXP_D->get_required_exp("primary", level + 1);
               }

               if ((experience >= floor(required_experience * 0.5))
                  && (new_exp < floor(required_experience * 0.5)))
               {
                  new_exp = floor(required_experience * 0.5);
               }
               else if (experience < floor(required_experience * 0.5))
               {
                  new_exp = experience;
               }
            }

            killer->add_experience(experience - new_exp);
         }
         else if (this_object()->is_unjustified_ithorian_target(killer))
         {
            new_exp = 0;
            killer->add_experience(experience - new_exp);
         }
      }
      else
      {
         killed_message += "equal in level.\n";
         killer_message += "equal in level.\n";

         if (query_race() != "ithorian")
         {
            new_exp = experience / 2;

            if (level < 50)
            {
               int required_experience;

               if (query_primary_guild() == "jedi")
               {
                  if (level < 20)
                  {
                     required_experience = EXP_D->get_required_exp("ronin jedi", level + 1);
                  }
                  else
                  {
                     required_experience = EXP_D->get_required_exp("secondary", level + 1);
                  }
               }
               else
               {
                  required_experience = EXP_D->get_required_exp("primary", level + 1);
               }

               if ((experience >= floor(required_experience * 0.5))
                  && (new_exp < floor(required_experience * 0.5)))
               {
                  new_exp = floor(required_experience * 0.5);
               }
               else if (experience < floor(required_experience * 0.5))
               {
                  new_exp = experience;
               }
            }

            killer->add_experience(experience - new_exp);
         }
         else if (this_object()->is_unjustified_ithorian_target(killer))
         {
            new_exp = 0;
            killer->add_experience(experience - new_exp);
         }
      }

      killer_message += "You have gained " + (experience - new_exp) + " experience points.\n\n";
      tell(killer, killer_message);
   }
   else if (killer && (killer != this_object()) && (environment(killer) != this_object()))
   {
      killed_message += "You were killed by " + killer->in_room_desc() + ".\n";

      if (level > 1)
      {
         int needed_exp;
         int exp_penalty = 50;
         int kept_exp;
         int max_health = query_max_health();
         float level_count = 0.0;
         new_exp = experience;

         set_primary_level(level - 1);

         killed_message += "Your primary guild, " + title_capitalize(query_primary_guild()) + ", has been decreased to level " + (level - 1) + ".\n";

         foreach (int value in query_guild_levels())
         {
            level_count += value;
         }

         foreach (string guild in query_guild_names())
         {
            int secondary_level = query_guild_level(guild);

            if (secondary_level >= level)
            {
               set_guild_level(guild, level - 1);

               killed_message += "Your " + title_capitalize(guild) + " level has been decreased to " + (level - 1) + ".\n";
            }
         }

         if ((sizeof(query_guild_names()) == 4) && (level <= 30))
         {
            foreach (string guild in query_guild_names())
            {
               int fourth_guild_level = query_guild_level(guild);

               if (fourth_guild_level <= 10)
               {
                  remove_guild(guild);

                  killed_message += "Your membership in the " + capitalize(guild) + " guild has been lost.\n";
               }
            }
         }

         refresh_stats();

         killed_message += "You lose " + (max_health - query_max_health()) + " hit points.\n";

         needed_exp = get_experience_to_advance_guild(query_primary_guild());

         if (level >= 20)
         {
            exp_penalty += level;
         }

         kept_exp = floor(needed_exp * ((100.0 - exp_penalty) / 100.0));

         if (new_exp > kept_exp)
         {
            new_exp = kept_exp;
         }
      }
      else
      {
         new_exp = experience;

         if (new_exp > 500)
         {
            new_exp = 500;
         }
      }
   }

   set_experience(new_exp);

   killed_message += "You had " + experience + " experience points and are left with " + new_exp + ", losing " + (experience - new_exp) + ".\n\n";

   receive_private_msg(killed_message);
}

int query_deaths()
{
   return deaths;
}

void set_deaths(int new_deaths)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s deaths to " + new_deaths, previous_object(-1)); }

   if (new_deaths >= 1)
   {
      deaths = new_deaths;
      save_me();
   }
}

void reset_deaths()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s deaths", previous_object(-1)); }

   deaths = 0;
   save_me();
}

mapping query_killed_by()
{
   return killed_by;
}

mapping query_killed_where()
{
   return killed_where;
}

void set_killed_by(int time, string name, string where)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + " killed by " + name + " at " + where + " on " + ctime(time), previous_object(-1)); }

   killed_by[time] = name;
   killed_where[time] = where;
}

void reset_killed_by()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("reset " + this_object()->short() + "'s killed by info", previous_object(-1)); }

   killed_by = ([ ]);
   killed_where = ([ ]);
}