// Last edited by deforce on 06-26-2008
inherit CMD;
inherit M_ANSI;

void do_print(string which)
{
   string array colors = ({ });
   string array illegal = ({ });
   mapping null = ANSI_D->query_translations()[1];
   int longest = 0;

   if (which)
   {
      colors = ({ which });
   }
   else
   {
      colors = this_user()->query_colours();

      if (!wizardp(this_user()))
      {
         colors -= ANSI_D->query_restrictions();
      }
   }

   if (!colors)
   {
      write("No colors set.\n");
      return;
   }

   illegal = filter(colors, (: $(null)[$1] == 0 :));
   colors -= illegal;

   foreach (string item in colors)
   {
      if (strlen(item) > longest)
      {
         longest = strlen(item);
      }
   }

   illegal = map(illegal, (: lower_case :));

   if (sizeof(illegal))
   {
      write("Unsupported settings (use 'colors remove whatever'): " + implode(illegal, "\n") + "\n\n");
   }

   colors = map(sort_array(colors, 1), function(string item, int longest)
   {
      string what = this_user()->query_colour(item);

      if (what)
      {
         return sprintf("%-"+longest+"s : %%^%s%%^%s%%^RESET%%^", lower_case(item), upper_case(item), what);
      }
      else
      {
         return sprintf("%-"+longest+"s : (not set)", item);
      }
   }, longest);

   write(colour_table(colors, this_user()->query_screen_width()) + "\n");
}

nomask private void main(string str)
{
   string which, what;

   if (!str)
   {
      do_print(0);
      return;
   }

   if (sscanf(str, "%s %s", which, what) != 2)
   {
      do_print(str);
      return;
   }

   if (which == "remove")
   {
      this_user()->remove_colour(what);
      printf("Setting for '%s' removed.\n", what);
      return;
   }

   this_user()->set_colour(which, what);
   printf("'%s' set to '%%^%s%%^%s%%^RESET%%^'.\n", which, upper_case(which), what);
}