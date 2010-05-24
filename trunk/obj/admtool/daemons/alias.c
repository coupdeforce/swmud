// Last edited by deforce on 03-18-2010
inherit "/obj/admtool/internal/base.c";
inherit CLASS_ALIAS;

nomask string module_name() { return "alias"; }
nomask string module_key() { return "a"; }
nomask string module_user() { return "[Mudlib:daemons]"; }
nomask string module_priv() { return "Mudlib:daemons"; }

private nomask void add_alias(string name, string expansion, int dev, string xinfo)
{
   int xverb;

   switch (xinfo)
   {
      case "y":
         xverb = 1;
         break;
      case "":
      case 0:
      case "n":
         break;
      default:
         write("**Invalid selection.\n");
         return;
   }

   do_modal_func();
   ALIAS_D->add_default_alias(name, expansion, xverb, dev);
   write("Done.\n");
}

private nomask void handle_l(string cmd)
{
   string a;
   mixed alias_info;
   string array xaliases;
   class alias val;
   string output;
   int i;
   mapping aliases;

   alias_info = ALIAS_D->query_default_aliases();

   if (cmd == "L")
   {
      aliases = alias_info[2];
      xaliases = alias_info[3];
   }
   else
   {
      aliases = alias_info[0];
      xaliases=alias_info[1];
   }

   output = sprintf("Alias: %7s Expansion: %27s Defaults:\n", "", "");
   output += repeat_string("-", 77) + "\n";

//    foreach (a, val in aliases)
   foreach (a in sort_array(keys(aliases), 1))
   {
      val = aliases[a];
      output += sprintf("%-14s %-38s ", a, val->template);

      if ((sizeof(val->defaults) == 1) && (val->defaults[0] == ""))
      {
         output += "\n";
      }
      else
      {
         output += "$*: " + val->defaults[0] + "\n";

         for (i = 1; i < sizeof(val->defaults); i++)
         {
            output += sprintf("%54s$%d: %s\n", "", i, val->defaults[i]);
         }
      }
   }

   more(output);
}

private nomask void handle_a(string cmd, string name,string expansion)
{
   int type;

   if (cmd == "a")
   {
      type = 0;
   }
   else
   {
      type = 1;
   }

   modal_func((: add_alias, name, expansion, type :), "Xalias?  [yn (default n)] ");
}

private nomask void handle_r(string cmd, string input)
{
   mixed alias_info;

   if (cmd == "r")
   {
      alias_info = ALIAS_D->query_default_aliases()[0];
   }
   else
   {
      alias_info = ALIAS_D->query_default_aliases()[2];
   }

   if (undefinedp(alias_info[input]))
   {
      if (cmd == "r")
      {
         printf("%s wasn't a player alias.\n", input);
      }
      else
      {
         printf("%s wasn't a wizard alias.\n", input);
      }

      return;
   }

   if (cmd == "r")
   {
      ALIAS_D->remove_default_alias(input);
   }
   else
   {
      ALIAS_D->remove_default_alias(input, 1);
   }

   write("Done.\n");
}

nomask class command_info array module_commands()
{
   return ({
      new(class command_info,
         key : "l",
         action : (: handle_l, "l" :),
         desc : "list global player aliases"),
      new(class command_info,
         key : "L",
         action : (: handle_l, "L" :),
         desc : "list global wizard aliases"),
      new(class command_info,
         key : "a",
         proto : "[alias [expansion]]",
         args : ({ "Alias: ", "Expansion: "}),
         action : (: handle_a, "a" :),
         desc : "add a global player alias"),
      new(class command_info,
         key : "A",
         action : (: handle_a, "A" :),
         args : ({"Alias: ","Expansion: "}),
         proto : "[alias [expansion]]",
         desc : "add a global wizard alias"),
      new(class command_info,
         key : "r",
         proto : "[alias]",
         args: ({"Alias: "}),
         action : (: handle_r, "r" :),
         desc : "remove a global player alias"),
      new(class command_info,
         key : "R",
         action : (: handle_r, "R" :),
         proto : "[alias]",
         args: ({"Alias: "}),
         desc : "remove a global wizard alias")
   });
}