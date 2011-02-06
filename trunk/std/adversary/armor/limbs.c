// Last edited by deforce on 11-29-2007
inherit CLASS_EVENT_INFO;

int is_limb(string);
string array query_limbs();
string query_random_limb();
string array query_non_limbs();
varargs int query_max_health(string);

class wear_info
{
   object primary;
   object array others;
}

private mapping armors = ([ ]);

class wear_info find_wi(string slot)
{
   class wear_info wi;
   wi = armors[slot];

   if (!wi)
   {
      if (!is_limb(slot)) { return 0; }

      wi = armors[slot] = new(class wear_info);
   }

   return wi;
}

//:FUNCTION query_armors
// object array query_armors(string slot);
// Returns the armors that are covering limb 'slot'.
object array query_armors(string slot)
{
   class wear_info wi;

   if (query_max_health(slot) == -1) { return 0; }

   wi = find_wi(slot);

   if (!wi) { return ({ this_object() }); }

   if (wi->primary)
   {
      return wi->others ? (({ this_object() }) + ({ wi->primary }) + wi->others) : (({ this_object() }) + ({ wi->primary }));
   }
   else
   {
      return wi->others ? ({ this_object() }) + wi->others : ({ this_object() });
   }
}

//:FUNCTION wear_item
// nomask int wear_item(object what, string where);
// Forces the adversary to wear 'what' on its 'where' limb.
nomask int wear_item(object what, string where)
{
   class wear_info wi;
   array also;
   wi = find_wi(where);

   if (!wi || wi->primary) { return 0; }

   wi->primary = what;
   also = what->also_covers();

   if (also)
   {
      foreach (string limb in also)
      {
         if (wi = find_wi(limb))
         {
            if (wi->others) { wi->others += ({ what }); }
            else { wi->others = ({ what }); }
         }
      }
   }

   return 1;
}
//:FUNCTION remove_item
// nomask int remove_item(object what, string where);
// Removes armor 'what' from the 'where' limb.
nomask int remove_item(object what, string where)
{
   class wear_info wi;
   string array also;

   if (!(wi = armors[where]) || wi->primary != what) { return 0; }

   wi->primary = 0;

   if(wi->others == 0) { map_delete(armors, where); }

   also = what->also_covers();

   if (also)
   {
      foreach (string limb in also)

      if (wi = find_wi(limb))
      {
         wi->others -= ({ what });

         if(sizeof(wi->others) == 0)
         {
            if (wi->primary == 0) { map_delete(armors, limb); }
            else { wi->others = 0; }
         }
      }
   }

   return 1;
}
//:FUNCTION has_body_slot
// int has_body_slot(string slot);
// Returns 1 if the body slot is a valid one.
int has_body_slot(string slot)
{
   return is_limb(slot);
}

//:FUNCTION query_armor_slots
// string array query_armor_slots()
// Returns all valid armor slots on an adversary.
string array query_armor_slots()
{
   return query_limbs() - query_non_limbs();
}

string query_random_armor_slot()
{
   return query_random_limb();
}

int query_ac(string limb)
{
   return query_armors(limb)[sizeof(query_armors(limb)) - 1]->query_armor_class();
}

object array event_get_armors(class event_info evt)
{
   return query_armors(evt->target_extra);
}

void reset_armors()
{
   armors = ([ ]);
}