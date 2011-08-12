#include <playerflags.h>

inherit CMD;
inherit M_ANSI;
inherit CLASS_FINGER;

#include <guild_colors.h>

#define WIDTH this_user()->query_screen_width()
#define DELIM (repeat_string("=", (WIDTH - 1)) + "\n")

string format_idle_time(int time);
string format_status(string user);
string show_location(string line, object body);
int sort_level_name(string first, string second);

string get_who_string()
{
   string output = "";
   string array users_all = sort_array(bodies()->query_userid(), 1);
   string array users_wizard;
   string array users_player;

   users_wizard = filter_array(users_all, (: wizardp($1) :));
   users_player = filter_array(users_all, (: !wizardp($1) :));

   output += "\n\n%^CYAN%^" + colour_center(implode(explode(upper_case(mud_name()), ""), " ")) + "%^RESET%^\n";
   output += "%^CYAN%^" + colour_center(ctime(time()) + "   -   " + sizeof(users_all) + " adventurers") + "%^RESET%^\n";
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
         string line = "%^BOLD%^[";

         if (wizard_level > 3) { line += "Grand Moff"; }
         else if (wizard_level > 2) { line += "Moff"; }
         else if (wizard_level > 1) { line += "Officer"; }
         else if (wizard_level > 0) { line += "Cadet"; }

         line += "] " + format_status(user);

         output += show_location(line, body) + "\n";
      }
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

   foreach (string user in users_player)
   {
      object body = find_body(user);

      if (body)
      {
         string line = get_guild_color(body->query_primary_guild());

         line += format_status(user);

         output += show_location(line, body) + "\n";
      }
   }

   return output;
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

string format_idle_time(int time)
{
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

      if (wizardp(this_user()) && (time > 0)) { output += ", " + (time % 60) + "s"; }

      output += ")";
   }
   else if (wizardp(this_user()) && (time > 0)) { output += " (" + (time % 60) + "s)"; }

   return output;
}

string format_status(string user)
{
   object body = find_body(user);
   class finger user_info = FINGER_D->get_finger_data(user);
   string output = "";

   if (!wizardp(body)) { output += sprintf("[%2i] ", body->query_primary_level()); }

   if (body->test_flag(F_IN_EDIT)) { output += (wizardp(user) ? "** " : "*"); }
   output += (body->get_target() ? "!" : "");
   output += (!body->is_visible() ? "(" : "");

   output += replace_string((body->query_title()), "%^RESET%^", "");

   output += (!body->is_visible() ? ")" : "");
   output += (body->get_target() ? "!" : "");
   if (body->test_flag(F_IN_EDIT)) { output += (wizardp(user) ? " **" : "*"); }

   output += " (" + title_capitalize(user_info->race) + ")";
   if (!wizardp(user)) { output += " (" + (user_info->team == 1 ? "Rebel" : user_info->team == -1 ? "Imperial" : "Neutral") + ")"; }

   output += "%^RESET%^" + (body->test_flag(F_INACTIVE) ? " (away)" : "");
   output += (!body->test_flag(F_INACTIVE) || wizardp(this_body()) ? format_idle_time(query_idle(body->query_link())) : "");

   return output;
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

private void main()
{
   out(get_who_string());
}