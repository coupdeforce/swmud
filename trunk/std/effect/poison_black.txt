// Last edited by deforce on 09-26-2009
inherit "/std/effect/poison_blue";

void setup(object target, object source, object apparatus, int rank, int level)
{
   int damage = (rank * 2) + random(ceil(level / 5.0) + 5);
   set_poison_strength(rank + to_int(ceil(level / 10.0)) + 2);

   if (!present("inject_damage", source))
   {
      new("/d/obj/inject_damage", apparatus->short(), "inject_damage")->move(source);
   }

   source->add_event(target, present("inject_damage", source), target->query_random_armor_slot(), damage);

   source->start_fight(target);
   source->handle_events();

   set_effect_duration(query_poison_strength());
   set_effect_delay(12);
   set_effect_type("poison damage");
   set_effect_name("poison");
}