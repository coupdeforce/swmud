inherit M_CONVERSATION;
inherit M_MESSAGES;

array get_trainable_guilds();
mapping get_trainable_abilities();
void set_trainable_guild(string guild);
varargs void set_trainable_ability(string ability, mapping prerequisites);
varargs void set_trainable_construction(string construction, mapping prerequisites);
varargs void set_trainable_manufacture(string manufacture, mapping prerequisites);
varargs void set_trainable_specialization(string guild, string specialization, mapping prerequisites);
void generate_training_options();
void train(string ability);
void teach_construction(string construction);
void teach_manufacture(string manufacture);
varargs void advance(string guild);
int query_jedi_alignment();
void set_not_guild_member(string text);
void set_got_stuff_to_teach(string text);
void set_nothing_to_teach(string text);
void add_guild_options(object player);
void add_specialization_options(object player);
string array add_trainable_abilities(object player);
string array add_trainable_constructions(object player);
string array add_trainable_manufactures(object player);

array trainable_guilds = ({ });
mapping trainable_abilities = ([ ]);
mapping trainable_constructions = ([ ]);
mapping trainable_manufactures = ([ ]);
mapping trainable_specializations = ([ ]);
mapping specialization_guilds = ([ ]);
string not_guild_member = "You're not a member of the guild.";
string got_stuff_to_teach = "Here's what I can teach you with your current ability";
string nothing_to_teach = "There is nothing more I can teach you.";
string need_more_skill = "Let me know when you're better at %s.";

void generate_training_options()
{
   object player = get_current_player();
   string array ability_options;
   string array construction_options;
   string array manufacture_options;

   if (!player) { return; }

   add_guild_options(player);
   add_specialization_options(player);
   ability_options = add_trainable_abilities(player);
   construction_options = add_trainable_constructions(player);
   manufacture_options = add_trainable_manufactures(player);

   add_options(([ "$ask_learn$" : "Is there anything you can teach me?" ]));

   if (sizeof(ability_options) || sizeof(construction_options) || sizeof(manufacture_options))
   {
      string teach_list = "";
      string response_list = "";

      if (sizeof(ability_options))
      {
         teach_list += title_capitalize(implode(ability_options, ", "));
         response_list += "$train$" + implode(ability_options, ",$train$");
      }

      if (sizeof(construction_options))
      {
         teach_list += (strlen(teach_list) ? ", " : "") + title_capitalize(implode(construction_options, " construction, ") + " construction");
         response_list += (strlen(response_list) ? "," : "") + "$learn construction$" + implode(construction_options, ",$learn construction$");
      }

      if (sizeof(manufacture_options))
      {
         teach_list += (strlen(teach_list) ? ", " : "") + title_capitalize(implode(manufacture_options, " manufacturing, ") + " manufacturing");
         response_list += (strlen(response_list) ? "," : "") + "$learn manufacture$" + implode(manufacture_options, ",$learn manufacture$");
      }

      add_responses(([ "$ask_learn$" : got_stuff_to_teach + ": " + teach_list + "@@" + response_list + "@@$clear$" ]));
   }
   else
   {
      add_responses(([ "$ask_learn$" : nothing_to_teach + "@@@@$ask_learn$" ]));
   }

   add_start(({ "$ask_learn$" }));
}

void add_guild_options(object player)
{
   string array guild_options = ({ });

   // Check if player can still join a primary guild
   if ((player->query_primary_guild() == "") && (player->query_primary_level() == 0))
   {
      foreach (string guild in trainable_guilds)
      {
         int required_experience = can_advance_guild_level(guild, player->query_primary_guild(), player->query_guilds());

         if ((player->query_guild_level(guild) > 0) || (required_experience > 0))
         {
            string join_text = "I would like to join the " + title_capitalize(guild) + " guild.";

            if (guild == "jedi")
            {
               join_text = "I would like to join the Jedi Order.";

               if (query_jedi_alignment() < 1)
               {
                  join_text = "I pledge myself to the Dark Side of the Force.";
               }
            }

            if (player->query_experience() >= required_experience)
            {
               guild_options += ({ guild });
               add_options(([ "$join$" + guild : join_text ]));
               add_responses(([ "$join$" + guild : "$join$" + guild + "@@@@$clear$" ]));
            }
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
      int required_exp = can_advance_guild_level(guild, player->query_primary_guild(), player->query_guilds());

      if ((required_exp > 0) && (player->query_experience() >= required_exp))
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
}

void add_specialization_options(object player)
{
   string array begin_specialization_options = ({ });

   foreach (string specialization in sort_array(keys(trainable_specializations), 1))
   {
      string guild = specialization_guilds[specialization];
      string player_specialization = player->query_guild_specialization(guild);
      int rank = player->query_guild_specialization_rank(guild, specialization);

      if (!strlen(player_specialization) || ((player_specialization == specialization) && (rank < 10)))
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

            add_start(({ "$specialize$" + specialization }));
         }
         else
         {
            begin_specialization_options += ({ specialization });

            add_options(([ "$specialize$" + specialization : "I would like to begin the " + title_capitalize(specialization) + " specialization in the " + title_capitalize(guild) + " guild." ]));
         }

         add_responses(([ "$specialize$" + specialization : "$specialize$" + specialization + "@@@@" + "$clear$" ]));
      }
   }

   if (sizeof(begin_specialization_options))
   {
      add_options(([ "can specialize" : "Can you help me to begin a specialization?" ]));

      add_responses(([ "can specialize" : "I am able to teach the following specializations: " + title_capitalize(implode(begin_specialization_options, ", ")) + "@@$specialize$" + implode(begin_specialization_options, ",$specialize$") + "@@$clear$" ]));

      add_start(({ "can specialize" }));
   }
}

string array add_trainable_abilities(object player)
{
   string array ability_options = ({ });

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

   return ability_options;
}

string array add_trainable_constructions(object player)
{
   string array construction_options = ({ });

   foreach (string construction in sort_array(keys(trainable_constructions), 1))
   {
      if (member_array(construction, player->query_learned_constructions()) == -1)
      {
         if (sizeof(trainable_constructions[construction]))
         {
            int check = 1;

            foreach (string prereq in keys(trainable_constructions[construction]))
            {
               if (player->query_skill(prereq) < trainable_constructions[construction][prereq])
               {
                  check = 0;
               }
            }

            if (check)
            {
               construction_options += ({ construction });
               add_options(([ "$learn construction$" + construction : "I would like to learn how to construct " + add_article(construction) + "." ]));
               add_responses(([ "$learn construction$" + construction : "$learn construction$" + construction + "@@@@" + "$learn construction$" + construction ]));
            }
         }
         else
         {
            construction_options += ({ construction });
            add_options(([ "$learn construction$" + construction : "I would like to learn how to construct " + add_article(construction) + "." ]));
            add_responses(([ "$learn construction$" + construction : "$learn construction$" + construction + "@@@@" + "$learn construction$" + construction ]));
         }
      }
   }

   return construction_options;
}

string array add_trainable_manufactures(object player)
{
   string array manufacture_options = ({ });

   foreach (string manufacture in sort_array(keys(trainable_manufactures), 1))
   {
      if (member_array(manufacture, player->query_learned_manufactures()) == -1)
      {
         if (sizeof(trainable_manufactures[manufacture]))
         {
            int check = 1;

            foreach (string prereq in keys(trainable_manufactures[manufacture]))
            {
               if (player->query_skill(prereq) < trainable_manufactures[manufacture][prereq])
               {
                  check = 0;
               }
            }

            if (check)
            {
               manufacture_options += ({ manufacture });
               add_options(([ "$learn manufacture$" + manufacture : "I would like to learn how to manufacture " + add_article(manufacture) + "." ]));
               add_responses(([ "$learn manufacture$" + manufacture : "$learn manufacture$" + manufacture + "@@@@" + "$learn manufacture$" + manufacture ]));
            }
         }
         else
         {
            manufacture_options += ({ manufacture });
            add_options(([ "$learn manufacture$" + manufacture : "I would like to learn how to manufacture " + add_article(manufacture) + "." ]));
            add_responses(([ "$learn manufacture$" + manufacture : "$learn manufacture$" + manufacture + "@@@@" + "$learn manufacture$" + manufacture ]));
         }
      }
   }

   return manufacture_options;
}

void specialize(string specialization)
{
   object player = get_current_player();
   string guild = specialization_guilds[specialization];
   int rank = player->query_guild_specialization_rank(guild, specialization);

   foreach (string skill in keys(trainable_specializations[specialization]))
   {
      if (player->query_skill(skill) < (trainable_specializations[specialization][skill] * (rank + 1)))
      {
         do_game_command("say " + sprintf(need_more_skill, SKILL_D->query_skill(skill)[0]));

         return;
      }
   }

   if (player->query_guild_specialization(guild) == specialization)
   {
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

void teach_construction(string construction)
{
   get_current_player()->add_learned_construction(construction);

   targetted_action("$N teaches $t how to construct " + add_article(construction) + ".", get_current_player());
}

void teach_manufacture(string manufacture)
{
   get_current_player()->add_learned_manufacture(manufacture);

   targetted_action("$N teaches $t how to manufacture " + add_article(manufacture) + ".", get_current_player());
}

void advance(string guild)
{
   object player = get_current_player();
   string primary_guild = player->query_primary_guild();
   int required_experience = can_advance_guild_level(guild, player->query_primary_guild(), player->query_guilds());

   if ((player->query_guild_level(guild) >= 20) && (player->query_guild_rank(guild) < 2))
   {
      tell(player, "You require additional training to advance to level " + (player->query_guild_level(guild) + 1) + " in " + title_capitalize(guild) + ".\n");

      return;
   }

   player->advance_guild_level(guild);
   player->subtract_experience(required_experience);

   targetted_action("$N " + ((player->query_guild_level(guild) >= 2) ? "$vtrain $t more" : "$vbegin $p1 training") + " in the " + title_capitalize(guild) + " guild.", get_current_player());

   tell(player, "You advance to level " + player->query_guild_level(guild) + " in " + title_capitalize(guild) + " at the cost of " + required_experience + " experience.\n");

   player->refresh_stats();
}

void join(string guild)
{
   object player = get_current_player();
   player->set_primary_guild(guild);

   if (player->query_guild_level(guild) < 1)
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

varargs void set_trainable_construction(string construction, mapping prerequisites)
{
   if (strlen(construction) && (member_array(construction, keys(trainable_constructions)) == -1))
   {
      trainable_constructions[construction] = ([ ]);

      if (sizeof(prerequisites))
      {
           trainable_constructions[construction] = prerequisites;
      }
   }
}

varargs void set_trainable_manufacture(string manufacture, mapping prerequisites)
{
   if (strlen(manufacture) && (member_array(manufacture, keys(trainable_manufactures)) == -1))
   {
      trainable_manufactures[manufacture] = ([ ]);

      if (sizeof(prerequisites))
      {
           trainable_manufactures[manufacture] = prerequisites;
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

string array get_trainable_guilds()
{
   return trainable_guilds;
}

mapping get_trainable_abilities()
{
   return trainable_abilities;
}

mapping get_trainable_constructions()
{
   return trainable_constructions;
}

mapping get_trainable_manufactures()
{
   return trainable_manufactures;
}

mapping get_trainable_specializations()
{
   return trainable_specializations;
}

mapping get_trainable_specialization_guilds()
{
   return specialization_guilds;
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

void set_need_more_skill(string text)
{
   if (strsrch(text, "%s") > -1)
   {
      need_more_skill = text;
   }
}