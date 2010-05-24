// Last edited by deforce on 02-11-2008
#include <mudlib.h>
#include <config.h>
#include <playerflags.h>

inherit CMD;

private void main(string arg)
{
   object body;
   string name;
   string toggle;
   string array silenced_players = ({ });

   if (strlen(arg))
   {
      if (sscanf(arg, "%s %s", name, toggle) == 2)
      {
         body = find_body(name);

         if (!body)
         {
            out("Could not find player \'" + capitalize(name) + "\'.\n");

            return;
         }

         if (toggle == "on")
         {
            if (!body->test_flag(F_SILENCED))
            {
               body->set_flag(F_SILENCED);

               out(capitalize(name) + " has been silenced.\n");
            }
            else
            {
               out(capitalize(name) + " is currently silenced.\n");
            }
         }
         else if (toggle == "off")
         {
            if (body->test_flag(F_SILENCED))
            {
               body->clear_flag(F_SILENCED);

               out(capitalize(name) + " has been released from silence.\n");
            }
            else
            {
               out(capitalize(name) + " is not currently silenced.\n");
            }
         }
      }
      else
      {
         body = find_body(arg);

         if (!body)
         {
            out("Could not find player \'" + capitalize(name) + "\'.\n");

            return;
         }

         if (body->test_flag(F_SILENCED))
         {
            out(capitalize(arg) + " is currently silenced.\n");
         }
         else
         {
            out(capitalize(arg) + " is not currently silenced.\n");
         }
      }

      return;
   }

   foreach (object player in bodies())
   {
      if (player->test_flag(F_SILENCED))
      {
         silenced_players += ({ capitalize(player->query_userid()) });
      }
   }

   if (sizeof(silenced_players))
   {
      out("The following players are silenced:\n");

      foreach (string user in sort_array(silenced_players, 1))
      {
         out("\n" + user);
      }
   }
   else
   {
      out("There are currently no silenced players.\n");
   }
}