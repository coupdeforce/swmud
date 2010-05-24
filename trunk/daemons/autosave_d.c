// Last modifed by deforce on 03-19-2007
#define CHECK_TIME  30
#define IDLE_TIME   3599
#define SAVE_TIME   719

void check_users();
string list();
string format_idle_time(int time);

private mapping last_time = ([ ]);

void check_users()
{
   int current_time = time();

   foreach (object user in users())
   {
      string username = user->query_userid();

      if (strlen(username) && !last_time[username])
      {
         last_time[username] = current_time;
      }
   }

   foreach (string user in keys(last_time))
   {
      object body = find_body(user);

      if (body)
      {
         int idle_time = query_idle(body->query_link());

         if ((idle_time > IDLE_TIME) && !wizardp(body) && !GROUP_D->member_group(user, "testplayers"))
         {
            tell(body, "You have been idle too long.\n");

            find_user(user)->force_me("quit");
         }
         else if (last_time[user] && ((current_time - last_time[user]) > SAVE_TIME))
         {
            if (idle_time > 120)
            {
               tell(body, "Autosaving." + (idle_time > 1200 ? format_idle_time(idle_time) : "") + "\n");
            }

            body->save_me();
            last_time[user] = current_time;
         }
      }
      else
      {
         map_delete(last_time, user);
      }
   }

   call_out((: check_users :), CHECK_TIME);
}

string list()
{
   string output = "";
   int current_time = time();

   foreach (string user in keys(last_time))
   {
      output += sprintf("%-13s: %i\n", capitalize(user), (SAVE_TIME + 1) - (current_time - last_time[user]));
   }

   return output;
}

string format_idle_time(int time)
{
   string output = "";

   if (time > 59)
   {
      if (time > 3599)
      {
         if (time > 86399)
         {
            output += (time / 86400) + "d, ";

            time = time % 86400;
         }

         output += (time / 3600) + "h, ";

         time = time % 3600;
      }

      output += (time / 60) + "m";

      time = time % 60;
   }

   if (time > 0) { output += ", " + (time % 60) + "s"; }

   return " (idle " + output + ")";
}

void create()
{
   if (clonep())
   {
      destruct(this_object());
      return;
   }

   call_out((: check_users :), CHECK_TIME);
}