// Last edited by deforce on 03-26-2010
inherit "/std/effect";

int poison_strength = 1;

//:FUNCTION set_poison_strength
void set_poison_strength(int strength)
{
   poison_strength = strength;
}

int query_poison_strength()
{
   return poison_strength;
}

void add_poison_strength(int strength)
{
   poison_strength += strength;

   if (poison_strength < 0)
   {
      poison_strength = 0;
   }
}

varargs void setup(int strength)
{
   if (strength) { poison_strength = strength; }

   set_effect_duration(poison_strength);
   set_effect_delay(10);
   set_effect_type("poison damage");
   set_effect_name("poison");
}

void do_effect(object ob)
{
   if (poison_strength > 0)
   {
      int constitution;

      ob->refresh_stats();

      constitution = ob->query_con();

//      ob->simple_action("$N $vtake damage from poison.");

      ob->weaken_us((poison_strength / 2) + random((poison_strength / 2)) + 1);

      poison_strength -= ((constitution / 10) + random((constitution / 10)) + 1);
   }

   if (poison_strength <= 0)
   {
      destruct(this_object());
   }
}

void transfer_to_existing_effect(object new_effect)
{
   add_poison_strength(new_effect->query_poison_strength());
   add_effect_duration(new_effect->query_poison_strength());
}