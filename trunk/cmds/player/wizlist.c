// Last modified by deforce on 03-11-2008
#include <security.h>

inherit CMD;
inherit M_ANSI;
inherit CLASS_FINGER;

#define DELIM repeat_string("-", 79)

private void main()
{
   string array users_wizard = filter_array(SECURE_D->get_wizard_list(), (: wizardp($1) :));
   string array users_admin = filter_array(users_wizard, (: adminp($1) :));
//   users_wizard = filter_array(users_wizard, (: !adminp($1) :));
   string output = DELIM;
   int time_now = time();

   if (sizeof(users_admin))
   {
      for (int count = 0; count < sizeof(users_admin); count++)
      {
         class finger user_info = FINGER_D->get_finger_data(users_admin[count]);

         if (!find_body(users_admin[count]))
         {
            users_admin[count] = user_info->last_login + ":::" + users_admin[count];
         }
         else
         {
            users_admin[count] = time_now + ":::" + users_admin[count];
         }
      }

      users_admin = sort_array(users_admin, -1);

      for (int count = 0; count < sizeof(users_admin); count++)
      {
         int time;
         sscanf(users_admin[count], "%d:::%s", time, users_admin[count]);
      }

      output += "\nAdmins";
      output += repeat_string(" ", 11) + "Last Seen";
      output += repeat_string(" ", 20) + "Mail";
      output += "\n" + DELIM;

      foreach (string user in users_admin)
      {
         class finger user_info = FINGER_D->get_finger_data(user);

         string line = "\n" + capitalize(user);
         line += repeat_string(" ", (17 - strlen(user)));

         if (!find_body(user))
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

   if (sizeof(users_wizard))
   {
      for (int count = 0; count < sizeof(users_wizard); count++)
      {
         class finger user_info = FINGER_D->get_finger_data(users_wizard[count]);

         if (!find_body(users_wizard[count]))
         {
            users_wizard[count] = user_info->last_login + ":::" + users_wizard[count];
         }
         else
         {
            users_wizard[count] = time() + ":::" + users_wizard[count];
         }
      }

      users_wizard = sort_array(users_wizard, -1);

      for (int count = 0; count < sizeof(users_wizard); count++)
      {
         int time;
         sscanf(users_wizard[count], "%d:::%s", time, users_wizard[count]);
      }

      output += "\n" + DELIM;
      output += "\nWizards";
      output += repeat_string(" ", 10) + "Last Seen";
      output += repeat_string(" ", 20) + "Mail";
      output += "\n" + DELIM;

      foreach (string user in users_wizard)
      {
         class finger user_info = FINGER_D->get_finger_data(user);

         string line = "\n" + capitalize(user);
         line += repeat_string(" ", (17 - strlen(user)));

         if (!find_body(user))
         {
            line += ctime(user_info->last_login);
         }
         else
         {
            line += ctime(time());
         }

         line += repeat_string(" ", (47 - strlen(line)));
         line += ((user_info->mail_unread > 0) ? (user_info->mail_unread + " unread") : "");

         output += line;
      }
   }

   out(output + "\n" + DELIM + "\n");
}