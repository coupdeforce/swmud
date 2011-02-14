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
         destroy_quantity(this_body(), ingredient[0], ingredient[2]);
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
      number_needed = item[2] - count_quantity(this_body(), item[0]);

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

void generate_list(object player)
{
   string array all_products = CONSTRUCT_D->query_product_internal_names();
   string array product_list = ({ });
   mixed array guild_requirements;
   mixed array skill_requirements;
   int has_required;

   foreach (string name in all_products)
   {
      guild_requirements = CONSTRUCT_D->query_guild_requirements(name);
      skill_requirements = CONSTRUCT_D->query_skill_requirements(name);
      has_required = 1;

      foreach (mixed requirement in guild_requirements)
      {
         if (player->query_guild_level(requirement[0]) < requirement[1])
         {
            has_required = 0;
            break;
         }
         else if (player->query_guild_rank(requirement[0]) < requirement[2])
         {
            has_required = 0;
            break;
         }
      }

      foreach (mixed requirement in skill_requirements)
      {
         if (player->query_skill(requirement[0]) < requirement[1])
         {
            has_required = 0;
            break;
         }
      }

      if (!has_required) { continue; }

      product_list += ({ CONSTRUCT_D->query_product_name(name) });
   }

   if (sizeof(product_list))
   {
      write("You know how to construct the following products:\n");

      foreach (string name in sort_array(product_list, 1))
      {
         write("  " + name + "\n");
      }
   }
}

int construct_object(object player, string product_name, string verb_used)
{
   string internal_name = CONSTRUCT_D->query_internal_name(product_name);
   string file_name;
   mixed array guild_requirements;
   mixed array skill_requirements;
   mixed array item_list;
   mixed array tool_list;
   mapping items_missing;
   mapping tools_missing;
   int number_needed;
   int number_destroyed;
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

   file_name = CONSTRUCT_D->query_file_name(internal_name);
   guild_requirements = CONSTRUCT_D->query_guild_requirements(internal_name);
   skill_requirements = CONSTRUCT_D->query_skill_requirements(internal_name);
   item_list = CONSTRUCT_D->query_item_list(internal_name);
   tool_list = CONSTRUCT_D->query_tool_list(internal_name);
   inventory = all_inventory(player);

   // Required guild name: requirement[0]
   // Required level in guild name: requirement[1]
   // Required rank in guild name: requirement[2]
   foreach (mixed requirement in guild_requirements)
   {
      if (player->query_guild_level(requirement[0]) < requirement[1])
      {
         write("You need more experience as a " + title_capitalize(requirement[0]) + " to construct a " + product_name + ".\n");
         return 0;
      }
      else if (player->query_guild_rank(requirement[0]) < requirement[2])
      {
         write("You require greater status as a " + title_capitalize(requirement[0]) + " to construct a " + product_name + ".\n");
         return 0;
      }
   }

   // Required skill name: requirement[0]
   // Amount required in skills name: requirement[1]
   foreach (mixed requirement in skill_requirements)
   {
      if (player->query_skill(requirement[0]) < requirement[1])
      {
         write("You require greater skill in " + SKILL_D->query_skill(requirement[0])[0] + " to construct a " + product_name + ".\n");
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
               write("  a " + name + " in a room or in your inventory\n");
            }
            else if (location == 1)
            {
               write("  a " + name + " in your inventory\n");
            }
            else if (location == 2)
            {
               write("  a " + name + " in a room\n");
            }
         }
      }

      write("\n");

      return 0;
   }

   destroy_ingredients(item_list);

   product = clone_object(file_name);
   product->move(player);

   player->simple_action("$N carefully $vconstruct a $o.", product);
}