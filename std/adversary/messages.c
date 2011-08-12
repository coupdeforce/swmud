// Last edited by deforce on 07-08-2008
void simple_action(string msg, array obs...);
varargs mixed *action(mixed *, mixed, object, object);
void inform(mixed *, mixed, object);
mapping query_combat_messages();

string damage_message(int percent)
{
   if (percent < 1) { return "!none"; }

   switch (percent)
   {
      case 1..3: return "!dam1";
      case 4..7: return "!dam2";
      case 8..11: return "!dam3";
      case 12..15: return "!dam4";
      case 16..19: return "!dam5";
      case 20..23: return "!dam6";
      case 24..27: return "!dam7";
      case 28..31: return "!dam8";
      case 32..35: return "!dam9";
      case 36..39: return "!dam10";
      case 40..43: return "!dam11";
      case 44..47: return "!dam12";
      case 48..51: return "!dam13";
      case 52..55: return "!dam14";
      case 56..59: return "!dam15";
      case 60..75: return "!dam16";
      case 76..91: return "!dam17";
      case 92..107: return "!dam18";
      case 108..123: return "!dam19";
      case 124..139: return "!dam20";
      case 140..155: return "!dam21";
      case 156..99999: return "!dam21";
      default: return "!dam22";
   }
}

void handle_message(string mess, object target, object weapon, object limb)
{
   if (mess[0] == '!')
   {
      string tmp;

      if (weapon)
      {
         tmp = weapon->query_combat_messages()[mess[1..]];
      }
      else
      {
         tmp = query_combat_messages()[mess[1..]];
      }

      if (!tmp)
      {
         simple_action("$N $vare puzzled because $n $vhave no message for '" + mess[1..] + "'.");

         return;
      }

      mess = tmp;
   }

   if (strlen(mess))
   {
      inform(({ this_object(), target }), action(({ this_object(), target }), mess, weapon, target->query_weapon(), limb), environment());
   }
}