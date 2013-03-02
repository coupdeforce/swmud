//_stat.c   (wizard command)
// Beek@ZorkMUD

// This is very OO.  Use it correctly or die. :)
// Basically you can override stat_me() using inheritance.
//
// string stat_me() {
//  string result = ::stat_me();
//  if (this_user()->query_userid()=="beek") {
//    result += "Debug info: i = "+i + "\n";
//  }
//  return result;
// }
#include <mudlib.h>

inherit CMD;

private void main(mixed *arg)
{
   string result;

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   result = arg[0]->stat_me();

   if (!result)
   {
      outf("No information available for %O.\n", arg[0]);
   }
   else
   {
      out(result);
   }
}