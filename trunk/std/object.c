// object.c  about 1/2 Rust and 1/2 Nevyn
// Modifications:  Peregrin@ZorkMUD (GUE),  Mar '94
// Beek - rewrote the DISCARDED_MESSAGE stuff b/c it was hideously inefficient.
// Beek --- Added the OFFICIAL "what these vars should contain" documentation
// Sep 9 1994 - Rust moved everything into modules.

#include <flags.h>
#include <clean_up.h>

inherit BASE_OBJ;
inherit M_SAVE;

inherit __DIR__ "object/mass";
inherit __DIR__ "object/light";		// before non_object
inherit __DIR__ "object/properties";
inherit __DIR__ "object/move";
inherit __DIR__ "object/visible";
inherit __DIR__ "object/hooks";
inherit __DIR__ "object/msg_recipient";

int test_unique();

//:FUNCTION stat_me
//return some debugging info about the state of the object
string stat_me() 
{
   string result = ::stat_me() + "Short: " + short() + "\n";
   result += "Weight: " + query_mass() + "  Light: " + query_light() + "\n";

   return result;
}

//:FUNCTION setup
// This function is overloaded by area implementors.  Nothing in
// the mudlib proper should override this.  Further, nothing should
// ever go into this function.  This allows an area implementor to
// simply respond to setup() and not worry about inheriting the
// function call.
void setup(mixed array args...)
{
   // Overload me!
}

//:FUNCTION mudlib_setup
// This function is overloaded by all mudlib objects deriving from
// this class.  They should inherit as necessary.  The intention of
// overriding this instead of create() to is ensure that the mudlib
// initialization completes _before_ the call to the area coder's
// setup() function.  Specifically, the mudlib objects need to
// initialize defaults that will then be changed by the setup() call.
// Note that if the mudlib object overrode create(), then its init
// code would occur _after_ the area coder's setup() and possibly
// blow away some of their settings.
void mudlib_setup(mixed array args...)
{
   // Overload me!
}

void create(mixed array args...)
{
   base_obj::create();
   configure_set(STD_FLAGS, 0, (: resync_visibility :));

   if (clonep(this_object()))
   {
      mudlib_setup(args...);

      // Use a call_other to avoid a redeclaration warning, since
      // mostly modules that aren't directly inheriting us will define
      // this function.
      this_object()->internal_setup(args...);

      setup(args...);
   }
}


// arbitrate some stuff that was stubbed in BASE_OBJ
varargs mixed  call_hooks( array args...)
{
   return hooks::call_hooks(args...);
}

int is_visible()
{
   return visible::is_visible();
}

void set_light(int x)
{
   light::set_light(x);
}

//### defeats the purpose, doesn't it?  I think this should default to 
//### failing.  That was what was intended; otherwise it would be named
//### disallow() ...
int allow(string what)
{
   return 1;
}

// by default, if we appear to be useless, we are!
int destruct_if_useless()
{
   destruct(this_object());
}

// Be *very* careful about calling functions in other objects from here,
// since that may cause that object to not clean_up() [since this function
// is called in objects that haven't had a function call in a while].
//
// e.g. environment()->anything, like many libs do, is remarkably stupid.
int clean_up(int instances)
{
   // If we have an environment, we will be destructed when our environment
   // cleans up.  So no need to worry about it ourself.  Note that once
   // we have an environment, we can never lose it, so the driver need not
   // worry about us any more.
   if (environment()) { return NEVER_AGAIN; }

   // if we are inherited, or have clones around, we don't want to clean up
   // as that would cause this program to need to be recompiled later.
   // (note: instances is only ever nonzero for blueprints)
   // This may change later, though.
   // Instances are always coing to be at least one because the blueprint is 
   // counted.  -- Tigran
   if (instances > 1) { return ASK_AGAIN; }

   // We don't have an environment.  We appear to be useless!
   return destruct_if_useless();
}

void on_clone( mixed array args... )
{
}

string query_unique_id()
{
   string output = "";

   if (!sscanf(file_name(this_object()), "%*s#%s", output)) { output = "unknown"; }

   return output;
}

int query_durability() { return 1; }
int query_max_durability() { return 1; }

void do_check_obj()
{
   write("You check " + this_object()->the_short() + " and you detect nothing unusual.\n");
}