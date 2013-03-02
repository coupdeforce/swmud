#include <mudlib.h>

inherit CMD;

void create()
{
   ::create();
   no_redirection();
}

private void main(string orig_input)
{
   object old;
   string array parts;
   object where;
   string cmd;

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   old = environment(this_body());
   parts = explode(orig_input," ");
   where = find_body(parts[0]);
   cmd = implode(parts[1..], " ");

   if (!where)
   {
      where = find_object(parts[0]);
   }

   if (!where)
   {
      outf("Could not find object\n");
      return;
   }

   this_body()->move(where);
   this_user()->force_me(cmd);

   if (old)
   {
      this_body()->move(old);
   }
   else
   {
      outf("Could not move back to old room.\n");
   }

   return;
}