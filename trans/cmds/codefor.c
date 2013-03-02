//:COMMAND
//codefor
//By Beek - derived from eval
#include <mudlib.h>

inherit CMD;

void create()
{
   ::create();
   no_redirection();
}

#define SYNTAX "USAGE:  codefor <lpc expression>\n"

string doith(string arg);

private void main(string str)
{
   string tmp_file;
   string initial_write_to_file;
   mixed ret;
   object o;

   if (!wizardp(this_body()))
   {
      out("This command is for wizard use only.\n");
      return;
   }

   initial_write_to_file =
      "#include <mudlib.h> \n"
      "#include <daemons.h>\n"
      "#include <config.h>\n"
      "mixed eval_function(){\n";

   tmp_file = wiz_dir(this_user()) + "/EVAL.c";
   rm(tmp_file);

   if (o = find_object(tmp_file)) { destruct(o); }

   if (!write_file(tmp_file,initial_write_to_file))
   {
      out("eval: Unable to write to " + tmp_file + ". \n");  //shouldn't happen.
      return 0;
   }

   write_file(tmp_file, str + ";\n");
   write_file(tmp_file, "}\n");

   ret = load_object(tmp_file);
   dump_prog(ret, 1, "/tmp/CODE_TMP_FILE");
   ret = read_file( "/tmp/CODE_TMP_FILE");
   rm("/tmp/CODE_TMP_FILE");
   rm( tmp_file );

   ret = explode(ret, ";;;  *** Disassembly ***");
   out(ret[1]);

   destruct(find_object(tmp_file));
   return;
}