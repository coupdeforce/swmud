// Last edited by deforce on 03-22-2010
// cmd.c -- general command processing
#include <daemons.h>
#include <commands.h>

object query_link();                    // in /std/body.c
void force_look(int force_long_desc);   // in /std/body.c
string move(object location);           // in /std/object/move.c
mixed expand_if_alias(string input);    // in /std/body/alias.c
object query_mailer();                  // in /std/body/mailbase.c

// forward decl
string history_and_alias_processing(string arg);

private nosave string array nonsense_msgs;

string nonsense()
{
   if (!nonsense_msgs)
   {
      nonsense_msgs = MESSAGES_D->get_messages("nonsense");
   }

   return choice(nonsense_msgs) + "\n";
}

mixed *debug_ids(object ob)
{
   return ({ ob, ob->query_id() });
}

varargs nomask int do_game_command(string str, int debug)
{
   mixed result;
   mixed go_result;
   object array objs;
   object rootenv;

   if (this_object()->is_body()) { this_object()->check_wizard_set("make " + this_object()->short() + " do_game_command(" + str + ")", previous_object(-1)); }

   // We can't try parsing the user has no environment.  We should
   // move them to the VOID area and continue processing.
   if (!environment(this_object()))
   {
      write("Oops!  You're lost.  Moving to the void...\n");
      move(load_object(VOID_ROOM));
      force_look(0);
   }

   // First we have to get the list of objects that are going to be available in the parse.
   rootenv = parser_root_environment(environment(this_object()));
//   objs = ({ rootenv, deep_useful_inv_parser_formatted(rootenv) });
   //RABUG(sprintf("do_game_command: (parseable objects: %O)", map(objs, (: debug_ids :) )));
   objs = PARSE_INV_D->get_objects(explode(str, " ")[0], this_object());

   // Parse the player's input
   result = parse_sentence(str, debug, objs);

   // If a string was returned, then the parser figured something out.
   // Write it out and we're done.
   if (stringp(result))
   {
      if (debug) { return result; }

      if (result[<1] != '\n') { result += "\n"; }

      write(result);

      return 1;
   }

   // If the result is 0, the parser didn't know the verb so we keep looking.
   // If a 1 was returned, then nothing more needs to be done.
   // If the result is -1 or -2, the parser figured something was wrong.
   switch(result)
   {
      case 0:
         break;
      case 1:
         return 1;
      case -1:
         write(nonsense());
         return 1;
      case -2:
         write("You aren't able to do that.\n");
         return 1;
      default:
         write("This parser code should never be reached. If it is, let someone know how you got here.\n");
         if (undefinedp(result)) { write("Result was undefined.\n"); }
         else
         {
            write( "Error was: " );
            write(result); write("\n");
         }
         return 1;
   }


   // If in debug mode, we're done
   if (debug) { return 1; }

   // Check if they typed an exit
   go_result = parse_sentence("go " + str);

   if (go_result == 1) { return 1; }
   if (!result) { result = go_result; }

   /* 'You can't go ...' is a parser generated message for general
       failure.  The go command is careful to return explicit
       error messages if the command makes sense, so we can safely
       ignore it. */
   /* With the changing of exits to be more than just strings, the above is
      no longer sufficient.  If the object is not found, an entirely
      different error message is passed that we have to catch as well.
      That one being 'There is no ...' */
   if (stringp(result) && (result[0..12] != "You can't go " && result[0..11] != "There is no "))
   {
      write(result);
      return 1;
   }

   return 0;
}

nomask void force_game_command(string str)
{
   object save_this_user = this_user();

   if (this_object()->is_body()) { this_object()->check_wizard_set("force " + this_object()->short() + " to do_game_command(" + str + ")", previous_object(-1)); }

   set_this_player(query_link());

   if (!do_game_command(str)) { write(nonsense()); }

   set_this_player(save_this_user);
}