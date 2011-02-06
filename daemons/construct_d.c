// Originally by Jym
// Maintains a list of Products that can be crafted
// Used by the craft skill to build a list of craftables

#include <security.h>
#include <classes.h>

inherit M_DAEMON_DATA;

// Product structure
//   key : internal name of product
//   data : filename of the product definition object

private mapping products = ([ ]);

int add_product(string product_name, string product_filename)
{
   products[product_name] = product_filename;

   save_me();

   return 1;
}

int remove_product(string product_name)
{
   if (!products[product_name])
   {
      return 0;
   }

   // Delete the product
   map_delete(products, product_name);

   save_me();

   return 1;
}

string query_product(string product_name)
{
   return products[product_name];
}

string array query_products()
{
   return keys(products);
}

#define PRIV_REQUIRED   "Mudlib:daemons"