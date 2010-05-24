#include <mudlib.h>

inherit CMD;

private void main(string str)
{
   string array info = explode(parse_dump(), "\n");
   string line, verb, rule;
   string output = "";
   int ignore = 0;
   int first;
   mapping emote;

   if (str == "") { str = 0; }

   foreach (line in info)
   {
      if (sscanf(line, "Verb %s:", verb))
      {
         if (str && verb != str)
         {
            ignore = 1;
            continue;
         }

         ignore = 0;
         first = 1;
         emote = SOUL_D->query_emote(verb);
      }
      else
      {
         if (ignore) { continue; }

         if (sscanf(line, "  (%*s) %s", rule))
         {
            if (emote && emote[rule]) { continue; }

            if (first)
            {
               output += sprintf("\n%s: ", verb);
               first = 0;
            }
            else
            {
               output += ", ";
            }

            output += rule;
         }
      }
   }

   foreach (string thing in sort_array(explode(output, "\n"), 1))
   {
      out(thing + "\n");
   }
}