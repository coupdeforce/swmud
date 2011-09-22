// Maintains a list of products that can be constructed
#include <security.h>
#include <classes.h>

inherit M_DAEMON_DATA;

private mapping products = ([ ]); // Internal name : product name
private mapping file_names = ([ ]); // Internal name : file name
private mapping has_unique_constructor = ([ ]); // Internal name : 1 or 0 for unique constructor
private mapping guild_requirements = ([ ]); // Internal name : ({ ({ guild_name, guild_level, rank }) })
private mapping skill_requirements = ([ ]); // Internal name : ({ ({ skill_name, skill_level }) })
private mapping learn_requirements = ([ ]); // Internal name : ({ learn_name })
private mapping item_list = ([ ]); // Internal name : ({ ({ internal_name, item_name, number_required, number_destroyed }) })
private mapping tool_list = ([ ]); // Internal name : ({ ({ internal_name, tool_name, location }) location is either 0 (anywhere), 1 (only inventory), 2 (only environment)

int add_product(string internal_name, string product_name, string product_filename, int is_unique_constructor, mixed array product_guild_requirements, mixed array product_skill_requirements, string array product_learn_requirements, mixed array product_item_list, mixed array product_tool_list)
{
   if ((products[internal_name] != product_name) && (member_array(product_name, values(products)) > -1))
   {
      return 0;
   }

   products[internal_name] = product_name;
   file_names[internal_name] = product_filename;
   has_unique_constructor[internal_name] = is_unique_constructor;
   guild_requirements[internal_name] = product_guild_requirements;
   skill_requirements[internal_name] = product_skill_requirements;
   learn_requirements[internal_name] = product_learn_requirements;
   item_list[internal_name] = product_item_list;
   tool_list[internal_name] = product_tool_list;

   save_me();

   return 1;
}

int remove_product(string name)
{
   if (!products[name])
   {
      return 0;
   }

   map_delete(products, name);
   map_delete(file_names, name);
   map_delete(has_unique_constructor, name);
   map_delete(guild_requirements, name);
   map_delete(skill_requirements, name);
   map_delete(learn_requirements, name);
   map_delete(item_list, name);
   map_delete(tool_list, name);

   save_me();

   return 1;
}

string query_internal_name(string name)
{
   foreach (string internal_name in keys(products))
   {
      if (products[internal_name] == name)
      {
         return internal_name;
      }
   }

   return "";
}

string query_product_name(string name)
{
   return products[name];
}

string query_file_name(string name)
{
   return file_names[name];
}

int query_unique_constructor(string name)
{
   return has_unique_constructor[name];
}

mixed array query_guild_requirements(string name)
{
   return guild_requirements[name];
}

mixed array query_skill_requirements(string name)
{
   return skill_requirements[name];
}

string array query_learn_requirements(string name)
{
   return learn_requirements[name];
}

mixed array query_item_list(string name)
{
   return item_list[name];
}

mixed array query_tool_list(string name)
{
   return tool_list[name];
}

string array query_product_internal_names()
{
   return keys(products);
}

string array query_product_names()
{
   return values(products);
}

#define PRIV_REQUIRED   "Mudlib:daemons"