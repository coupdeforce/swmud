// Last edited by deforce on 02-11-2008
// USAGE: tell <player> <message>
//        tell /h, /history, /last

//:PLAYERCOMMAND
//$$see: reply, tellhist
//USAGE: tell <player> <message>
//
//Sends a message to a player.

#include <mudlib.h>
#include <commands.h>
#include <playerflags.h>

inherit CMD;
inherit M_GRAMMAR;
inherit M_COMPLETE;
inherit M_ANSI;

#define MAX_HISTORY 20

string format_idle_time(int time);

void query_history(string name);
void add_history(string name, string msg);
void clear_history(string name);

private mapping history = ([ ]);

void create()
{
   ::create();
   no_redirection();
}

private void main(string arg)
{
   string user;
   mixed tmp;
   string mystring;
   string deststring;
   object who;

   if ((arg == "/last") || (arg == "/history") || (arg == "/h"))
   {
      string output = "";
      string array messages = this_user()->list_tell_history();

      if (sizeof(messages))
      {
         output = "Tell history:\n";

         foreach (string message in messages)
         {
            output += "\n" + message;
         }
      }
      else { output += "No tell history."; }

      out(output);

      return;
   }

   if (!arg || (sscanf(arg, "%s %s", user, arg) != 2))
   {
      out("Usage: tell <user> <message>\n");

      return;
   }

   if (this_body()->test_flag(F_SILENCED))
   {
      out("You have been silenced from sending tells.\n");

      return;
   }

   who = find_user(lower_case(user));

   if (!who)
   {
      outf("Couldn't find %s.\n", user);

      return;
   }

   if (!interactive(who))
   {
      outf("%s is linkdead.\n", who->query_name());

      return;
   }

   // If someone tries to message themselves
   if (who == this_user())
   {
      out("You look around nervously, and decide that the thoughts you share with yourself are better left unsaid.\n");

      return;
   }

   if ( arg[0] == ':' || arg[0] == ';' )
   {
      array soul_ret;
      int tindex;

      arg = arg[1..];

      soul_ret = SOUL_D->parse_soul(arg);

      if (!soul_ret)
      {
         mystring = sprintf("%%^TELL%%^You emote to %s: %%^RESET%%^%s %s\n",
            capitalize(who->query_userid()), capitalize(this_user()->query_userid()), arg);

         deststring = sprintf("%%^TELL%%^* %s%%^RESET%%^ %s\n", capitalize(this_user()->query_userid()), arg);
      }
      else
      {
         mystring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s", soul_ret[1][0]);

         if ((tindex = member_array(who, soul_ret[0])) == -1)
         {
            deststring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s", soul_ret[1][<1]);
         }
         else
         {
            deststring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s", soul_ret[1][tindex]);
         }
      }
   }
   else
   {
      mystring = sprintf("%%^TELL%%^You tell %s:%%^RESET%%^ %s\n",
         capitalize(who->query_userid()), arg);

      deststring = "%^TELL%^" + capitalize(this_user()->query_userid())
         + " tells you: %^RESET%^" + arg + "\n";
   }

   write(mystring);
   this_user()->add_tell_history(mystring);

   if (who != this_body())
   {
      int idletime = query_idle(who);

      who->receive_private_msg(deststring, MSG_INDENT);
      who->add_tell_history(deststring);
      who->set_reply(this_user()->query_userid());

      if ((idletime > 59) || find_body(user)->test_flag(F_INACTIVE) && !wizardp(who))
      {
         tell(this_user(), capitalize(who->query_userid()) + " has been idle for " + format_idle_time(idletime) + ", and may be unavailable.\n");
      }
      else if (wizardp(who) && find_body(user)->test_flag(F_INACTIVE))
      {
         tell(this_user(), capitalize(who->query_userid()) + " may be unavailable.\n");
      }

      if (find_body(user)->test_flag(F_SILENCED))
      {
         tell(this_user(), capitalize(who->query_userid()) + " has been silenced and is unable to reply.\n");
      }
      else if (find_body(user)->test_flag(F_IN_EDIT))
      {
//         if (find_body(user)->query_variable("edit_block") { out(capitalize(user) + " is blocking tells.\n"); }
         tell(this_user(), capitalize(who->query_userid()) + " is in edit and may be unable to reply.\n");
      }
   }
}

nomask int valid_resend(string ob)
{
   return ob == "/cmds/player/reply";
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
            output += (time / 86400) + " days, ";

            time = time % 86400;
         }

         output += (time / 3600) + " hours, ";

         time = time % 3600;
      }

      output += (time / 60) + " minutes";

      if (time > 0) { output += ", " + (time % 60) + " seconds"; }
   }
   else if (time > 0) { output += (time % 60) + " seconds"; }

   return output;
}