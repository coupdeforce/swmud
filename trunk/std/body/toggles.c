// Last edited by deforce on 02-23-2010
#include <toggles.h>

private mapping toggles = ([ ]);

mixed query_toggle(string toggle)
{
   if (strlen(toggle) && toggles[toggle])
   {
      return toggles[toggle];
   }

   return 0;
}

varargs void set_toggle(string toggle, mixed value)
{
   if (!toggle_data[toggle])
   {
      error(toggle + " is not a valid toggle.");
   }

   if (strlen(toggle) && value)
   {
      toggles[toggle] = value;
   }
   else
   {
      toggles[toggle] = 0;
   }
}