inherit CMD;
inherit M_ANSI;

private void main(string arg)
{
   if (!wizardp(this_body()) && arg)
   {
      if (arg == "clear")
      {
         this_user()->query_shell_ob()->set_variable("PROMPT", "> ");
      }
      else
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
   }
   else if (wizardp(this_body()))
   {
      if (arg)
      {
         this_user()->query_shell_ob()->set_variable("PROMPT", arg);
      }
      else
      {
         out("For help with the wizard prompt, please type \"?\".");
      }
   }
   else if (!wizardp(this_body()))
   {
      string array substitution = ({ "%h", "%m", "%h/%m", "%t", "%j", "%t/%j", "%e", "%c", "%b" });
      int option_count = sizeof(substitution);
      string prompt = this_user()->query_shell_ob()->get_variable("PROMPT");
      string option_list = "";

      foreach (string item in explode(prompt, " "))
      {
         for (int count = 0; count < sizeof(substitution); count++)
         {
            if (substitution[count] == item)
            {
               option_list += (count + 1) + " ";
            }
         }
      }

      out("Your current prompt options are: " + option_list + "\n");
   }
}