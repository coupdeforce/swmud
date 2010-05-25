// Last edited by deforce on 04-22-2010
inherit M_CONVERSATION;
inherit M_MESSAGES;

array get_trainable_guilds();
mapping get_trainable_abilities();
void set_trainable_guild(string guild);
varargs void set_trainable_ability(string ability, mapping prerequisites);
varargs void set_trainable_specialization(string guild, string specialization, mapping prerequisites);
void generate_training_options();
void train(string ability);
varargs void advance(string guild);
int guild_check();
int query_jedi_alignment();
void set_not_guild_member(string text);
void set_got_stuff_to_teach(string text);
void set_nothing_to_teach(string text);

array trainable_guilds = ({ });
mapping trainable_abilities = ([ ]);
mapping trainable_specializations = ([ ]);
mapping specialization_guilds = ([ ]);
string not_guild_member = "You're not a member of the guild.";
string got_stuff_to_teach = "Here's what I can teach you with your current ability";
string nothing_to_teach = "There is nothing more I can teach you.";

array get_trainable_guilds()
{
   return trainable_guilds;
}

mapping get_trainable_abilities()
{
   return trainable_abilities;
}

mapping get_trainable_specializations()
{
   return trainable_specializations;
}

mapping get_specialization_guilds()
{
   return specialization_guilds;
}

void generate_training_options()
{
   object player = get_current_player();
   string array guild_options = ({ });
   string array ability_options = ({ });

   if (!player) { return; }

   if ((player->query_primary_guild() == "") && (player->query_primary_level() == 0))
   {
      foreach (string guild in trainable_guilds)
      {
         int guild_level = player->query_guild_level(guild);

         if (guild == "jedi")
         {
            if (sizeof(player->query_guild_levels()) == 0)
            {
               string join_text = "I would like to join the Jedi Order.";

               if (query_jedi_alignment() < 1)
               {
                  join_text = "I pledge myself to the Dark Side of the Force.";
               }

               if (player->query_experience() >= player->get_experience_to_advance_guild(guild))
               {
                  guild_options += ({ guild });
                  add_options(([ "$join$" + guild : join_text ]));
                  add_responses(([ "$join$" + guild : "$join$" + guild + "@@@@$clear$" ]));
               }
            }
         }
         else if (((sizeof(player->query_guild_levels()) == 0) && player->can_advance_guild_level(guild))
            || ((guild_level > 0) && (guild_level >= sort_array(player->query_guild_levels(), -1)[0])))
         {
            guild_options += ({ guild });
            add_options(([ "$join$" + guild : "I would like to join the " + title_capitalize(guild) + " guild." ]));
            add_responses(([ "$join$" + guild : "$join$" + guild + "@@@@$clear$" ]));
         }
      }

      if (sizeof(guild_options))
      {
         add_start(explode("$join$" + implode(guild_options, ",$join$"), ","));
      }
   }

   guild_options = ({ });

   foreach (string guild in trainable_guilds)
   {
      if (player->can_advance_guild_level(guild))
      {
         guild_options += ({ guild });
         add_options(([ "$advance$" + guild : "I would like to advance in the " + title_capitalize(guild) + " guild." ]));
         add_responses(([ "$advance$" + guild : "$advance$" + guild + "@@$advance$" + guild + "@@$clear$" ]));
      }
   }

   if (sizeof(guild_options))
   {
      add_start(explode("$advance$" + implode(guild_options, ",$advance$"), ","));
   }

   if (!guild_check()) { return; }

   foreach (string ability in sort_array(keys(trainable_abilities), 1))
   {
      if (member_array(ability, player->query_learned_skills()) == -1)
      {
         if (sizeof(trainable_abilities[ability]))
         {
            int check = 1;

            foreach (string prereq in keys(trainable_abilities[ability]))
            {
               if (player->query_skill(prereq) < trainable_abilities[ability][prereq])
               {
                  check = 0;
               }
            }

            if (check)
            {
               ability_options += ({ ability });
               add_options(([ "$train$" + ability : "I would like to learn " + title_capitalize(ability) + "." ]));
               add_responses(([ "$train$" + ability : "$train$" + ability + "@@@@" + "$train$" + ability ]));
            }
         }
         else
         {
            ability_options += ({ ability });
            add_options(([ "$train$" + ability : "I would like to learn " + title_capitalize(ability) + "." ]));
            add_responses(([ "$train$" + ability : "$train$" + ability + "@@@@" + "$train$" + ability ]));
         }
      }
   }

   add_options(([ "$ask_learn$" : "Is there anything you can teach me?" ]));

   if (sizeof(ability_options))
   {
      add_responses(([ "$ask_learn$" : got_stuff_to_teach + ": " + title_capitalize(implode(ability_options, ", ")) + "@@$train$" + implode(ability_options, ",$train$") + "@@$clear$" ]));
   }
   else
   {
      add_responses(([ "$ask_learn$" : nothing_to_teach + "@@@@$ask_learn$" ]));
   }

   add_start(({ "$ask_learn$" }));

   foreach (string specialization in sort_array(keys(trainable_specializations), 1))
   {
      string guild = specialization_guilds[specialization];
      string player_specialization = player->query_guild_specialization(guild);
      int rank = player->query_guild_specialization_rank(guild, specialization);

      if (!strlen(player_specialization)
         || ((player_specialization == specialization) && (rank < 10)))
      {
         if (player_specialization == specialization)
         {
            if (rank < 9)
            {
               add_options(([ "$specialize$" + specialization : "I would like to further my " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild." ]));
            }
            else
            {
               add_options(([ "$specialize$" + specialization : "I would like to complete my " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild." ]));
            }
         }
         else
         {
            add_options(([ "$specialize$" + specialization : "I would like to begin the " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild." ]));
         }

         add_responses(([ "$specialize$" + specialization : "$specialize$" + specialization + "@@@@" + "$clear$" ]));
         add_start(({ "$specialize$" + specialization }));
      }
   }
}

void specialize(string specialization)
{
   object player = get_current_player();
   string guild = specialization_guilds[specialization];

   if (player->query_guild_specialization(guild) == specialization)
   {
      int rank = player->query_guild_specialization_rank(guild, specialization);

      if (rank < 10)
      {
         player->add_guild_specialization(guild, specialization);

         do_game_command("say You have achieved Rank " + (rank + 1) + " of the " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild" + (rank >= 9 ? ", and your specialization is now complete." : "") + ".");
      }
      else
      {
         do_game_command("say You have already achieved Rank 10 of the " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild.  Please tell a wizard that you got this message.");
      }
   }
   else if (strlen(player->query_guild_specialization(guild)))
   {
      do_game_command("say You are already specialized in " + title_capitalize(player->query_guild_specialization(guild)) + " for the " + title_capitalize(guild) + " guild.");
   }
   else
   {
      player->set_guild_specialization(guild, specialization, 1);

      do_game_command("say You have achieved Rank 1 of the " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild.");
   }
}

void train(string ability)
{
   get_current_player()->add_learned_skill(ability);

   targetted_action("$N teaches $t " + title_capitalize(ability) + ".", get_current_player());
}

void advance(string guild)
{
   object player = get_current_player();
   int required_experience = player->get_experience_to_advance_guild(guild);
   string primary_guild = player->query_primary_guild();

   if ((player->query_guild_level(guild) >= 20) && (player->query_guild_rank(guild) < 2))
   {
      tell(player, "You require additional training to advance to level " + (player->query_guild_level(guild) + 1) + " in " + title_capitalize(guild) + ".\n");

      return;
   }

   if (required_experience == 0)
   {
      if (guild == primary_guild)
      {
         if (guild == "jedi")
         {
            required_experience = EXP_D->get_required_exp("ronin jedi", 1);
         }
         else
         {
            required_experience = EXP_D->get_required_exp("primary", 1);
         }
      }
      else if (sizeof(player->query_guild_names()) < 3)
      {
         if (guild == "jedi")
         {
            required_experience = EXP_D->get_required_exp("jedi", 1);
         }
         else
         {
            required_experience = EXP_D->get_required_exp("secondary", 1);
         }
      }
      else
      {
         if (guild == "jedi")
         {
            required_experience = EXP_D->get_required_exp("jedi quaternary", 1);
         }
         else
         {
            required_experience = EXP_D->get_required_exp("quaternary", 1);
         }
      }
   }

   player->advance_guild_level(guild);
   player->subtract_experience(required_experience);

   targetted_action("$N $vtrain $t more in the " + title_capitalize(guild) + " guild.", get_current_player());

   tell(player, "You advance to level " + player->query_guild_level(guild) + " in " + title_capitalize(guild) + " at the cost of " + required_experience + " experience.\n");

   player->refresh_stats();
}

void join(string guild)
{
   object player = get_current_player();
   player->set_primary_guild(guild);

   if (sizeof(player->query_guild_levels()) == 0)
   {
      advance(guild);
   }

   if (guild == "jedi")
   {
      player->set_jedi_alignment(25 * (this_object()->query_jedi_alignment() > 0 ? 1 : -1));
   }

   targetted_action("$N welcomes $t to the " + title_capitalize(guild) + " guild.", get_current_player());
}

void set_trainable_guild(string guild)
{
   if (strlen(guild) && (member_array(guild, trainable_guilds) == -1))
   {
      trainable_guilds += ({ guild });
   }
}

varargs void set_trainable_ability(string ability, mapping prerequisites)
{
   if (strlen(ability) && (member_array(ability, keys(trainable_abilities)) == -1))
   {
      trainable_abilities[ability] = ([ ]);

      if (sizeof(prerequisites))
      {
//         foreach (string key in keys(prerequisites))
//         {
//            trainable_abilities[ability] += ([ key : (prerequisites[key] < 100 ? prerequisites[key] * 100 : prerequisites[key]) ]);
//         }

           trainable_abilities[ability] = prerequisites;
      }
   }
}

varargs void set_trainable_specialization(string guild, string specialization, mapping prerequisites)
{
   if (strlen(specialization) && (member_array(specialization, keys(trainable_specializations)) == -1))
   {
      trainable_specializations[specialization] = ([ ]);
      specialization_guilds[specialization] = guild;

      if (sizeof(prerequisites))
      {
         trainable_specializations[specialization] = prerequisites;
      }
   }
}

int guild_check()
{
   string array guild_names = get_current_player()->query_guild_names();

   foreach (string guild in trainable_guilds)
   {
      int member_guild = member_array(guild, guild_names);

      if (member_guild == -1)
      {
         return 0;
      }
      else if (get_current_player()->query_guild_level(guild_names[member_guild]) < 1)
      {
         return 0;
      }
   }

   return 1;
}

void set_not_guild_member(string text)
{
   not_guild_member = text;
}

void set_got_stuff_to_teach(string text)
{
   got_stuff_to_teach = text;
}

void set_nothing_to_teach(string text)
{
   nothing_to_teach = text;
}