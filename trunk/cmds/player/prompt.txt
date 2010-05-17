// Last modified by deforce on 01-22-2007
inherit CMD;
inherit M_ANSI;

private void main(string arg)
{
   if (!wizardp(this_body()) && arg)
   {
      string array substitution = ({ "%h", "%m", "%h/%m", "%t", "%j", "%t/%j", "%e", "%c", "%b" });
      int option_count = sizeof(substitution);
      mapping used = ([ ]);
      string prompt = "";

      foreach (string item in explode(arg, " "))
      {
         int number = to_int(item) - 1;

         if ((number >= 0) && (number < option_count) && substitution[number] && !used[number])
         {
            prompt += substitution[number] + " ";
            used[number] = 1;
         }
      }

      prompt += "> ";

      this_user()->query_shell_ob()->set_variable("PROMPT", prompt);
   }
   else if (wizardp(this_body()) && arg)
   {
      this_user()->query_shell_ob()->set_variable("PROMPT", arg);
   }
   else
   {
      this_user()->query_shell_ob()->set_variable("PROMPT", "> ");
   }
}