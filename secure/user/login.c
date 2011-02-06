// Last edited by deforce on 03-11-2008
// Written by Rust Jan 12, 1994
// Deathblade moved to new user/body system on Apr 30, 1995 to
#include <daemons.h>
#include <config.h>
#include <commands.h>
#include <security.h>

string query_userid();
void set_userid(string new_name);
void save_me();
void restore_me(string some_name, int preserve_vars);
void userinfo_handle_logon();
void sw_body_handle_existing_logon(int);
void register_failure(string addr);
varargs void modal_push(function input_func, mixed prompt, int secure, function return_to_func);
varargs void modal_func(function input_func, mixed prompt, int secure);
void modal_pop();
void modal_recapture();
mixed unguarded(mixed priv, function fp);
void initialize_channels();
void display_didlog();

// Login states
#define TIMEOUT                 -1
#define INITIAL_PROMPT          0
#define NAME_PROMPT             1
#define CONFIRM_NEW_NAME        2
#define NEW_PASSWORD            3
#define CONFIRM_PASSWORD        4
#define GET_PASSWORD            10

private string password;

// This is used when we want to get rid of somebody... we have to wait a bit
// for MudOS to flush the buffer before disconnecting them (desting this_object())
//### if MudOS waits before closing the socket, this system may be obsolete
//### I believe it is; MudOS is actually fairly tolerant of desting people
//### from arbitrary routines
private nomask void get_lost()
{
   remove_call_out();
   modal_func((: 1 :), ""); // Ignore all input
   destruct();
}

// Name and password processing
nomask int matches_password(string str)
{
   return crypt(str, password) == password;
}

nomask void set_password(string str)
{
   if (base_name(previous_object()) != CMD_OB_PASSWD)
   {
      error("illegal attempt to set a password\n");
   }

   password = crypt(str, 0);
   save_me();
}

varargs private nomask int check_site(string name)
{
   if (BANISH_D->check_site())
   {
      if (BANISH_D->check_registered(0, name)) { return 1; }

      return 0;
   }

   return 1;
}

private nomask int valid_name(string str)
{
   if (BANISH_D->check_name(str))
   {
      write("Sorry, that name is forbidden.  Please choose another.\n");
      return 0;
   }

   if (!check_site(str))
   {
      printf("Sorry, your site has been banished from %s.  To ask for a character, please mail %s with your desired name.\n",
         mud_name(), ADMIN_EMAIL);
      get_lost();
      return 0;
   }

   if (strlen(str) < 2)
   {
      write("Sorry, that name is too short.  Please choose a name with 2-12 letters.\n");
      return 0;
   }

   if (strlen(str) > 12)
   {
      write("Sorry, that name is too long.  Please choose a name with 2-12 letters.\n");
      return 0;
   }

   // We used to rely on the banish code to do this, but that is a Bad Thing as it is
   // too easy to see those entries deleted.  In particular, we've been distributing the lib
   // for a while now without a banish.o that contains this rule.  To use the banish code
   // to implement a feature here is too convoluted and unintuitive.
   // So... we'll put a regex here to check the name.
   // Note that this regex matches the restriction imposed by the SECURE_D.
   // Also note the name is in lower case right now.
   if (!regexp(str, "^[a-z]+$"))
   {
      write("Sorry, that name is forbidden.  Please choose a name containing only lowercase letters.\n");
      return 0;
   }

   return 1;
}

private nomask void initialize_user()
{
   initialize_channels();
   display_didlog();
}

private nomask int check_special_commands(string arg)
{
   string array b;

   switch(arg)
   {
      case "who":
         b = users()->query_userid();
         b -= ({ "Someone" });
         b -= ({ });
         b-= ({ 0 });
         switch(sizeof(b))
         {
            case 0:
               write("No one appears to be logged on.\n");
               break;

            case 1:
               printf("Only %s is currently on.\n", b[0]);
               break;

            default:
               printf("The following people are logged on:\n%s\n",
               implode(b,", "));
               break;
         }

         return 0;

      case "":
      case "quit":
      case "exit":
      case "leave":
         write("Bye.\n");
         get_lost();
         return 0;

      default:
         return 1;
   }
}

private nomask void modify_guest_userid()
{
   string array userids = users()->query_userid();

   for (int i = 1; ; ++i)
   {
      if (member_array("guest" + i, userids) == -1)
      {
         set_userid("guest" + i);
         save_me();
         return;
      }
   }
}

// Handles most of Phase 0 of the login process.  At this point, we have neither a
// shell nor a body.  Note that this routine is written in continuation passing style.
// If you're not familiar with it, don't worry; the idea is quite simple.  Though LPC
// doesn't support threads, it does support including function arguments as parameters.
// In this case, we use one of the function parameters ('state') to include information
// about where we are in the process, and use a switch() statement to return to where we left
// off when the driver returns information to us.  This allows this to be written as
// one routine, instead of a series of smaller routines.  Every 'break;' statement
// corresponds to waiting for information from the user, and what we are waiting for
// is commented.  The exception is the time_out timer, which transfers us to state -1
// when it goes off.  This had several advantages; first, it removes the need for forward
// declarations; secondly, it allows you to follow the login process simply by reading
// this routine in order, instead of trying to figure out which routine calls which routine.
private nomask varargs void login_handle_logon(int state, mixed extra, string arg)
{
   switch (state)
   {
      string array foo;

      case INITIAL_PROMPT:
         // setup timeout
         call_out((: login_handle_logon, TIMEOUT :), LOGIN_NAME_WAIT);
         write("");
#ifdef WELCOME_DIR
         foo = get_dir(WELCOME_DIR + "/");
         if (sizeof(foo))
         {
            write(read_file(absolute_path(WELCOME_DIR + "/" + choice(foo))));
         }
#else
         write(read_file(WELCOME_FILE));
#endif

// Warning: We have put literally thousands of hours of work into this mudlib,
// and given it to you for free, and all we ask is that you give us credit by
// leaving the lib version alone and visible on the login screen.
// Is that really so much to ask?
// If you think you have changed the lib to the point where you should be
// allowed to change this, ask us first.  Please make sure you have extensively
// modified/rewritten more than half of the base mudlib first
// (intend to modify ... doesn't cut it)
//   printf("%s is based on Lima 1.0b3 and running on %s\n\n", mud_name(), driver_version());
   printf("%s is based on Lima and running on MudOS.\n\n", mud_name());

   write("Hello, Player!\n");

   modal_push((: login_handle_logon, NAME_PROMPT, 0 :), LOGIN_PROMPT);

   // Do this to kick off the modal system and print a prompt
   modal_recapture();
   break;

   // Name prompt
   case NAME_PROMPT:
   if (!arg || (arg == ""))
   {
      write("Sorry, everybody needs a name here.  Please try again.\n");
      return;
   }

   arg = lower_case(arg);

   if (!check_special_commands(arg)) { return; }
   else if (!valid_name(arg)) { return; }
   else if (unguarded(1, (: file_size, LINK_PATH(arg) + __SAVE_EXTENSION__ :)) <= 0)
   {
      modal_func((: login_handle_logon, CONFIRM_NEW_NAME, arg :),
         "\nPlease make sure the name you choose is not based on a prominent character from the movies or expanded universe.\n\nIs '" + capitalize(arg) + "' correct? ");
      return;
   }

   // Always check the site
   if ((arg == "guest") && !check_site()) { return; }

   // Restore the object, without worrying about preserving variables.
   // Note that this sets the userid value.
   restore_me(arg, 0);

   if (arg == "guest")
   {
      modify_guest_userid();
      modal_pop();
      sw_body_handle_existing_logon(1);
      return;
   }

   modal_func((: login_handle_logon, GET_PASSWORD, 0 :), "Password: ", 1);

   // Adjust the time we'll wait for the user
   remove_call_out(); // All call outs
   call_out((: login_handle_logon, -1 :), LOGIN_PASSWORD_WAIT);
   break;

   // Is 'name' correct?
   case CONFIRM_NEW_NAME:
      arg = lower_case(arg);
      switch(arg)
      {
         case "n":
         case "no":
         case "nay":
            modal_func((: login_handle_logon, NAME_PROMPT, 0 :),
               "\nPlease enter your name: ");
               break;
         case "y":
         case "yes":
         case "aye":
#ifdef NO_NEW_PLAYERS
// Added Guest allowance during NO_NEW_PLAYERS
// Vette April 17, 1997
            if (GUEST_D->guest_exists(extra))
            {
               write("Access granted.\n");
               GUEST_D->remove_guest(extra);
            }
            else
            {
               write("Unfortunately, " + mud_name() + " is still in the developmental stage, and is not accepting new users.\n"
                  + "Please mail " + ADMIN_EMAIL + " with any questions.\n");
               get_lost();
               return;
            }
#endif /* NO_NEW_PLAYERS */
            // Begin the character creatin sequence. Store their name.
            set_userid(extra);

            write("\nA new player in our midst!\n");

            modal_func((: login_handle_logon, NEW_PASSWORD, 0 :), "Please enter your password: ", 1);
            break;
         case "maybe":
         case "possibly":
         case "mu":
         case "perhaps":
            write("You can play games once you log in.");
         // Fallthrough
         default:
            write("Please answer Yes or No.\n");
            break;
      }

      break;

      // New password
      case NEW_PASSWORD:

      if (strlen(arg) < 6)
      {
         write("Your password must have at least 6 characters.\n");
         return;
      }

      write("\n"); // needed after a non-echo input

      modal_func((: login_handle_logon, CONFIRM_PASSWORD, crypt(arg, 0) :), "Again to confirm: ", 1);
      break;

      // Confirm password
      case CONFIRM_PASSWORD:
         if (crypt(arg, extra) != extra)
         {
            write("\nPasswords have to match, unlike socks.\n");

            modal_func((: login_handle_logon, NEW_PASSWORD, 0 :), "Password: ", 1);
            return;
         }

         password = extra;
         write("\n"); // Needed after a no-echo input

         // Done with the login sequence.  Pop our input handler now.
         modal_pop();

         // Time to go get some "user" information.
         userinfo_handle_logon();
         break;

      // Password prompt
      case GET_PASSWORD:
         if (matches_password(arg))
         {
            // Done with the login sequence.  Pop our input handler now.
            modal_pop();

            // We need to do work to finish initializing the user. -- Tigran
            initialize_user();
            sw_body_handle_existing_logon(0);
            return;
         }

         register_failure(query_ip_name(this_object()));

         if (extra == 2)
         {
            write("\nYou're just too much for me.\nSorry.\n");

            get_lost();
            return;
         }

         write("\nWrong password, please try again.\n");
         modal_func((: login_handle_logon, GET_PASSWORD, extra + 1 :), "Password: ", 1);
         break;

      case TIMEOUT: // The timer has expired
         write("\nSorry, you've taken too long.\n");
         get_lost();
         break;
   }
}

// Invoked by the driver to set up the object for logging in
private nomask void logon()
{
   login_handle_logon(INITIAL_PROMPT, 0);
}