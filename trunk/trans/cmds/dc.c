// Destroys and clones, destroys equipment on old mobs unlike renew
#include <commands.h>

inherit CMD;

private void main(mixed *arg)
{
   object ob, env;
   string file;

   ob = arg[0];

   file = base_name(ob);

   if (file_size(file + ".c") < 0)
   {
      out("dc: file '" + file + "' does not exist (or is a directory).\n");
      return 0;
   }

   out("Recloned '" + ob->short() + "'.\n");

   env = environment(ob);
   destruct(ob);
   new(file)->move(env);
}