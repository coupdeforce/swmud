// base_obj.c -- base for the object classes
// This is an object with the very most basic features.  It shouldn't be used directly.
// Use OBJ (and its subclasses) or use add_item() in the M_ITEMS module (typically in a ROOM or NON_ROOM).
// Created by Deathblade on 1996-01-21
inherit M_GRAMMAR;
inherit __DIR__ "object/names";
inherit __DIR__ "object/description";
inherit __DIR__ "object/flags";
inherit __DIR__ "object/non_object";
inherit __DIR__ "object/vsupport";
inherit __DIR__ "object/attributes";

void create()
{
   names::create();
   flags::create();
   description::set_object_creation_time();
}

//:FUNCTION stat_me
//return some debugging info about the state of the object
string stat_me()
{
   return "Short: " + short() + "\n"
      + "IDs: " + implode(parse_command_id_list(), ", ") + "\n"
      + "Plurals: " + implode(parse_command_plural_id_list(), ", ") + "\n"
      + "Adjectives: " + implode(parse_command_adjectiv_id_list(), ", ") + "\n"
      + "Long: \n" + long() + "\n";
}

// Add some very basic functionality.
// Heavier weight objects will most likely override these and supply "real" functionality.
int is_visible()
{
   return 1;
}

//### explain this comment.
// Here on purpose.
varargs mixed call_hooks(mixed s ...)
{
}

// need a set_light() for remove()
void set_light(int x)
{
}