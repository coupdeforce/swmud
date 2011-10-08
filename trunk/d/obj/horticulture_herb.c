inherit OBJ;
inherit M_GETTABLE;
inherit M_VALUABLE;

private string type = "healing";
private int potency = 20;

void mudlib_setup()
{
   m_valuable::mudlib_setup();
   add_save(({ "type", "potency" }));
}

varargs void setup(string set_type, int set_potency)
{
   if (strlen(set_type))
   {
      type = set_type;
   }

   if (set_potency)
   {
      potency = set_potency;
   }

   set_id(type + " herb", "herb", "horticulture_herb");
   set_long("An herb that was harvested from a " + type + " plant.");
   set_mass(10);
   set_value(4 * potency);
}

string query_herb_type() { return type; }
int query_herb_potency() { return potency; }
mixed direct_eat_obj() { return 1; }
int indirect_combine_obj_with_obj() { return 1; }

void do_check_obj()
{
   if (this_body()->query_race() == "ithorian")
   {
      write("You check " + this_object()->the_short() + " and discover that it has " + potency + "% of its maximum potency.\n");

      return;
   }

   write("It looks like a normal " + type + " herb.\n");
}

void do_eat_obj()
{
   this_body()->simple_action("$N $veat " + add_article(this_object()->short()) + ".");

   if (type == "healing")
   {
      this_body()->heal_us(potency);
   }
   else if (type == "antidote")
   {
      object toxin = body->has_effect("poison");

      if (toxin)
      {
         toxin->add_poison_strength(potency * -1);
         write("%^BOLD%^You feel no effect.%^RESET%^\n");
      }
      else
      {
         write("%^BOLD%^You feel no effect.%^RESET%^\n");
      }
   }

   destruct(this_object());
}