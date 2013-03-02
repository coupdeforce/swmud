//:COMMAND
// replaces all the instances of str1 with str2 in file
// Usage: replace <str1> <str2> <file>
#include <mudlib.h>

inherit M_GLOB;
inherit CMD;

private void main(mixed* argv, mapping flags)
{
   string file;
   string output = "";

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   argv[0] = translate(argv[0], 1);

   foreach(file in argv[2])
   {
      ed_start(file);

      if (flags["n"])
      {
         ed_cmd("set number");
      }

      output = ed_cmd("1,$g/" + argv[0] + "/p");

      if (output && strlen(output))
      {
         output += sprintf("[Changing in %s]:\n%s\n\n", file, output);

         ed_cmd(sprintf("1,$s/%s/%s/g", argv[0], argv[1]));
         ed_cmd("w");
      }

      ed_cmd("q");
   }

   if (!strlen(output))
   {
      out("No matches found.\n");
   }
   else
   {
      if (flags["q"])
      {
         out("Done.\n");
      }
      else
      {
         out(output);
      }
   }
}