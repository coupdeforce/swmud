// Last edited by deforce on 10-04-2009
mapping skill_titles =
([
   "slash_1h" : ({ "$N the Slasher",
//       ({ "$N the Master of Slashing", "$N the Mistress of Slashing" }), }),
       "$N the Master Slasher", }),

   "slash_2h" : ({ "$N the Double-Slasher",
       "$N the Master Double-Slasher", }),

   "strike_1h" : ({ "$N the Striker",
       "$N the Master Striker", }),

   "strike_2h" : ({ "$N the Double-Striker",
       "$N the Master Double-Striker", }),

   "anatomy" : ({ "$N the Anatomy Student",
      "$N the Master of Anatomy", }),

   "armor_repair" : ({ ({ "$N the Armor Repairman", "$N the Armor Repairwoman", }),
      "$N the Master of Armor Repair", }),

   "bground_check" : ({ "$N the Background Checker",
      "$N the Master of Background Checking", }),

   "blaster_repair" : ({ ({ "$N the Blaster Repairman", "$N the Blaster Repairwoman", }),
      "$N the Master of Blaster Repair", }),

   "fasttalk" : ({ "$N the Fast-Talker",
      "$N the Master of Fast-Talking", }),

   "force_choke" : ({ "$N the Force Choker",
      "$N the Master of Force Choking", }),

   "force_healing" : ({ "$N the Force Healer",
      "$N the Master of Force Healing", }),

   "haggle" : ({ "$N the Haggler",
      "$N the Master of Haggling", }),

   "heal_accel" : ({ "$N the Healing Accelerator",
      "$N the Master of Healing Acceleration", }),

   "lightning" : ({ "$N the Lightning Channeler",
      "$N the Master of Force Lightning", }),

   "tracker" : ({ "$N the Tracker",
      "$N the Master Tracker", }),
]);

mixed get_title(string skill)
{
   return skill_titles[skill];
}