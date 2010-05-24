// Last edited by deforce on 05-03-2010
#include <combat_modules.h>

class event_info
{
   object target;
   string target_extra;
   object weapon;
   mixed data;
   object attacker;
}

int event_damage(class event_info evt)
{
   if (mapp(evt->data))
   {
      int damage = 0;

      foreach (string type in keys(evt->data))
      {
         damage += evt->data[type];
      }

      return damage;
   }

   return 0;
}