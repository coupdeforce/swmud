#include <playerflags.h>

inherit CMD;
inherit M_ANSI;

#include <guild_colors.h>
#include <wizards.h>

#define USER_WIDTH 26
#define WIDTH this_user()->query_screen_width()
#define DELIM (repeat_string("=", (WIDTH - 1)) + "\n")

string format_idle_time(object body);
string format_status(string user);
string show_location(string line, object body);
int sort_level_name(string first, string second);

void main()
{
   string output = "";
   string array users_player = sort_array(bodies()->query_userid(), 1);
   string array users_wizard;

   users_wizard = filter_array(users_player, (: wizardp($1) :));
   users_player = filter_array(users_player, (: !wizardp($1) :));

   output += "\n\n%^CYAN%^" + colour_center(ctime(time()) + "   -   " + implode(explode(upper_case(mud_name()), ""), " ") + "   -   " + sizeof(bodies()) + " adventurers%^RESET%^\n");
   output += DELIM;

   for (int count = 0; count < sizeof(users_wizard); count++)
   {
      object body = find_body(users_wizard[count]);
      int wizard_level = SECURE_D->query_is_wizard(users_wizard[count]);

      users_wizard[count] = sprintf("%02d", wizard_level) + ":::" + users_wizard[count];
   }

   users_wizard = sort_array(users_wizard, "sort_level_name");

   for (int count = 0; count < sizeof(users_wizard); count++)
   {
      string array name = explode(users_wizard[count], ":::");

      users_wizard[count] = name[1];
   }

   foreach (string user in users_wizard)
   {
      object body = find_body(user);

      if (body && (body->is_visible() || wizardp(this_body())))
      {
         int wizard_level = SECURE_D->query_is_wizard(user);
         string line = "%^BOLD%^[" + title_capitalize(get_wizard_title(wizard_level)) + "] " + format_status(user);

         output += show_location(line, body) + "\n";
      }
   }

   if (sizeof(users_player) && sizeof(users_wizard))
   {
      output += DELIM;
   }

   for (int count = 0; count < sizeof(users_player); count++)
   {
      object body = find_body(users_player[count]);
      int primary_level = body->query_primary_level();

      users_player[count] = sprintf("%02d", primary_level) + ":::" + users_player[count];
   }

   users_player = sort_array(users_player, "sort_level_name");

   for (int count = 0; count < sizeof(users_player); count++)
   {
      string array name = explode(users_player[count], ":::");

      users_player[count] = name[1];
   }

   if (sizeof(users_player))
   {
      int columns = floor(WIDTH / USER_WIDTH);
      int line_count = ceil(to_float(sizeof(users_player)) / columns);
      string array lines = ({ });

      foreach (string user in users_player)
      {
         string line = get_guild_color(find_body(user)->query_primary_guild());
         line += format_status(user);
         line += repeat_string(" ", (USER_WIDTH - color_strlen(line)));

         lines += ({ line });
      }

      for (int row_count = 0; row_count < line_count; row_count++)
      {
         string line_output = lines[row_count];

         for (int column_count = 1; column_count < columns; column_count++)
         {
            if (((column_count * line_count) + row_count) < sizeof(lines))
            {
               line_output += lines[(column_count * line_count) + row_count];
            }
         }

         output += line_output + "\n";
      }
   }

   out(output);
}

int sort_level_name(string first, string second)
{
   int first_level = to_int(first[0..1]);
   int second_level = to_int(second[0..1]);
   first = first[2..];
   second = second[2..];

   if (first_level < second_level) { return 1; }
   else if (first_level > second_level) { return -1; }

   return strcmp(first, second);
}

string format_idle_time(object body)
{
   int time = query_idle(body->query_link());
   string output = "";

   if (time > 59)
   {
      output += " (";

      if (time > 3599)
      {
         if (time > 86399)
         {
            output += (time / 86400) + "d, ";

            time = time % 86400;
         }

         output += (time / 3600) + "h, ";

         time = time % 3600;
      }

      output += (time / 60) + "m";

      if (wizardp(this_user()) && wizardp(body) && (time > 0)) { output += ", " + (time % 60) + "s"; }

      output += ")";
   }
   else if (wizardp(this_user()) && (time > 0)) { output += " (" + (time % 60) + "s)"; }

   return output;
}

string format_status(string user)
{
   object body = find_body(user);
   string output = "";

   if (!wizardp(body)) { output += sprintf("[%2i] ", body->query_primary_level()); }

   if (body->test_flag(F_IN_EDIT)) { output += (wizardp(user) ? "** " : "*"); }
   output += (body->get_target() ? "!" : "");
   output += (!body->is_visible() ? "(" : "");
   if (!wizardp(body) && body->test_flag(F_INACTIVE)) { output += "["; }

   output += capitalize(user);

   if (!wizardp(body) && body->test_flag(F_INACTIVE)) { output += "]"; }
   output += (!body->is_visible() ? ")" : "");
   output += (body->get_target() ? "!" : "");
   if (body->test_flag(F_IN_EDIT)) { output += (wizardp(user) ? " **" : "*"); }

   output += "%^RESET%^" + (wizardp(user) && body->test_flag(F_INACTIVE) ? " (away)" : format_idle_time(body));

   if (wizardp(body)) { return output; }

   return output[0..USER_WIDTH + (strlen(output) - color_strlen(output)) - 1];
}

string show_location(string line, object body)
{
   if (wizardp(this_user()))
   {
      string room_desc = (environment(body) ? environment(body)->get_brief() : "(Nowhere)");
      int free_space = WIDTH - color_strlen(line) - 7;

      if (color_strlen(room_desc) < free_space)
      {
         line += repeat_string(" ", (free_space + 4 - color_strlen(room_desc)));
         line += " " + room_desc;
      }
      else
      {
         room_desc = room_desc[0..(free_space - 4)];

         if (color_strlen(room_desc) > (free_space - 3))
         {
            room_desc = colour_truncate(room_desc, (free_space - 3));
         }

         line += repeat_string(" ", (free_space + 1 - color_strlen(room_desc)));
         line += " " + room_desc + "...";
      }
   }

   return line;
}