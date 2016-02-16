#include <classes.h>

inherit CLASS_SKILL_DATA;

mapping check_tools(mixed tools)
{
   mapping missing = ([ ]);

   foreach (mixed tool in tools)
   {
      if ((tool[2] == 0) && !present(tool[0], this_body()) && !present(tool[0], environment(this_body())))
      {
         missing[tool[1]] = 0;
      }
      else if ((tool[2] == 1) && !present(tool[0], this_body()))
      {
         missing[tool[1]] = 1;
      }
      else if ((tool[2] == 2) && !present(tool[0], environment(this_body())))
      {
         missing[tool[1]] = 2;
      }
   }

   return missing;
}

int meets_guild_requirements(object player, mixed array requirements)
{
   foreach (mixed requirement in requirements)
   {
      if (player->query_guild_level(requirement[0]) < requirement[1])
      {
         return 0;
      }
      else if (player->query_guild_rank(requirement[0]) < requirement[2])
      {
         return 0;
      }
   }

   return 1;
}

int meets_skill_requirements(object player, mixed array requirements)
{
   foreach (mixed requirement in requirements)
   {
      if (player->query_skill(requirement[0]) < requirement[1])
      {
         return 0;
      }
   }

   return 1;
}

void generate_list(object player)
{
   string array all_products = MANUFACTURE_D->query_product_internal_names();
   mapping product_list = ([ ]);
   mixed array guild_requirements;
   mixed array skill_requirements;
   string array learn_requirements;
   string array race_requirements;
   mixed requirement_set;
   int has_required;
   int set_count;

   foreach (string name in all_products)
   {
      guild_requirements = MANUFACTURE_D->query_guild_requirements(name);
      skill_requirements = MANUFACTURE_D->query_skill_requirements(name);
      learn_requirements = MANUFACTURE_D->query_learn_requirements(name);
      has_required = 1;

      if (has_required && arrayp(guild_requirements) && sizeof(guild_requirements) && arrayp(guild_requirements[0]) && sizeof(guild_requirements[0][0]))
      {
         has_required = 0;

         for (set_count = 0; !has_required && (set_count < sizeof(guild_requirements)); set_count++)
         {
            has_required = meets_guild_requirements(player, guild_requirements[set_count]);
         }
      }

      if (has_required && arrayp(skill_requirements) && sizeof(skill_requirements) && arrayp(skill_requirements[0]) && sizeof(skill_requirements[0][0]))
      {
         has_required = 0;

         for (set_count = 0; !has_required && (set_count < sizeof(skill_requirements)); set_count++)
         {
            has_required = meets_skill_requirements(player, skill_requirements[set_count]);
         }
      }

      foreach (string requirement in learn_requirements)
      {
         if (!player->has_learned_manufacture(requirement))
         {
            has_required = 0;
            break;
         }
      }

      if (sizeof(MANUFACTURE_D->query_race_requirements(name)) && (member_array(player->query_race(), MANUFACTURE_D->query_race_requirements(name)) == -1))
      {
         has_required = 0;
      }

      if (!has_required) { continue; }

      product_list[name] = MANUFACTURE_D->query_product_name(name);
   }

   if (sizeof(product_list))
   {
      write("You know how to manufacture the following products:\n");

      foreach (string name in sort_array(keys(product_list), 1))
      {
         write("  " + product_list[name] + "\n");
      }
   }
   else
   {
      write("You don't seem to know how to manufacture anything yet.\n");
   }
}

int manufacture_object(object player, string product_name, string verb_used)
{
   string internal_name = MANUFACTURE_D->query_internal_name(product_name);
   string file_name;
   mixed array guild_requirements;
   mixed array skill_requirements;
   string array learn_requirements;
   mixed array item_list;
   mixed array tool_list;
   mapping items_missing;
   mapping tools_missing;
   object array inventory;
   object product;

   if (product_name == "list")
   {
      generate_list(player);

      return 0;
   }

   if (!strlen(internal_name))
   {
      write("\"" + product_name + "\" is not a valid product.\n");
      return 0;
   }

   file_name = MANUFACTURE_D->query_file_name(internal_name);
   guild_requirements = MANUFACTURE_D->query_guild_requirements(internal_name);
   skill_requirements = MANUFACTURE_D->query_skill_requirements(internal_name);
   learn_requirements = MANUFACTURE_D->query_learn_requirements(internal_name);
   item_list = MANUFACTURE_D->query_item_list(internal_name);
   tool_list = MANUFACTURE_D->query_tool_list(internal_name);
   inventory = all_inventory(player);

   foreach (string requirement in learn_requirements)
   {
      if (!player->has_learned_manufacture(requirement))
      {
         write("You have not learned how to manufacture " + add_article(product_name) + ".\n");
         return 0;
      }
   }

   // Required guild name: requirement[0]
   // Required level in guild name: requirement[1]
   // Required rank in guild name: requirement[2]
   if (arrayp(guild_requirements) && sizeof(guild_requirements) && arrayp(guild_requirements[0]) && sizeof(guild_requirements[0][0]))
   {
      int has_required = 0;
      int set_count;

      for (set_count = 0; !has_required && (set_count < sizeof(guild_requirements)); set_count++)
      {
         has_required = meets_guild_requirements(player, guild_requirements[set_count]);
      }

      if (!has_required)
      {
         write("\nYou need more experience in the following guilds to manufacture " + add_article(product_name) + ":\n");

         for (set_count = 0; !has_required && (set_count < sizeof(guild_requirements)); set_count++)
         {
            if (set_count > 0) { write("    or\n"); }

            foreach (mixed requirement in guild_requirements[set_count])
            {
               write("  " + title_capitalize(requirement[0]) + ": level " + requirement[1] + ", rank " + requirement[2] + "\n");
            }
         }

         return 0;
      }
   }

   // Required skill name: requirement[0]
   // Amount required in skills name: requirement[1]
   if (arrayp(skill_requirements) && sizeof(skill_requirements) && arrayp(skill_requirements[0]) && sizeof(skill_requirements[0][0]))
   {
      int has_required = 0;
      int set_count;

      for (set_count = 0; !has_required && (set_count < sizeof(skill_requirements)); set_count++)
      {
         has_required = meets_skill_requirements(player, skill_requirements[set_count]);
      }

      if (!has_required)
      {
         write("\nYou require greater skill in the following areas to manufacture " + add_article(product_name) + ":\n");

         for (set_count = 0; !has_required && (set_count < sizeof(skill_requirements)); set_count++)
         {
            if (set_count > 0) { write("    or\n"); }

            foreach (mixed requirement in skill_requirements[set_count])
            {
               if (SKILL_D->get_skill_data(requirement[0]))
               {
                  write("  " + SKILL_D->get_skill_data(requirement[0])->proper_name + "\n");
               }
               else
               {
                  write("  " + requirement[0] + "\n");
               }
            }
         }

         return 0;
      }
   }

   if (sizeof(MANUFACTURE_D->query_race_requirements(internal_name)) && (member_array(player->query_race(), MANUFACTURE_D->query_race_requirements(internal_name)) == -1))
   {
      write("Only a member of the following races can manufacture " + add_article(product_name) + ": " + title_capitalize(implode(MANUFACTURE_D->query_race_requirements(internal_name), ", ")) + ".\n");
      return 0;
   }

   tools_missing = check_tools(tool_list);

   if (sizeof(items_missing) || sizeof(tools_missing))
   {
      if (sizeof(items_missing))
      {
         write("\nYou are missing the following materials:\n");

         foreach (string name, int amount in items_missing)
         {
            write(sprintf("%3i %s\n", amount, name + (amount > 1 ? "s" : "")));
         }
      }

      if (sizeof(tools_missing))
      {
         write("\nYou are missing the following tools:\n");

         foreach (string name, int location in tools_missing)
         {
            if (location == 0)
            {
               write("  " + add_article(name) + " in a room or in your inventory\n");
            }
            else if (location == 1)
            {
               write("  " + add_article(name) + " in your inventory\n");
            }
            else if (location == 2)
            {
               write("  " + add_article(name) + " in a room\n");
            }
         }
      }

      write("\n");

      return 0;
   }

   if (load_object(file_name))
   {
      new(file_name)->manufacture_object(this_body());
   }
   else
   {
      write("Unable to manufacture " + product_name + ", please tell a wizard.\n");

      return 0;
   }

   return 1;
}