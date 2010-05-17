// Last modified by deforce on 04-21-2010
// By Beek.  Merged in some functionality written by Valentino.

inherit M_ACTIONS;
inherit M_INPUT;

void continue_conversation(object, string);
void simple_action(string);
string query_name();
string get_current_player_name();
void set_goodbye(mixed arg);
void set_options(mapping m);
void add_options(mapping m);
void set_responses(mapping m);
void add_responses(mapping m);
void add_option_block(string option, string player);
int has_option_block(string option, string player);
mapping get_option_blocks();
mapping get_option_data();
mapping get_response_data();
object get_current_player();

mapping options = ([ ]);
mapping responses = ([ ]);
mapping option_blocks = ([ ]);

mapping current = ([ ]);
mapping start = ([ ]);
array default_start = ({ });
mixed goodbye_action;
int is_busy = 0;
int last_response_time = 0;
object current_player;

void set_goodbye(mixed arg)
{
   goodbye_action = arg;
}

void set_options(mapping m)
{
   options = m;
}

void add_options(mapping m)
{
   options += m;
}

void set_responses(mapping m)
{
   responses = m;
}

void add_responses(mapping m)
{
   responses += m;
}

void add_option_block(string option, string player)
{
   if (strlen(option) && strlen(player))
   {
      if (!option_blocks[option])
      {
         option_blocks[option] = ({ });
      }

      option_blocks[option] += ({ player });
   }
}

int has_option_block(string option, string player)
{
   if (strlen(option) && strlen(player))
   {
      if (option_blocks[option])
      {
         return member_array(player, option_blocks[option]) > -1;
      }
   }

   return 0;
}

mapping get_option_blocks()
{
   return option_blocks;
}

mapping get_option_data()
{
   return options;
}

mapping get_response_data()
{
   return responses;
}

varargs void set_start(array a, object target)
{
   if (target) { start[target] = a; }
   else { default_start = a; }
}

void add_start(array a)
{
   default_start += a;
}

string get_current_player_name()
{
   if (current_player)
   {
      return current_player->short();
   }

   return "";
}

object get_current_player()
{
   if (current_player)
   {
      return current_player;
   }

   return 0;
}

mixed direct_talk_to_liv() { return 1; }
mixed direct_talk_with_liv() { return 1; }

void show_menu(object ob)
{
   int n = 1;

   write("\n");

   if (current[ob])
   {
      foreach (string option in current[ob])
      {
         printf("   %%^MENU_CHOICE%%^%2d%%^RESET%%^: %s\n", n++, options[option]);
      }

      printf("    %%^MENU_CHOICE%%^q%%^RESET%%^: Quit talking to " + query_name() + ".\n");

      modal_simple( (: continue_conversation, ob :), "[choice] :> ");
   }
}

void do_action(object ob, mixed action)
{
   string add;
   string remove;

   if (arrayp(action))
   {
      foreach(mixed act in action)
      {
         do_action(ob, act);
      }
   }
   else
   {
      if (functionp(action)) { evaluate(action); }
      else if (stringp(action))
      {
         if (sscanf(action, "%s@@%s", action, add) == 2)
         {
            if (sscanf(add, "%s@@%s", add, remove) == 2)
            {
               if (remove == "$clear$")
               {
                  current[ob] = ({ });
               }
               else
               {
                  current[ob] -= explode(remove, ",");
               }
            }

            current[ob] += explode(add, ",");
         }

         if (action[0] == '!') { do_game_command(action[1..]); }
         else if (action[0] == '&') { call_other(this_object(), action[1..]); }
         else if (action[0..11] == "$specialize$") { call_other(this_object(), "specialize", action[12..]); }
         else if (action[0..6] == "$train$") { call_other(this_object(), "train", action[7..]); }
         else if (action[0..8] == "$advance$") { call_other(this_object(), "advance", action[9..]); }
         else if (action[0..5] == "$join$") { call_other(this_object(), "join", action[6..]); }
         else { do_game_command("say " + action); }
      }
      else
      {
         if (wizardp(ob))
         {
            if (!action) { write("Error: no response found\n"); }
            else { write("Error: invalid type response\n"); }
         }

         simple_action("$N $vare not sure what to say in response to that.");
         show_menu(ob);
      }
   }
}

void bye(object ob)
{
   map_delete(current, ob);

   if (goodbye_action) { do_action(ob, goodbye_action); }

   is_busy = 0;
}

void continue_conversation(object ob, string input)
{
   int num;
   string tmp;
   string response;
   string tag;

   last_response_time = time();

   if (input == "q") { return bye(ob); }

   if (sscanf(input, "%d%s", num, tmp) != 2 || tmp != "")
   {
//      simple_action("$N $vsuggest that you use the numbers to indicate what you want to say.");
      show_menu(ob);
      return;
   }

   if (num <= 0 || num > sizeof(current[ob]))
   {
//      simple_action("$N $vsuggest that you pick a legal option.");
      show_menu(ob);
      return;
   }

   num--;
   tag = current[ob][num];
   ob->simple_action("$N $vsay: " + options[tag]);
   response = responses[tag];

   do_action(ob, response);

   current[ob] -= ({ tag });

   if (sizeof(current[ob])) { show_menu(ob); }
   else { return bye(ob); }
}

void begin_conversation()
{
   if (!is_busy)
   {
      is_busy = 1;
      last_response_time = time();
      current_player = this_body();
      options = ([ ]);
      responses = ([ ]);
      default_start = ({ });

      call_other(this_object(), "generate_options");
      call_other(this_object(), "generate_training_options");

      current[current_player] = start[current_player] || default_start;

      show_menu(current_player);
   }
   else
   {
      if ((time() - last_response_time) < 90)
      {
         string busy_message = "%^TELL%^" + capitalize(query_name()) + " tells you: %^RESET%^";
         busy_message += "I'm busy with " + current_player->short() + " right now, please wait.\n";
         tell(this_user(), busy_message);
      }
      else
      {
         if (current_player)
         {
            continue_conversation(current_player, "q");
         }

         is_busy = 1;
         last_response_time = time();
         current_player = this_body();
         options = ([ ]);
         responses = ([ ]);
         default_start = ({ });

         call_other(this_object(), "generate_options");
         call_other(this_object(), "generate_training_options");

         current[current_player] = start[current_player] || default_start;

         show_menu(current_player);
      }
   }
}

// Overload this in NPC file
void generate_options()
{
}

// Overloaded in m_teacher
void generate_training_options()
{
}