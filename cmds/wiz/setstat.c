// Last edited by deforce on 11-19-2007
inherit CMD;

void show_help();

string array valid_stats = ({ "str", "con", "agi", "dex", "per", "int", "cha", "luc", "for" });

private void main(string arg)
{
   string array stats = ({ });
   string stat_list;
   int value;
   object body = this_body();

   if ((!arg) || (arg == "/h"))
   {
      show_help();

      return;
   }
   else if (sscanf(arg, "%s %d", stat_list, value) == 2)
   {
      if (stat_list == "all") { stats = valid_stats; }
      else
      {
         foreach (string stat in explode(stat_list, ","))
         {
            if ((member_array(stat, valid_stats) != -1) && (member_array(stat, stats) == -1))
            {
               stats += ({ stat });
            }
         }
      }
   }

   if (sizeof(stats))
   {
      foreach (string stat in stats)
      {
         call_other(body, "set_" + stat, value);
      }

      out("Your " + implode(stats, ", ") + (sizeof(stats) > 1 ? " have" : " has") + " been changed.\n");
   }
   else
   {
      out("None of your stats were changed.\n");
   }
}

void show_help()
{
   out("Usage: setstat stat value             - Change an individual stat.\n");
   out("       setstat stat1, stat2... value  - Change multiple stats.\n");
   out("       setstat all value              - Change all stats.\n");
   out("       setstat /h                     - Show this info.\n");
}