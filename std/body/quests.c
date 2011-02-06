// Last edited by deforce on 03-22-2010
#include <daemons.h>

string array completed_quests = ({ });

void save_me();

int add_quest(string quest)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set quested as completed for " + this_object()->short() + " for quest '" + quest + "'", previous_object(-1)); }

   if (previous_object() != find_object(QUEST_D)) { return 0; }

   if (member_array(quest, completed_quests) != -1) { return 0; }

   completed_quests += ({ quest });
   save_me();

   return 1;
}

int check_quest(string quest)
{
   if (!completed_quests) { return 0; }

   return (member_array(quest, completed_quests) != -1);
}

string array query_quests()
{
   return completed_quests;
}