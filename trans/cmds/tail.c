//:COMMAND
// Prints out the last few lines of a file
// The specification is: tail file *
// We will recieve: ({ array of file names })

#include <mudlib.h>
inherit CMD;

string tail(string file_contents, int n)
{
   if (!file_contents)
   {
      out("ERROR: file too large.\n");
   }
   else
   {
      out(implode(explode(file_contents, "\n")[<n..], "\n"));
   }
}

private void main(mixed *arg, mapping flags, string stdin)
{
   int i;
   int n = 10;

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   if (flags["n"])
   {
      n = to_int(flags["n"]) || 10;
   }

   if (stdin)
   {
      tail(stdin, n);
      return;
   }

   for (i = 0; i < sizeof(arg[0]); i++)
   {
      tail(read_file(arg[0][i]), n);
   }
}