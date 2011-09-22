inherit M_ACTIONS;

//:MODULE
//m_vendor is used to create vendor objects that buy and sell stuff
//they work as traditional shopkeepers and also bartenders
//See: /domains/std/shopkeeper.c

// TODO
// compatibility of buying and selling vehicles
// problem lies in ob->move(this_object()), and vehicles are too large.

// Shall the vendor sell objects which were in the store-room befor the start?
// If you define this vendors may not share a store-room.
#ifndef VENDOR_SHARE_STORAGE
#define SELL_OLD_OBJECTS
#endif

// the minimal value of sold items (for objects with query_value() == 0)
#ifdef VENDOR_MIN_VALUE
#define MIN_VALUE VENDOR_MIN_VALUE
#else
#define MIN_VALUE 1
#endif

#include <move.h>

class item
{
   string short;
   string long;
   string file;
   int value;
   string array ids;
   int amount;
   object array objects;
}

string short();
string query_subjective();
int check_uniqueness(object ob);
void clear_stock();
void organize_stock();
int sort_short(object first, object second);
varargs string evaluate_deal(int deal_value, int real_value, int is_buying);
int test_flag(int);

private mixed for_sale;
private mixed will_buy;
private mixed will_buy_stolen = 0;
private mixed currency_type = "gold";
private mapping stored_items = ([ ]);
private string unique_inv = "";
private int all_unique = 0;
private int max_item_number = 0;
private float cost_multiplier = 1;
private float sell_divisor = 3;

//:FUNCTION set_cost_multiplier
float set_cost_multiplier(float value)
{
   if (value > 0)
   {
      cost_multiplier = value;
   }
}

//:FUNCTION set_sell_divisor
float set_sell_divisor(float value)
{
   if (value > 0)
   {
      sell_divisor = value;
   }
}

//:FUNCTION selling_cost
// multiply the objects value with cost_mult to get the cost for selling
// This is how much they actually sell for
float selling_cost(float cost)
{
   cost -= random(26);

   if (cost < MIN_VALUE) { return MIN_VALUE * cost_multiplier; }

   return cost * cost_multiplier;
}

//:FUNCTION list_selling_cost
// multiply the objects value with cost_mult to get the cost for selling
// This is how much something is listed to sell for
float list_selling_cost(float cost)
{
   if (cost < MIN_VALUE) { return MIN_VALUE * cost_multiplier; }

   return cost * cost_multiplier;
}

//:FUNCTION buying_value
// Override if you want a different way to determine value
// This is how much a player gets for selling something
float buying_value(float value)
{
   value += random(26);

   if (value < MIN_VALUE)
   {
      if ((MIN_VALUE / sell_divisor) < 1)
      {
         return 1.0;
      }

      return MIN_VALUE / sell_divisor;
   }

   return value / sell_divisor;
}

private class item init_item(object ob)
{
   class item item;

   item = new(class item, short: ob->short(), long: ob->long(),
      ids: ob->query_id() + ({ ob->short(), ob->plural_short() }),
      value: ob->query_value(), amount: 1);

   if (item->long[<1] != '\n') { item->long += "\n"; }

   return item;
}

//:FUNCTION add_sell
//enables you to add items to the vendors stored_item's mapping
void add_sell(string file, int amt)
{
   object ob;
   class item item;

   if (!amt) { amt = 1; }

   if (file)
   {
      if (ob = new (file))
      {
         item = init_item(ob);
         item->amount = amt;
         item->file = base_name(ob);
         stored_items[++max_item_number] = item;
         destruct(ob);
      }
      else { error("No such item: " + file + "\n"); }
   }
}

//:FUNCTION set_sell
// with a mapping you can set many items into the vendor's to sell list
void set_sell(mapping items)
{
   string item;
   int amt;

   foreach (item, amt in items) { add_sell(item, amt); }
}

//:FUNCTION add_sell_object
// adds a unique item to the vendor's stored_items mapping
void add_sell_object(object ob)
{
   class item item;

   foreach (item in values(stored_items))
   {
      if (item->objects && compare_objects(item->objects[0], ob))
      {
         item->objects += ({ ob });
         item->amount++;
         return;
      }
   }

   item = init_item(ob);
   item->objects = ({ ob });
   stored_items[++max_item_number] = item;
}

//:FUNCTION set_for_sale
//Set the array of object names which this living object is willing to sell.
//set_for_sale(1) means everything is for sale.  set_for_sale(0) means nothing
//is.  If a function is passed it will get the object to sell as argument.
//If a single string is returned it will be used as error message.
void set_for_sale(mixed x)
{
   for_sale = x;
}

mixed query_for_sale()
{
   return for_sale;
}

//:FUNCTION set_will_buy
//Set the array of object names which this living object is willing to buy.
//set_will_buy(1) means it will buy anything.  set_will_buy(0) means it wont
//by anything.  If a function is passed it will get the object to buy as
//argument. If a single string is returned it will be used as error message.
void set_will_buy(mixed x)
{
   will_buy = x;
}

mixed query_will_buy()
{
   return will_buy;
}

//:FUNCTION set_will_buy_stolen
//Set the array of object ids which this living object is willing to buy if they've been stolen.
//set_will_buy_stolen(1) means it will buy anything that's stolen.  set_will_buy(0) means it won't
//buy anything that's been stolen.  If a function is passed it will get the object to buy as
//argument. If a single string is returned it will be used as error message.
void set_will_buy_stolen(mixed x)
{
   will_buy_stolen = x;
}

mixed query_will_buy_stolen()
{
   return will_buy_stolen;
}

//:FUNCTION set_currency_type
//Sets the type of currency the vendor will buy/sell in
mixed set_currency_type(string type)
{
   if (!MONEY_D->is_currency(type)) { catch("Invalid currency set in shopkeeper!\n"); }

   currency_type = type;
}

//:FUNCTION query_currency_type
//Queries the type of currency the vendor will buy/sell in
mixed query_currency_type()
{
   return currency_type;
}

// do NOT overload any of the direct or indirect rules. They are not called
// if this is the only vendor and no vender is specified in the call.
// Use set_will_buy() and set_for_sale() instead.

mixed direct_ask_liv_about_wrd(object liv, string item) { return 1; }
mixed direct_ask_liv_about_str(object liv, string item) { return 1; }
mixed direct_list_from_liv(object liv) { return 1; }
mixed direct_list_str_from_liv(string item, object liv) { return 1; }
mixed direct_buy_str_from_liv(string item, object liv) { return 1; }
mixed indirect_sell_obj_to_liv(object ob, object liv) { return 1; }

int test_buy_from(object ob, object seller)
{
   mixed result;

   if (!will_buy)
   {
      do_game_command("say I don't want to buy anything.");

      return 0;
   }

   if (ob->test_flag(F_STOLEN) && !will_buy_stolen)
   {
      do_game_command("say That looks like it was stolen!  Do I look like a fence to you?!?");

      return 0;
   }

   result = evaluate(will_buy, ob);

   if (result == 1) { return 1; }

   if (stringp(result))
   {
      if (result[<1] != '\n') { result += "\n"; }

      write(result);

      return 0;
   }

   if (arrayp(result))
   {
      foreach (string name in result)
      {
         if (ob->id(name)) { return 1; }
      }
   }

   if (member_array(base_name(ob), will_buy) > -1) { return 1; }

   if (ob->test_flag(F_STOLEN) && (member_array(base_name(ob), will_buy_stolen) > -1)) { return 1; }

   write(capitalize(short()) + " doesn't want to buy " + ob->the_short() + ".\n");

   return 0;
}

//FUNCTION: buy_object_from
//gets called from the verb sell. Addes bought object to the list of
//stored_items depending on check_uniqueness()
void buy_object_from(object ob, object seller)
{
   float value;
   float value_adjustment = 0.0;
   mixed item;
   string file;
   mapping money;

   if (!test_buy_from(ob, seller)) { return; }

   value = to_float(ob->query_value());

   if ((seller->query_guild_level("merchant") || seller->query_guild_level("smuggler"))
      && seller->has_learned_skill("haggling") && (seller->query_toggle("haggle") == "on"))
   {
      int level = seller->query_guild_level("merchant") + seller->query_guild_level("smuggler");
      int rank = seller->query_skill("haggle") / 100;

      if (!seller->test_skill("haggle", (level * 10) - ((this_object()->query_cha() - seller->query_cha()) * 10)))
      {
         value_adjustment += (level + (rank * 5.0));
      }
      else
      {
         value_adjustment -= random(80 - level + 1);

         if (this_object()->query_cha() > seller->query_cha())
         {
            value_adjustment -= random(this_object()->query_cha() - seller->query_cha() + 1);
         }
      }
   }

   value_adjustment += (seller->query_cha() / 2.0);

   value += (value * value_adjustment) / 100.0;

   value = buying_value(value);

   if (value < 1.0)
   {
      write("You wouldn't get any " + currency_type + " for " + ob->the_short() + ".\n");

      return;
   }

   if (ob->move(this_object()) != MOVE_OK)
   {
      write("You can't seem to give " + ob->the_short() + " to " + short() + ".\n");

      return;
   }

   money = MONEY_D->calculate_denominations(value, currency_type);

   foreach (string type, int amount in money)
   {
      seller->add_money(type, amount);
   }

   this_body()->my_action("$N $vsell a $o for " + MONEY_D->currency_to_string(money, currency_type)
      + evaluate_deal(value, buying_value(ob->query_value()))
      + ".\n", ob);

   this_body()->other_action("$N $vsell a $o.\n", ob);

   switch (check_uniqueness(ob))
   {
      case 0: /* object is not unique, so just keep the filename. */
      file = base_name(ob);
      destruct(ob);
      foreach (item in values(stored_items))
      {
         if (item->file && item->file == file)
         {
            if (item->amount != -1) { item->amount++; }

            return;
         }
      }

      add_sell(file, 1);
      break;
      case 1: /* object is unique */
      add_sell_object(ob);
      ob->move(load_object(unique_inv));
      break;
      case 2: /* object is destroyable(), so don't sell it */
      destruct(ob);
      break;
   }

   reset_eval_cost();
   organize_stock();
}

//:FUNCTION query_items
//gets called from the verb ask and the rule ask obj about str
//The player commands buy and list use it too.
//This function shows the players what items the shopkeeper has.
//If flag is set the it will show the long() too
int query_items(string item, int flag)
{
   int *keys = ({});
   int key, num;
   string cost;
   int row_count = 1;

   organize_stock();

   if (sizeof(stored_items) == 0 || !for_sale)
   {
      write("This shop has nothing to sell.\n");

      return 0;
   }

   if (item == "all")
   {
      keys = keys(stored_items);
   }
   else
   {
      foreach (key in keys(stored_items))
      {
         if (member_array(item, stored_items[key]->ids) != -1)
         {
            keys += ({ key });
         }
      }
   }

   if (sizeof(keys) == 0)
   {
      write("Nothing in this shop matches that!\n");

      return 0;
   }

   keys = sort_array(keys, 1);
//   printf("%|6s %|8s %-24s %s\n","List #", "Amount", "Name/id", capitalize(currency_type));
   printf("%4s  %-54s   %15s\n", "Qty", "Item", capitalize(currency_type));
   write(repeat_string("-", 79) + "\n");

   foreach (key in keys)
   {
      cost = MONEY_D->currency_to_string(list_selling_cost(stored_items[key]->value), currency_type);

      num = stored_items[key]->amount;

      printf("%s%4s  %-54s   %15s   %s\n", (row_count % 2 ? "%^BOLD%^" : ""), (num != -1 ? sprintf("%d", num) : "*"), stored_items[key]->short[0..45] + stored_items[key]->objects[0]->get_attributes(), cost, (row_count % 2 ? "%^RESET%^" : ""));

      if (flag) { write(stored_items[key]->long + "\n"); }

      row_count++;
   }

   return 1;
}

int test_sell(object ob)
{
   // for_sale == 0 is tested in sell_stored_objects() already
   mixed result = evaluate(for_sale, ob);

   if (result == 1) { return 1; }

   if (stringp(result))
   {
      if (result[<1] != '\n') result += "\n";
      write(result);
      return 0;
   }

   if (arrayp(result))
   {
      foreach (string name in result)
      {
         if (ob->id(name)) { return 1; }
      }
   }

   if (member_array(base_name(ob), for_sale) > -1) { return 1; }

   write(capitalize(short()) + " doesn't want to sell " + ob->the_short() + ".\n");
   return 0;
}

protected int sell_object_to(object ob, object buyer)
{
   float cost;
   float cost_adjustment = 0.0;
   mapping array money;

   if (!test_sell(ob)) { return 0; }

   cost = to_float(ob->query_value());

   if ((buyer->query_guild_level("merchant") || buyer->query_guild_level("smuggler"))
      && buyer->has_learned_skill("haggling") && (buyer->query_toggle("haggle") == "on"))
   {
      int level = buyer->query_guild_level("merchant") + buyer->query_guild_level("smuggler");
      int rank = buyer->query_skill("haggle") / 100;

      if (!buyer->test_skill("haggle", (level * 10) - ((this_object()->query_cha() - buyer->query_cha()) * 10)))
      {
         cost_adjustment += (level + (rank * 5.0));
      }
      else
      {
         cost_adjustment -= random(80 - level + 1);

         if (this_object()->query_cha() > buyer->query_cha())
         {
            cost_adjustment -= random(this_object()->query_cha() - buyer->query_cha() + 1);
         }
      }
   }

   cost_adjustment += (buyer->query_cha() / 2.0);

   cost -= (cost * cost_adjustment) / 100.0;

   cost = ceil(selling_cost(cost));

   if (cost > this_body()->query_amt_currency(currency_type))
   {
      printf("Sorry, that costs %s, which you don't have!\n",
         MONEY_D->currency_to_string(cost, currency_type));

      return 0;
   }

   if (ob->move(buyer) != MOVE_OK)
   {
      write(capitalize(query_subjective()) + " can't seem to give " + ob->the_short() + " to you.\n");

      return 0;
   }

   money = MONEY_D->handle_subtract_money(buyer, cost, currency_type);

   buyer->my_action("$N $vbuy a $o for "
      + MONEY_D->currency_to_string(cost, currency_type)
      + (sizeof(money[1]) ? " and get "
      + MONEY_D->currency_to_string(money[1], currency_type) + " as change" : "")
      + evaluate_deal(cost, ob->query_value(), 1)
      + ".", ob, this_object());

   buyer->other_action("$N $vpurchase $o.\n", ob);

   reset_eval_cost();
   organize_stock();

   return 1;
}

private void sell_items_to(int key, int amount, object buyer)
{
   int i;
   class item item;
   object ob;

   item = stored_items[key];

   if (item->amount != -1 && item->amount < amount) { amount = item->amount; }

   for (i = 1; i <= amount; i++)
   {
      if (item->objects) { ob = item->objects[0]; }
      else { ob = new(item->file); }

      if (sell_object_to(ob, buyer))
      {
         if (item->objects) { item->objects -= ({ ob }); }

         if (item->amount != -1)
         {
            item->amount--;

            if (item->amount <= 0) { map_delete(stored_items, key); }
         }
      }

      else { return; }
   }
}

//:FUNCTION sell_stored_objects
//Gets called from the buy verb with the string from obj rule.
//This is the way the players can buy objects from the shopkeeper
//that he has stored away.
void sell_stored_objects_to(string item, int number, int amount, object buyer)
{
   if (!for_sale)
   {
      write("This shop doesn't sell anything.\n");

      return;
   }

   if (amount == 0) amount = 1;

   if (number)
   {
      if (!stored_items[number])
      {
         write("There is no item #" + number + " available.\n");

         return;
      }

      if (item == "all") { sell_items_to(number, amount, buyer); }
      else
      {
         if (member_array(item, stored_items[number]->ids) != -1) { sell_items_to(number, amount, buyer); }
         else { write("There is no '" + item + "' at #" + number + ".\n"); }
      }
   }
   else
   {
      foreach (int key in keys(stored_items))
      {
         if (member_array(item, stored_items[key]->ids) != -1)
         {
            sell_items_to(key, amount, buyer);

            return;
         }
      }

      write("There is no '" + item + "' in the list.\n");
   }
}

// for debugging only
mapping query_stored()
{
   return stored_items;
}

//:FUNCTION set_unique_inventory
//void set_unique_inventory(string str)
//This function determines if the vendor should hold onto what he
//buys instead of desting it and replacing it with an original.  For
//Example without unique set if you sell a sword to the vendor, no
//matter what the condition is, it will be dested and replaced with
//a new one.  Some muds would prefer the old way of what you sell is
//what you buy.  The unique inventory is set by sending the room
//where the inventory is kept.
//ex: set_unique_inventory("/domains/std/rooms/storage");
//NOTE: only armor, weaps, vehicles are uniqued
//Unless the object has a is_unique() { return 1; } function in it
//See set_all_unique to unique everything
void set_unique_inventory(string str)
{
   unique_inv = str;
#ifdef SELL_OLD_OBJECTS
   foreach (object ob in sort_array(all_inventory(load_object(str)), "sort_short")) { add_sell_object(ob); }
#endif
}

string query_unique_inventory()
{
   return unique_inv;
}

//:FUNCTION set_all_unique
//Sets ALL objects to be uniqued.  Only works when set_unique_inventory()
//is used.
void set_all_unique(int i)
{
   all_unique = i;
}

int query_all_unique()
{
   return all_unique;
}

int query_vendor()
{
   return (for_sale || will_buy);
}

//:FUNCTION check_uniqueness
//This fuction test if an object should be destroyed or saved when bought,
//depending on destroyable(), set_all_unique() and is_unique().
int check_uniqueness(object ob)
{
   /* If the object is supposed to be removed from the game upon selling */
   /* ie. quest objects */
//:FUNCTION destroyable
//place int destroyable() { return 1; } if you do not wish your object
//to be purchaseable from a vendor.  Players may sell it to receive its
//value, but it will then be destroyed.  Mostly used for quest objects
   if (ob->destroyable()) { return 2; }

   /* If we dont allow uniqueness at all, atuo return 0 */
   if (!unique_inv || unique_inv == "") { return 0; }

   /* If we want everything to be unique, return 1 */
   if (all_unique) { return 1; }

   /* If the object has been coded to be unique, return 1 */
//:FUNCTION is_unique
//put int is_unique() { return 1; } in your object if you wish it
//to be kept in a storeroom.  This is most likely because your
//object would have variable changes in it.  For example, weapons
//can have their wc reduced by wear and you'd rather someone not be
//able to purchase a brand new sword after selling a broken one.
   if (ob->is_unique()) { return 1; }

   if (ob->is_armor()) { return 1; }
   if (ob->is_weapon()) { return 1; }
   if (ob->is_vehicle()) { return 1; }

   return 0;
}

int sort_short(object first, object second)
{
   string first_short = first->short();
   string second_short = second->short();

   return strcmp(first_short, second_short);
}

void clear_stock()
{
   stored_items = ([ ]);
}

varargs string evaluate_deal(int deal_value, int real_value, int is_buying)
{
   if (deal_value > (real_value * 1.25))
   {
      return ", and feel like you got " + (is_buying ? "ripped off" : "a great deal");
   }
   else if (deal_value > (real_value * 1.1))
   {
      return ", and feel like you got a " + (is_buying ? "bad deal" : "good deal");
   }
   else if (deal_value < (real_value * 0.75))
   {
      return ", and feel like you got " + (is_buying ? "a great deal" : "ripped off");
   }
   else if (deal_value < (real_value * 0.9))
   {
      return ", and feel like you got a " + (is_buying ? "good deal" : "bad deal");
   }

   return "";
}

void organize_stock()
{
   if (!all_unique)
   {
      mapping files = ([ ]);

      foreach (class item item in values(stored_items))
      {
         if (!files[item->file]) { files[item->file] = 0; }

         files[item->file] += item->amount;
      }

      clear_stock();

      set_sell(files);

      if (strlen(unique_inv))
      {
         set_unique_inventory(unique_inv);
      }
   }
   else if (strlen(unique_inv))
   {
      clear_stock();

      set_unique_inventory(unique_inv);
   }
}