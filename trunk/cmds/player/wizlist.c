#include <security.h>

inherit CMD;
inherit M_ANSI;
inherit CLASS_FINGER;

#include <wizards.h>

#define DELIM repeat_string("-", 79)

private void main()
{
   string output = "";
   int time_now = time();

   foreach (int level in keys(query_wizard_titles()))
   {
      string array names = ({ });

      foreach (string name in SECURE_D->get_wizard_list())
      {
         if (SECURE_D->query_is_wizard(name) == level)
         {
            names += ({ name });
         }
      }

      if (sizeof(names))
      {
         output += "\n" + DELIM + "\n" + title_capitalize(get_wizard_title_plural(level));
         output += repeat_string(" ", 11) + "Last Seen";
         output += repeat_string(" ", 20) + "Mail";
         output += "\n" + DELIM;

         names = sort_array(names, 1);

         foreach (string name in names)
         {
            class finger user_info = FINGER_D->get_finger_data(name);

            string line = "\n" + capitalize(name);
            line += repeat_string(" ", (17 - strlen(name)));

            if (!find_body(name))
            {
               line += ctime(user_info->last_login);
            }
            else
            {
               line += ctime(time_now);
            }

            line += repeat_string(" ", (47 - strlen(line)));
            line += ((user_info->mail_unread > 0) ? (user_info->mail_unread + " unread") : "");

            output += line;
         }
      }
   }

   out(output + "\n" + DELIM + "\n");
}