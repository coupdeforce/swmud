inherit CLASS_EVENT_INFO;

int is_limb(string);
varargs int query_max_health(string);

class slot
{
   string name;
   int non_armor;
   object item;
}

private mapping body_slots = ([
   "torso" : new(class slot, name: "torso", non_armor: 0, item: 0),
   "head" : new(class slot, name: "head", non_armor: 0, item: 0),
   "left arm" : new(class slot, name: "left arm", non_armor: 0, item: 0),
   "right arm" : new(class slot, name: "right arm", non_armor: 0, item: 0),
   "left leg" : new(class slot, name: "left leg", non_armor: 0, item: 0),
   "right leg" : new(class slot, name: "right leg", non_armor: 0, item: 0),
   "left hand" : new(class slot, name: "left hand", non_armor: 0, item: 0),
   "right hand" : new(class slot, name: "right hand", non_armor: 0, item: 0),
   "left foot" : new(class slot, name: "left foot", non_armor: 0, item: 0),
   "right foot" : new(class slot, name: "right foot", non_armor: 0, item: 0),
   "neck" : new(class slot, name : "neck", non_armor : 1, item: 0),
   "waist" : new(class slot, name : "waist", non_armor : 1, item: 0),
   "back" : new(class slot, name : "back", non_armor : 1, item: 0),
   "chest" : new(class slot, name : "chest", non_armor : 1, item: 0),
]);

protected nomask void set_body_slots(mapping what)
{
   body_slots = what;
}

#define BODY_SLOT(x) ((class slot)body_slots[x])

//:FUNCTION wear_item
// nomask int wear_item(object what, string sname);
// Wear object 'what' on slot 'sname'.
nomask int wear_item(object what)
{
   string array slot_names = what->query_slots();
   int body_size = this_object()->query_body_size();
   int size = what->get_body_size();

   foreach (string slot in slot_names)
   {
      if (!body_slots[slot]) { return 0; }
      if (BODY_SLOT(slot)->item) { return 0; }
   }

   if ((((size + 1) - body_size) > 1) && ((body_size - (size - 1)) > 1)) { return 0; }

   foreach (string slot in slot_names)
   {
      BODY_SLOT(slot)->item = what;
   }

   return 1;
}

nomask int remove_item(object what)
{
   string array slot_names = what->query_slots();

   foreach (string slot in slot_names)
   {
      if (!body_slots[slot]) { return 0; }
   }

   foreach (string slot in slot_names)
   {
      BODY_SLOT(slot)->item = 0;
   }

   return 1;
}

nomask string query_random_armor_slot()
{
   string tmp;

   if (!sizeof(body_slots)) { return 0; }

   tmp = choice(filter_array(keys(body_slots), (: !BODY_SLOT($1)->non_armor :)));

   if (!tmp) { return 0; }

   return tmp;
}

nomask string query_random_clothing_slot()
{
   string tmp;

   if (!sizeof(body_slots)) { return 0; }

   tmp = choice(filter_array(keys(body_slots), (: BODY_SLOT($1)->non_armor :)));

   if (!tmp) { return 0; }

   return tmp;
}

nomask void reset_body_slots()
{
   body_slots = ([
      "torso" : new(class slot, name: "torso", non_armor: 0, item: 0),
      "head" : new(class slot, name: "head", non_armor: 0, item: 0),
      "left arm" : new(class slot, name: "left arm", non_armor: 0, item: 0),
      "right arm" : new(class slot, name: "right arm", non_armor: 0, item: 0),
      "left leg" : new(class slot, name: "left leg", non_armor: 0, item: 0),
      "right leg" : new(class slot, name: "right leg", non_armor: 0, item: 0),
      "left hand" : new(class slot, name: "left hand", non_armor: 0, item: 0),
      "right hand" : new(class slot, name: "right hand", non_armor: 0, item: 0),
      "left foot" : new(class slot, name: "left foot", non_armor: 0, item: 0),
      "right foot" : new(class slot, name: "right foot", non_armor: 0, item: 0),
      "neck" : new(class slot, name : "neck", non_armor : 1, item: 0),
      "waist" : new(class slot, name : "waist", non_armor : 1, item: 0),
      "back" : new(class slot, name : "back", non_armor : 1, item: 0),
      "chest" : new(class slot, name : "chest", non_armor : 1, item: 0),
   ]);
}

//:FUNCTION has_body_slot
// nomask int has_body_slot(string what);
// Returns 1 if the adversary has a 'what' body slot.
nomask int has_body_slot(string what)
{
   return !!body_slots[what];
}

//:FUNCTION query_armor_slot
// object array query_armor_slot(string slot);
// Returns the armor that is covering limb 'slot'.
object query_armor_slot(string slot)
{
   object armor;

   if (!is_limb(slot)) { return 0; }

   if (query_max_health(slot) < 1) { return 0; }

   armor = BODY_SLOT(slot)->item;

   if (!armor) { return this_object(); }

   return armor;
}

object event_get_armor(class event_info evt)
{
   return query_armor_slot(evt->target_extra);
}

//:FUNCTION query_armor_slots
// string array query_armor_slots()
// Returns a list of all armor slots on the adversary.
string array query_armor_slots()
{
   return filter_array(keys(body_slots), (: !BODY_SLOT($1)->non_armor :));
}

//:FUNCTION query_clothing_slots
// string array query_clothing_slots()
// Returns a list of all clothing slots on the adversary.
string array query_clothing_slots()
{
   return filter_array(keys(body_slots), (: BODY_SLOT($1)->non_armor :));
}