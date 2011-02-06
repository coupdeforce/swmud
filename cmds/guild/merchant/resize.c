// Last edited by deforce on 03-15-2010
#include <flags.h>

inherit M_INPUT;
inherit VERB_OB;

void show_resize_menu(object thing);
void resize(object thing, int size);
void assign_flag(int which, int state);
void clear_flag(int);
int test_flag(int);

void do_resize_obj_to_str(object thing, string str)
{
   object this_body = this_body();
   int rank = this_body->query_skill("resize") / 100;
   string array sizes = ({ "very small", "small", "medium", "large", "very large" });
   int size = member_array(lower_case(str), sizes);

   if (thing->test_flag(F_WORN))
   {
      write("You'll need to remove " + thing->the_short() + " before attempting to resize it.\n");

      return;
   }

   if (size == -1)
   {
      write("You don't know how to size something to '" + str + "'.\n");

      return;
   }

   if ((size + 1) == thing->query_body_size())
   {
      write(capitalize(thing->the_short()) + " is already " + sizes[size] + " sized.\n");

      return;
   }

   if (thing->test_flag(F_RESIZED) && (thing->query_resized() > rank) && (rank < 10))
   {
      write("You are unable to resize " + thing->the_short() + " with your current skill level.\nIt looks like it has been resized " + thing->query_resized() + " times.\n");

      return;
   }

   if (thing->is_living())
   {
      if (thing == this_body)
      {
         write("Unless you haven't stopped growing, you're already as big or as small as you're going to be.\n");
      }
      else
      {
         write("You should try appreciating " + thing->short() + " for who " + thing->query_pronoun() + " is, without wanting " + thing->query_objective() + " to be any smaller or larger.\n");
      }

      return;
   }

   if (!thing->query_slots())
   {
      write("The " + thing->short() + " is not something that needs to be worn, and does not need to be a certain size.\n");

      return;
   }

   resize(thing, size + 1);
}

void do_resize_obj(object thing)
{
   object this_body = this_body();
   int rank = this_body->query_skill("resize") / 100;

   if (thing->test_flag(F_WORN))
   {
      write("You'll need to remove " + thing->the_short() + " before attempting to resize it.\n");

      return;
   }

   if (thing->test_flag(F_RESIZED) && (thing->query_resized() > rank) && (rank < 10))
   {
      write("You are unable to resize " + thing->the_short() + " with your current skill level.\nIt looks like it has been resized " + thing->query_resized() + " times.\n");

      return;
   }

   if (thing->is_living())
   {
      if (thing == this_body)
      {
         write("Unless you haven't stopped growing, you're already as big or as small as you're going to be.\n");
      }
      else
      {
         write("You should try appreciating " + thing->short() + " for who " + thing->query_pronoun() + " is, without wanting " + thing->query_objective() + " to be any smaller or larger.\n");
      }

      return;
   }

   if (!thing->query_slots())
   {
      write("The " + thing->short() + " is not something that needs to be worn, and does not need to be a certain size.\n");

      return;
   }

   show_resize_menu(thing);
}

void do_resize()
{
   write("Resize what?\n");
}

private void number_input(object thing, string input)
{
   int choice;

   if (input == "q")
   {
      return;
   }

   choice = to_int(input);

   if ((choice > 0) && (choice < 6))
   {
      if (choice == thing->query_body_size())
      {
         write(capitalize(thing->the_short()) + " is already " + thing->size_description(choice) + " sized.\n");
      }
      else
      {
         resize(thing, choice);
      }
   }
}

void show_resize_menu(object thing)
{
   string array sizes = ({ "very small", "small", "medium", "large", "very large" });

   write("\nSelect the number for the desired size:\n\n");

   for (int count = 0; count < sizeof(sizes); count++)
   {
      write(sprintf("%2i: %s\n", (count + 1), sizes[count]));
   }

   write("\nCurrent size: " + thing->size_description(thing->query_body_size()) + "\n");

   if (!thing->query_resized())
   {
      write("It has never been resized.\n");
   }
   else
   {
      write("It has been resized " + thing->query_resized() + " times.\n");
   }

   modal_simple((: number_input, thing :), "\nEnter blank or 'q' to cancel > ");
}

void resize(object thing, int size)
{
   object this_body = this_body();
   int level = this_body->query_guild_level("merchant");
   int rank = this_body->query_skill("resize") / 100;

   this_body->add_skill_delay(8);

   if (this_body->test_skill("resize", this_body->query_guild_level("merchant") * 10))
   {
      thing->set_body_size(size);
      thing->add_a_resize();

      if ((level + (rank * 5)) > random(100))
      {
         thing->clear_flag(F_RESIZED);

         this_body->simple_action("$N $vresize " + thing->the_short() + " to be " + thing->size_description(size) + ".\n");
      }
      else
      {
         thing->assign_flag(F_RESIZED, 1);

         this_body->simple_action("$N $vresize " + thing->the_short() + " to be " + thing->size_description(size) + ", and $vrealize that it will require a certain skill to be resized again.\n");
      }

      write("It has now been resized " + thing->query_resized() + " times.\n");
   }
   else
   {
      this_body->simple_action("$N $vfail to resize " + thing->the_short() + ".\n");
   }
}

mixed can_resize_obj_to_str()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant"))
   {
      if (!this_body->has_learned_skill("resizing"))
      {
         return "You have not learned how to resize something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to resize somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on resizing something.\n";
      }
   }
   else
   {
      return "Only merchants know how to resize something.\n";
   }

   return 1;
}

mixed can_resize_obj()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant"))
   {
      if (!this_body->has_learned_skill("resizing"))
      {
         return "You have not learned how to resize something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to resize somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on resizing something.\n";
      }
   }
   else
   {
      return "Only merchants know how to resize something.\n";
   }

   return 1;
}

mixed can_resize()
{
   object this_body = this_body();

   if (this_body->query_guild_level("merchant"))
   {
      if (!this_body->has_learned_skill("resizing"))
      {
         return "You have not learned how to resize something.\n";
      }
      else if (this_body->get_target())
      {
         return "You are unable to resize somethng while in combat.\n";
      }
      else if (this_body->has_skill_delay())
      {
         return "You are too busy to concentrate on resizing something.\n";
      }
   }
   else
   {
      return "Only merchants know how to resize something.\n";
   }

   return 1;
}

void create()
{
   add_rules( ({ "", "OBJ", "OBJ to STR" }) );
}