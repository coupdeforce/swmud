// Last updated by deforce on 04-01-2008
#include <config.h>
#include <daemons.h>
#include <security.h>

string query_userid();

void save_me();
void remove();
void initialize_user();
void report_login_failures();

void modal_simple(function input_func, mixed prompt, int secure, int lock);
varargs void modal_push(function input_func, mixed prompt, int secure, function return_to_func);
void modal_pop();

void set_privilege(mixed priv); // from M_ACCESS
mixed unguarded(mixed priv, function fp);

void start_shell();
void run_login_script();

int query_n_gen();
void incarnate(int isnew, string bfn);

// The file name for the body object
private string body_fname;

// The body object once it has been instantiated
nosave private object body;

nomask string query_body_fname()
{
   return body_fname;
}

nomask object query_body()
{
   return body;
}

// Temp hack for upgrading link files. see restore_me()
protected nomask void set_body_fname(string new_body_fname)
{
   body_fname = new_body_fname;
}

private void load_mailer()
{
   object mailbox;
   int idx;

   mailbox = MAILBOX_D->get_mailbox(query_userid());

   idx = mailbox->first_unread_message();

   if (idx == -1)
   {
      mailbox->set_message_index(mailbox->query_message_count() - 1);
   }
   else
   {
      mailbox->set_message_index(idx);

      write("\n\nYou have " + mailbox->query_unread_count() + " unread mail" + ((mailbox->query_unread_count() > 1) ? "s" : "") + " out of " + mailbox->query_message_count() + " message" + ((mailbox->query_message_count() > 1) ? "s" : "") + ".\n");
   }
}

varargs nomask void switch_body(string new_body_fname, int permanent)
{
   object where;
   object old_body;

   if (previous_object() != body && this_body() != body)
   {
      error("security violation: bad body switch attempt\n");
   }

   where = (body ? environment(body) : (mixed)VOID_ROOM);

   if (permanent && new_body_fname)
   {
      body_fname = new_body_fname;
      save_me();
   }

   if (!new_body_fname) { new_body_fname = body_fname; }

   old_body = body;
   body = new(new_body_fname, query_userid());
   master()->refresh_parse_info();

   if (old_body)
   {
      old_body->move(VOID_ROOM);

      if (old_body) { catch(destruct(old_body)); }
   }

   load_mailer();
   report_login_failures();

   /* NOTE: we are keeping the same shell for now... */

   body->su_enter_game(where);
}


void choose_stats()
{
   write("choose_stats");
   // modal_pop();
}

string stat_prompt()
{
   return "Stats";
}

// Starts the character creation phase
private nomask void char_create(int is_new, string bfn)
{
   if (is_new)
   {
      modal_push((: choose_stats :), (: stat_prompt :));
   }

   incarnate(is_new, bfn);
}

// Functions to get the body set up and the user into the game.
private nomask void incarnate(int is_new, string bfn)
{
   if (bfn) { body_fname = bfn; }

   body = new(body_fname, query_userid());
   master()->refresh_parse_info();

   LAST_LOGIN_D->register_last(query_userid(), query_ip_name(this_object()));

   if (query_n_gen() != -1) { body->set_gender(query_n_gen()); }

   save_me();

   if (is_new)
   {
      body->set_start_location(NEW_PLAYER_START);
   }

   start_shell();
   body->enter_game(is_new);
   initialize_user();
   run_login_script();

   if (is_new)
   {
      //new(PLAYER_MENU)->start_menu();

      this_body()->move(CREATE_ROOM);
      this_body()->init_stats();
      this_body()->set_max_health(20);
      this_body()->set_health(20);
      this_body()->add_experience(1000);
      this_user()->query_shell_ob()->set_variable("path", ({ "/cmds/player" }));
      this_user()->query_shell_ob()->set_variable("PROMPT", "%h/%m > ");
      this_body()->set_points_remaining(225);
      this_body()->set_character_birthday(LAST_LOGIN_D->query_last(query_userid())[0]);
      this_body()->set_total_connected_time(0);
      this_body()->set_title("$N the Bantha Fodder");

      new(CREATE_MENU)->start_menu();

      body->save_me();
   }
}

void sw_body_handle_existing_logon(int);

private nomask void rcv_try_to_boot(object who, string answer)
{
   answer = lower_case(answer);
   if ((answer == "yes") || (answer == "y"))
   {
      /* Check this again, in case the user quits after the question is asked
      ** but before this point. If 'who' exists, give them a message and steal
      ** the body, otherwise not. */
      if (who)
      {
         who->receive_private_msg("You are taken over by yourself.\n");
         body=who->query_body();
         who->steal_body();
         start_shell();
         body->reconnect(this_object());
         return;
      }

      sw_body_handle_existing_logon(0);

      return;
   }
   if ((answer == "n") || (answer == "no"))
   {
      if (wizardp(query_userid()))
      {
         sw_body_handle_existing_logon(1);

         return;
      }

      write("Try another time then.\n");

      destruct(this_object());
   }

   write("please type 'y' or 'n'  >");
   modal_simple((: rcv_try_to_boot, who :), 0, 0, 1);
}

protected nomask void sw_body_handle_existing_logon(int enter_now)
{
   remove_call_out(); /* all call outs */

   if (!enter_now)
   {
      // Okay... an existing user is ready for their body.  Look for other
      // users currently connected with this userid.  Those other users may
      // be interactive or link-dead.  Do the right thing...
      object array users;
      string array ids;
      int idx;
      object the_user;

      // adjust the privilege of the user ob
      if (adminp(query_userid())) { set_privilege(1); }
      else { set_privilege(query_userid()); }

      // check for link-deadedness
      users = children(USER_OB) - ({ this_object() });
      ids = users->query_userid();

      if ((idx = member_array(query_userid(), ids)) != -1)
      {
         if (!interactive(the_user = users[idx]))
         {
            if (body = the_user->query_body())
            {
               master()->refresh_parse_info();
               the_user->steal_body();
               start_shell();
               body->reconnect(this_object());
               body->move(body->query_last_location());

               return;
            }
         }
         else
         {
            write("\nYou are already logged in!\nThrow yourself off?  ");
            modal_simple((: rcv_try_to_boot, the_user :),0,0,1);

            return;
         }
      }
   }

   load_mailer();
   write("\n" + read_file(MOTD_FILE) + "\n");

   report_login_failures();
   BIRTHDAY_D->report();

   incarnate(0, 0);
}

/* when a user reconnects, this is used to steal the body back */
nomask void steal_body()
{
   /* only USER_OB can steal the body. */
   if (base_name(previous_object()) != USER_OB)
   {
      error("illegal attempt to steal a body\n");
   }

   body = 0;
   remove();
}

#ifdef USE_RACES
void got_entry(function when_done, string line)
{
   mapping races = RACE_D->query_race_data();

   line =  lower_case(line);

   if (line == "list")
   {
      string format;
      int width = 0;

      foreach (string name in races)
      {
         if (strlen(name) > width) { width = strlen(name); }
      }

      format = "%#-75." + (75 / (width + 3)) + "s\n\n";

      write("Please select a race from the following list:\n");
      printf(format, implode(sort_array(keys(races), 1), "\n"));
      return;
   }
   else if (line == "help stats")
   {
      write("\n" + read_file("/help/creation/race_stats") + "\n");
   }
   else if (line == "help races")
   {
      write("\n" + read_file("/help/creation/races_general") + "\n");
   }

   if (races[line])
   {
      modal_pop();
      evaluate(when_done, races[line]);
      return;
   }

   if (sscanf(line, "help %s", line) && races[line])
   {
      write(races[line]->short_description());
      return;
   }

   write("There is no race by that name.\n");
}
#endif /* USE_RACES */

void create_body()
{
#ifndef USE_RACES
   incarnate(1, DIR_RACES "/human");
#else

   string array races = sort_array(RACE_D->query_races(), 1);
   function when_done = (: incarnate, 1 :);

   if (sizeof(races) == 1)
   {
      string default_race = races[0];
      write("You will be a " + default_race + ".\n");
      incarnate(1, DIR_RACES + "/" + default_race);
   }
   else
   {
      string format;
      int width = 0;

      foreach (string name in races)
      {
         if (strlen(name) > width) { width = strlen(name); }
      }

      format = "%#-75." + (75 / (width + 3)) + "s\n\n";

      write("\nPlease select a race from the following list:\n");
      printf(format, implode(races, "\n"));

      write("Type 'help stats' to see a listing of attribute ranges, 'help races' to see a comparison of attribute ranges, 'help race name' for a brief description, and 'list' to show the choices again.  Type the name of a race to select it, once you have made your decision.\n");
      modal_push((: got_entry, when_done :), "Race? ");
   }
#endif /* USE_RACES */
}

// A new character has been created and all inputs have been entered.
// Do a bit of additional work and go for a body.
protected nomask void sw_body_handle_new_logon()
{
   remove_call_out();   /* all call outs */

#ifdef AUTO_WIZ
   /* auto-wiz everybody as they are created */
   write(">>>>> You've been granted automatic guest wizard status. <<<<<\n");
   unguarded(1, (: SECURE_D->create_wizard($(query_userid())) :));
#endif

   /* auto-admin the first wizard if there are no admins */
   {
      string *members = SECURE_D->query_domain_members("admin");

      if (!sizeof(members))
      {
         if (!wizardp(query_userid()))
         {
            unguarded(1, (: SECURE_D->create_wizard($(query_userid())) :));
         }

         write( ">>>>> You have been made admin. Remember to use admtool. <<<<<\n");
         unguarded(1, (: SECURE_D->add_domain_member("admin", $(query_userid()), 1) :));
      }
   }

   /* adjust the privilege of the user ob */
   if (adminp(query_userid())) { set_privilege(1); }
   else { set_privilege(query_userid()); }

   // pass a lfun pointer so that we don't have to worry about validating the call.
   create_body();
}