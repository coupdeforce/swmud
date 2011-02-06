// Last modified by deforce on 02-25-2007
inherit CMD;
inherit M_ANSI;
inherit ADVERSARY;

string format_slot(string slot);

object body;

private void main(string arg)
{
   string output = "";

   if (wizardp(this_body()) && find_body(arg)) { body = find_body(arg); }
   else { body = this_body(); }

   output += "Vital:\n" + format_slot("head") + format_slot("torso");
   output += "Accessory:\n" + format_slot("neck") + format_slot("waist");
   output += "Arms:\n" + format_slot("left arm") + format_slot("right arm");
   output += "Hands:\n" + format_slot("left hand") + format_slot("right hand");
   output += "Legs:\n" + format_slot("left leg") + format_slot("right leg");
   output += "Feet:\n" + format_slot("left foot") + format_slot("right foot");

   if (output) { out(output); }
}

string format_slot(string slot)
{
   object armor = body->query_armors(slot);
   string output = "";

   // Unoccupied slots have the player's description
   if (armor && (armor->short() != body->short()))
   {
      output += sprintf("  %%^CYAN%%^%-11s%%^RESET%%^:", title_capitalize(slot));
      output += " %^ORANGE%^" + armor->short() + "%^RESET%^";

      if (wizardp(this_body()))
      {
         output += " (AC %^BOLD%^%^RED%^" + armor->query_armor_class() + "%^RESET%^)";
      }
   }
   else
   {
      if (!sizeof(body->query_armors(slot)))
      {
         output += "  %^CYAN%^" + title_capitalize(slot) + "%^RESET%^";
      }
   }

   return output + "\n";
}