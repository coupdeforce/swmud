#include <move.h>
#include <toolbox.h>

void destroy_quantity(object body, string ingredient, int amount)
{
   int count = 0;

   foreach (object thing in all_inventory(body))
   {
      if (count >= amount) { return; }

      if (thing->id(ingredient))
      {
         destruct(thing);

         count++;
      }
   }
}

void destroy_ingredients(mixed ingredients)
{
   foreach (mixed ingredient in ingredients)
   {
      if (ingredient[2] > 0)
      {
         if (ingredient[0][0..8] == "::parts::")
         {
            remove_parts_from_toolboxes(ingredient[0][9..], ingredient[2]);
         }
         else
         {
            destroy_quantity(this_body(), ingredient[0], ingredient[2]);
         }
      }
   }
}

int count_quantity(object body, string ingredient)
{
   int count = 0;

   foreach (object thing in all_inventory(body))
   {
      if (thing->id(ingredient))
      {
         count++;
      }
   }

   return count;
}

mapping check_items(mixed items)
{
   int number_needed;
   mapping missing = ([ ]);

   foreach (mixed item in items)
   {
      if (item[0][0..8] == "::parts::")
      {
         number_needed = item[2] - check_toolboxes_for_parts(item[0][9..], item[2]);
      }
      else
      {
         number_needed = item[2] - count_quantity(this_body(), item[0]);
      }

      if (number_needed > 0)
      {
         missing[item[1]] = number_needed;
      }
   }

   return missing;
}

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

int test_trained_skills(object player, string array skills)
{
   int passed = 0;

   foreach (string skill in skills)
   {
      if (player->test_skill(skill, 0))
      {
         passed = 1;
      }
   }

   return passed;
}

void generate_list(object player, string product_name)
{
   string array all_products = CONSTRUCT_D->query_product_internal_names();
   mapping product_list = ([ ]);
   mixed array guild_requirements;
   mixed array skill_requirements;
   string array learn_requirements;
   int has_required;
   int set_count;

   foreach (string name in all_products)
   {
      if (strlen(product_name) && (strsrch(CONSTRUCT_D->query_product_name(name), product_name) == -1))
      {
         continue;
      }

      guild_requirements = CONSTRUCT_D->query_guild_requirements(name);
      skill_requirements = CONSTRUCT_D->query_skill_requirements(name);
      learn_requirements = CONSTRUCT_D->query_learn_requirements(name);
      has_required = 1;

      if (has_required && arrayp(guild_requirements) && sizeof(guild_requirements) && arrayp(guild_requirements[0]) && sizeof(guild_requirements[0][0]))
      {
         has_required = 0;

         for (set_count = 0; !has_required && (set_count < sizeof(guild_requirements)); set_count++)
         {
            has_required = meets_guild_requirements(player, guild_requirements[set_count]);
         }
      }

      foreach (string requirement in learn_requirements)
      {
         if (!player->has_learned_construction(requirement))
         {
            has_required = 0;
            break;
         }
      }

      if (has_required && arrayp(skill_requirements) && sizeof(skill_requirements) && arrayp(skill_requirements[0]) && sizeof(skill_requirements[0][0]))
      {
         has_required = 0;

         for (set_count = 0; !has_required && (set_count < sizeof(skill_requirements)); set_count++)
         {
            has_required = meets_skill_requirements(player, skill_requirements[set_count]);
         }

         if (!has_required)
         {
            product_list[name] = CONSTRUCT_D->query_product_name(name) + "%^YELLOW%^*%^RESET%^";
         }
      }

      if (!has_required) { continue; }

      product_list[name] = CONSTRUCT_D->query_product_name(name);
   }

   if (sizeof(product_list))
   {
      write("You know how to construct the following products:\n");

      foreach (string name in sort_array(keys(product_list), 1))
      {
         write("  " + product_list[name] + "\n");
      }
   }
   else
   {
      if (strlen(product_name))
      {
         write("You don't seem to know how to construct anything that contains \"" + product_name + "\".\n");
      }
      else
      {
         write("You don't seem to know how to construct anything yet.\n");
      }
   }
}

int construct_object(object player, string product_name, string verb_used)
{
   string internal_name = CONSTRUCT_D->query_internal_name(product_name);
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

   if ((product_name == "list") || ((strlen(product_name) > 5) && (product_name[0..4] == "list ")))
   {
      generate_list(player, product_name[5..]);

      return 0;
   }

   if (!strlen(internal_name))
   {
      write("\"" + product_name + "\" is not a valid product.\n");
      return 0;
   }

   file_name = CONSTRUCT_D->query_file_name(internal_name);
   guild_requirements = CONSTRUCT_D->query_guild_requirements(internal_name);
   skill_requirements = CONSTRUCT_D->query_skill_requirements(internal_name);
   learn_requirements = CONSTRUCT_D->query_learn_requirements(internal_name);
   item_list = CONSTRUCT_D->query_item_list(internal_name);
   tool_list = CONSTRUCT_D->query_tool_list(internal_name);
   inventory = all_inventory(player);

   foreach (string requirement in learn_requirements)
   {
      if (!player->has_learned_construction(requirement))
      {
         write("You have not learned how to construct " + add_article(product_name) + ".\n");
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
         write("\nYou need more experience in the following guilds to construct " + add_article(product_name) + ":\n");

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
         write("\nYou require greater skill in the following areas to construct " + add_article(product_name) + ":\n");

         for (set_count = 0; !has_required && (set_count < sizeof(skill_requirements)); set_count++)
         {
            if (set_count > 0) { write("    or\n"); }

            foreach (mixed requirement in skill_requirements[set_count])
            {
               if (player->query_skill(requirement[0]) < requirement[1])
               {
                  write("  " + SKILL_D->query_skill(requirement[0])[0] + "\n");
               }
            }
         }

         return 0;
      }
   }

   items_missing = check_items(item_list);
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

   if (sizeof(CONSTRUCT_D->query_trained_skills(internal_name))
      && !test_trained_skills(player, CONSTRUCT_D->query_trained_skills(internal_name)))
   {
      player->simple_action("$N $vfail to construct a " + CONSTRUCT_D->query_product_name(internal_name) + ".");

      return 0;
   }

   if (CONSTRUCT_D->query_unique_constructor(internal_name))
   {
      if (load_object(file_name))
      {
         if (find_object(file_name)->construct_object(this_body()))
         {
            destroy_ingredients(item_list);
         }
         else
         {
            return 0;
         }
      }
      else
      {
         write("Unable to construct " + product_name + ", please tell a wizard.\n");

         return 0;
      }
   }
   else
   {
      product = clone_object(file_name);
      product->set_customize_record("constructed", player->short());

      if (product->move(player) == MOVE_NO_ROOM)
      {
         product->move(environment(player));
      }

      destroy_ingredients(item_list);

      player->simple_action("$N carefully $vconstruct a $o.", product);
   }

   return 1;
}