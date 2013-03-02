//:COMMAND
//  Find - Locate files containing a particular pattern
//  Created by Rust, Jul 10, 1995
// See also: findfile
#include <mudlib.h>

inherit CMD;
inherit M_REGEX;
inherit M_GLOB;

private void main(mixed argv, mapping flags)
{
   string file;
   string this_output;

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   argv[1] = decompose(map(argv[1], (:is_directory($1) ? glob($1 + ($1[<1] == '/' ? "*" : "/*")): $1:)));

   if (flags["i"])
   {
      argv[0] = insensitive_pattern(translate(argv[0], 1));
   }

   foreach (file in argv[1])
   {
      ed_start(file);

      if (flags["n"])
      {
         ed_cmd("n");
      }

      this_output = ed_cmd("1, $g/" + argv[0] + "/p");

      if (this_output && strlen(this_output))
      {
         outf("[%s]:\n%s\n\n", file, this_output);
      }

      ed_cmd("q");
   }

   if (!strlen(get_output()))
   {
      out("No matches found.\n");
   }
}