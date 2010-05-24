#include <mudlib.h>
inherit CMD;
inherit M_ANSI;

private void main(string arg)
{
   string output = "";
   string array messages = this_user()->list_tell_history();

   if (sizeof(messages))
   {
      output += "Tell history:\n";

      foreach (string message in messages)
      {
         output += "\n" + message;
      }
   }
   else { output += "No tell history."; }

   out(output);
}