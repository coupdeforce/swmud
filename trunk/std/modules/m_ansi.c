//:FUNCTION i_use_ansi
// Returns TRUE if the current user (not the object receiving the message!) has ansi turned on.
nomask int i_use_ansi()
{
   return get_user_variable("ansi") != 0;
}

string ansi(string str)
{
   if (i_use_ansi())
   {
      return terminal_colour(str, ANSI_D->query_translations()[0]);
   }
   else
   {
      return terminal_colour(str, ANSI_D->query_translations()[1]);
   }
}

//:FUNCTION colour_strlen
//colour_strlen(s) gives the length of the visible portion of s.  Colour
//codes (e.g. %^GREEN%^) are ignored.
int colour_strlen(string str)
{
   return strlen(terminal_colour(str, ANSI_D->query_translations()[1]));
}

string colour_center(string str)
{
   int len = colour_strlen(str);

   return repeat_string(" ", (this_user()->query_screen_width() - len) / 2) + str;
}

//:FUNCTION colour_truncate
//colour_truncate(str, len) returns a string which consists of len or
//fewer visible characters, not counting colour codes.
string colour_truncate(string str, int len)
{
   string result;
   int idx;

   result = terminal_colour(str, ANSI_D->query_translations()[2], len);

   if ((idx = member_array('\n', result)) == -1)
   {
      return result;
   }

   return result[0..idx-1];
}

varargs string colour_table(array items, int width, int num_horiz)
{
   int max_length = 0;
   int item_count = sizeof(items);
   int size_horiz;
   int num_vert;
   int array lengths = allocate(item_count);
   array lists;
   int count;

   for (count = 0; count < item_count; count++)
   {
      lengths[count] = colour_strlen(items[count]);

      if (lengths[count] > max_length)
      {
         max_length = colour_strlen(items[count]);
      }
   }

   if (num_horiz == 0)
   {
      num_horiz = width / (max_length + 2);

      if (num_horiz == 0)
      {
         num_horiz = 1;
      }

      /* This moves some entries from the right side
       * of the table to fill out the last line,
       * which makes the table look a bit nicer.
       * E.g.
       * (n=13,p=6)      (l=3,p=5)
       * X X X X X X     X X X X X
       * X X X X X X  -> X X X X X
       * X               X X X X
       */

      num_vert = (item_count - 1) / num_horiz + 1;

      if ((item_count > num_horiz) && (item_count % num_horiz))
      {
         num_horiz -= (num_horiz - item_count % num_horiz) / num_vert;
      }
   }
   else
   {
      num_vert = (item_count - 1) / num_horiz + 1;
   }

   size_horiz = width / num_horiz;

   if (item_count < num_horiz)
   {
      num_horiz = item_count;
   }

   lists = map(allocate(num_vert), (: allocate($(num_horiz)) :));

   for (count = 0; count < item_count; count++)
   {
      string tmp;

      if (lengths[count] > size_horiz)
      {
         tmp = colour_truncate(items[count], size_horiz);
      }
      else
      {
         tmp = items[count] + repeat_string(" ", size_horiz - lengths[count]);
      }

      lists[count % num_vert][count / num_vert] = tmp;
   }

//   return implode(map(lists, (: implode($1, "") :)), "\nmax_len is " + max_length + " with width " + width + " with num_horiz " + num_horiz + "\n");
   return implode(map(lists, (: implode($1, "") :)), "\n");
}