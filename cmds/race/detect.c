// Last edited by deforce on 11-11-2009
// Rodian and Verpine racial skill
#include <flags.h>

inherit VERB_OB;

void test_flag(int);
void detect(object room);

private mapping translations = ([ "n" : "north", "s" : "south", "e" : "east", "w" : "west", "ne" : "northeast", "nw" : "northwest", "se" : "southeast", "sw" : "southwest", "u" : "up", "d" : "down" ]);

void do_detect_obj(object room)
{
   if (room->is_room())
   {
      object this_body = this_body();

      this_body->add_skill_delay(8);

      if (this_body->test_skill("detect", this_body->query_primary_level() * 10))
      {
         detect(room);
      }
      else
      {
         write("You fail to detect anything in " + room->the_short() + ".\n");
      }
   }
   else
   {
      write(capitalize(room->the_short()) + " is not a room.\n");
   }
}

void do_detect_wrd(string direction)
{
   object room;

   if (translations[direction]) { direction = translations[direction]; }

   room = load_object(environment(this_body())->query_exit_destination(direction));

   do_detect_obj(room);
}

void do_detect()
{
   do_detect_obj(environment(this_body()));
}

mixed can_detect_obj()
{
   object this_body = this_body();

   if ((this_body->query_race() == "rodian") || (this_body->query_race() == "verpine"))
   {
      if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on detection.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to concentrate on detection while in combat.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are too distracted to detect anything.\n";
      }
   }
   else
   {
      return "Only Rodians and Verpine can detect something.\n";
   }

   return 1;
}

mixed can_detect_wrd()
{
   object this_body = this_body();

   if ((this_body->query_race() == "rodian") || (this_body->query_race() == "verpine"))
   {
      if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on detection.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to concentrate on detection while in combat.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are too distracted to detect anything.\n";
      }
   }
   else
   {
      return "Only Rodians and Verpine can detect something.\n";
   }

   return 1;
}

mixed can_detect()
{
   object this_body = this_body();

   if ((this_body->query_race() == "rodian") || (this_body->query_race() == "verpine"))
   {
      if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on detection.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to concentrate on detection while in combat.\n";
      }
      else if (this_body->query_stunned())
      {
         return "You are too distracted to detect anything.\n";
      }
   }
   else
   {
      return "Only Rodians and Verpine can detect something.\n";
   }

   return 1;
}

void detect(object room)
{
   int rank = this_body()->query_skill("detect") / 100;
   object array living_beings = filter(all_inventory(room) - ({ this_body() }), (: $1->is_living() :));

   if (sizeof(living_beings))
   {
      string text = "";

//      if (random(100) > (rank * 10))
//      {
//         living_beings = living_beings - filter(living_beings, (: $1->test_flag(F_HIDDEN) :));
//      }

      foreach (object living in living_beings)
      {
         text += "  " + living->show_in_room() + "\n";
      }

      write("You detect the follow living beings in " + room->the_short() + ":\n" + text);
   }
   else
   {
      write("You detect that there are no living beings in " + room->the_short() + ".\n");
   }
}

void create()
{
   add_rules( ({ "", "OBJ", "WRD" }) );
}