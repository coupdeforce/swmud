#include <move.h>

inherit M_INPUT;

object this_body;
object array herbs = ({ });
string array herb_types = ({ });
mapping usable_herbs = ([ ]);
mapping average_potency = ([ ]);
int rank = 0;
int herb_limit = 0;

private int compare_herbs(object herb1, object herb2)
{
   if (herb1->query_herb_potency() > herb2->query_herb_potency())
   {
      return -1;
   }
   else if (herb2->query_herb_potency() > herb1->query_herb_potency())
   {
      return 1;
   }

   return 0;
}

private void manufacture_patch(string type)
{
   int heal_capacity = 0;
   int heal_rate = 0;
   int mass = 0;
   object product;

   foreach (object herb in usable_herbs[type])
   {
      mass += herb->query_mass();
      heal_capacity += herb->query_herb_potency();

      destruct(herb);
   }

   heal_rate = heal_capacity / sizeof(usable_herbs[type]);

   product = clone_object("/d/obj/horticulture_patch", type, heal_capacity, heal_rate);
//   product->clear_id();
//   product->set_id(type + " patch", "patch");
//   product->set_long("A transdermal " + type + " patch, made by an Ithorian from organically grown " + type + " plants.  It will automatically release medication when it's needed, until its reservoir is depleted.");
   product->set_mass(mass);
//   product->set_slot(type + " patch");
   product->set_customize_record("manufactured", this_body->short());
   product->set_autoload();

   if (product->move(this_body) == MOVE_NO_ROOM)
   {
      product->move(environment(this_body));
   }

   this_body->add_experience((this_body->query_primary_level() > 0) ? (sizeof(usable_herbs[type]) * array_sum(this_body->query_guild_levels())) : 10);

   this_body->simple_action("$N $vmanufacture " + add_article(type) + " patch.");

   this_body->test_skill("horticulture");
}

private void type_input(string input)
{
   int choice;

   if (input == "q")
   {
      return;
   }

   choice = to_int(input);

   if ((choice > 0) && (choice <= sizeof(herb_types)))
   {
      manufacture_patch(herb_types[choice - 1]);
   }
}

void show_type_menu()
{
   write("\nSelect the number for the desired type of patch:\n\n");

   for (int count = 0; count < sizeof(herb_types); count++)
   {
      write(sprintf("%2i: %s (%i herbs at %i%% potency)\n", (count + 1), herb_types[count], sizeof(usable_herbs[herb_types[count]]), average_potency[herb_types[count]]));
   }

   modal_simple((: type_input :), "\nEnter blank or 'q' to cancel > ");
}

void manufacture_object(object body)
{
   this_body = body;
   herbs = filter_array(all_inventory(body), (: $1->id("horticulture_herb") :));
   rank = body->query_skill("horticulture") / 100;
   herb_limit = 10 + (rank * 2);

   if (!sizeof(herbs))
   {
      write("You have no herbs that you can use to manufacture a patch.\n");
      return;
   }

//   write("Found " + sizeof(herbs) + " herbs.\n");

   herbs = sort_array(herbs, (: compare_herbs :));

   foreach (object herb in herbs)
   {
      if (member_array(herb->query_herb_type(), herb_types) == -1)
      {
         herb_types += ({ herb->query_herb_type() });
         usable_herbs[herb->query_herb_type()] = ({ });
         average_potency[herb->query_herb_type()] = 0;
      }

      if (sizeof(usable_herbs[herb->query_herb_type()]) < herb_limit)
      {
         usable_herbs[herb->query_herb_type()] += ({ herb });
         average_potency[herb->query_herb_type()] += herb->query_herb_potency();
      }

//      write(herb->query_herb_potency() + "\n");
   }

   foreach (string type in keys(average_potency))
   {
      average_potency[type] /= sizeof(usable_herbs[type]);
   }

   show_type_menu();
}