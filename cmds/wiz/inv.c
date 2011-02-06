#include <mudlib.h>
inherit CMD;

private void main()
{
   string array list = ({ });

   foreach (object thing in all_inventory(this_body()))
   {
      list += ({ file_name(thing) });
   }

   if (sizeof(list))
   {
      list = sort_array(list, 1);

      out("\n  " + implode(list, "\n  "));
   }
   else
   {
      out("No objects found.\n");
   }
}