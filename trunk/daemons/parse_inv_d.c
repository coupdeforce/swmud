#include <flags.h>

object array get_objects(string verb, object thing);
int test_flag(int);

object array get_objects(string verb, object thing)
{
   switch (verb)
   {
      case "adjust": return all_inventory(thing) + all_inventory(environment(thing));
      case "alter": return all_inventory(thing) + all_inventory(environment(thing));
      case "appraise": return all_inventory(thing) + all_inventory(environment(thing));
      case "attach": return filter_array(all_inventory(thing) + all_inventory(environment(thing)), (: !$1->test_flag(F_HOLSTERED) :));
      case "check": return all_inventory(thing) + all_inventory(environment(thing));
      case "conceal": return filter_array(filter_array(filter_array(filter_array(filter_array(all_inventory(thing), (: !$1->test_flag(F_WORN) :)), (: !$1->test_flag(F_WIELDED) :)), (: !$1->test_flag(F_CONCEALED) :)), (: !$1->is_attached() :)), (: !$1->test_flag(F_LIGHTED) :));
      case "decay": return filter_array(deep_inventory(environment(thing)), (: !environment($1)->is_carry_container() :));
      case "detach": return filter_array(deep_inventory(environment(thing)), (: $1->test_flag(F_HOLSTERED) :)) + filter_array(deep_inventory(environment(thing)), (: $1->has_weapons() :));
      case "drop": return all_inventory(thing);
//      case "get": return deep_inventory(thing) + all_inventory(environment(thing));
      case "hide": return filter_array(filter_array(filter_array(filter_array(filter_array(all_inventory(thing), (: !$1->test_flag(F_WORN) :)), (: !$1->test_flag(F_WIELDED) :)), (: !$1->test_flag(F_CONCEALED) :)), (: !$1->is_attached() :)), (: !$1->test_flag(F_LIGHTED) :));
      case "holster": return filter_array(all_inventory(thing) + all_inventory(environment(thing)), (: !$1->test_flag(F_HOLSTERED) :));
      case "install": return all_inventory(thing);
      case "look": return ({ environment(thing) }) + all_inventory(thing) + all_inventory(environment(thing));
      case "lower": return all_inventory(thing) + all_inventory(environment(thing));
      case "put": return all_inventory(thing) + all_inventory(environment(thing));
      case "raise": return all_inventory(thing) + all_inventory(environment(thing));
      case "remove": return filter_array(all_inventory(thing), (: $1->test_flag(F_WORN) :));
      case "recharge": return all_inventory(thing) + all_inventory(environment(thing));
      case "repair": return filter_array(all_inventory(thing) + all_inventory(environment(thing)), (: $1->test_flag(F_DAMAGED) :) );
      case "resize": return all_inventory(thing) + all_inventory(environment(thing));
      case "sell": return all_inventory(thing);
      case "unconceal": return filter_array(all_inventory(thing), (: $1->test_flag(F_CONCEALED) :));
      case "unholster": return filter_array(deep_inventory(environment(thing)), (: $1->test_flag(F_HOLSTERED) :)) + filter_array(deep_inventory(environment(thing)), (: $1->has_weapons() :));
      case "uninstall": return filter_array(filter_array(deep_inventory(thing), (: $1->is_lightsaber_component() :)), (: environment($1)->is_lightsaber() :)) + filter_array(all_inventory(thing), (: $1->is_lightsaber() :)) + filter_array(filter_array(deep_inventory(thing), (: $1->is_blade_component() :)), (: environment($1)->is_blade() :)) + filter_array(all_inventory(thing), (: $1->is_blade() :)) + filter_array(filter_array(deep_inventory(thing), (: $1->is_armor_component() :)), (: environment($1)->is_modifiable_armor() :)) + filter_array(all_inventory(thing), (: $1->is_modifiable_armor() :)) + filter_array(filter_array(deep_inventory(thing), (: $1->is_blaster_component() :)), (: environment($1)->is_modifiable_blaster() :)) + filter_array(all_inventory(thing), (: $1->is_modifiable_blaster() :));
      case "unwield": return filter_array(all_inventory(thing), (: $1->test_flag(F_WIELDED) :));
      case "wear": return filter_array(all_inventory(thing), (: !$1->test_flag(F_WORN) :));
      case "wield": return filter_array(all_inventory(thing), (: !$1->test_flag(F_WIELDED) :));
   }

   return ({ environment(thing), deep_useful_inv_parser_formatted(environment(thing)) });
}