#include <mudlib.h>

inherit CMD;
inherit M_INPUT;

private nomask void handle_piping(string verb, string arg)
{
   if ((arg == "**") || (arg == "."))
   {
      write("Done.\n");
      modal_pop();
      destruct();
      return;
   }

   this_user()->force_me(verb + " " + arg);
}

nomask void start_cmd(mixed * arg)
{
   if (!clonep() || (base_name(previous_object()) != base_name()))
   {
      write("Illegal attempt to spoof command.\n");
      destruct();
      return;
   }

   write("Entering pipe mode. Type '**' to quit.\n");
   modal_push((: handle_piping, arg[0] :), "*\b"); 
}

private nomask void main(mixed * arg, mapping flags, string stdin)
{
   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   if (stdin)
   {
      if (strsrch(arg[0],"$*") != -1)
      {
         foreach (string line in explode(stdin, "\n") - ({ "" }))
         {
            this_user()->force_me(replace_string(arg[0],"$*",line));
            printf(">>%s<<\n", replace_string(arg[0],"$*",line));
         }
      }
      else
      {
         foreach (string line in explode(stdin, "\n") - ({ "" }))
         {
	    this_user()->force_me(arg[0] + " " + line);
         }
      }
   }
   else
   {
      new(file_name())->start_cmd(arg);
   }
}