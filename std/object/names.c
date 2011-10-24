// Grammar related stuff
private string array ids;
private nosave string array plurals;
private string array adjs;
private string primary_id, primary_adj;

private mixed internal_short; // Calculated internally
private nosave int unique; // Unique objects are referred to as 'the' instead of 'a'
private nosave int plural; // Plural objects (eg: stairs) are referred to as 'the' instead of 'a'
private nosave mixed proper_name; // Should only be set for objects who should not be refered to as "a xxx" or "the xxx"


// Can be implemented by subclasses to provide additional stuff dynamically
string array fake_item_id_list();
int is_visible();
string invis_name();
int test_flag(mixed);

varargs mixed call_hooks(string, mixed, mixed);
private void resync();

void create()
{
   parse_init();
   ids = ({});
   plurals = ({});
   adjs = ({});
   resync();
   this_object()->add_save(({ "ids", "plurals", "adjs", "primary_id", "primary_adj", "internal_short", "long", "persist_flags", "psets" }));

   if (!this_object()->is_living())
   {
      this_object()->add_save(({ "mass", "customize_record", "customize_times" }));
   }
}

//:FUNCTION set_proper_name
//Set the proper name of an object.  Objects with proper names never have adjectives added in front of their names.
void set_proper_name(string str)
{
   proper_name = str;
   resync();
}

//:FUNCTION set_unique
//Unique objects are always refered to as 'the ...' and never 'a ...'
void set_unique(int x)
{
   unique = x;
}

//:FUNCTION query_unique
//Return the value of 'unique'
int query_unique()
{
   return unique;
}

//:FUNCTION set_plural
//Plural objects are referred to as "the", not "a"
void set_plural(int x)
{
   plural = x;
}

//:FUNCTION query_plural
//Return the value of plural
int query_plural()
{
   return plural;
}

private void resync()
{
   if (!proper_name)
   {
      if (!primary_id && sizeof(ids))
      {
         primary_id = ids[0];
      }

      if (!primary_adj && sizeof(adjs))
      {
         primary_adj = adjs[0];
      }

      if (primary_id)
      {
         if (primary_adj)
         {
            internal_short = primary_adj + " " + primary_id;
         }
         else
         {
            internal_short = primary_id;
         }
      }
      else
      {
         internal_short = "nondescript thing";
      }
   }
   else
   {
      internal_short = proper_name;
   }

   parse_refresh();
}

mixed ob_state()
{
   return internal_short;
}

//:FUNCTION short
//Return the 'short' description of a object, which is the name by which
//it should be refered to
string short()
{
   if (!is_visible())
   {
      return invis_name();
   }

   return evaluate(internal_short);
}

// Used for multiple copies of the same object
string plural_short()
{
   if (!query_plural())
   {
      return pluralize(short());
   }

   else return short();
}

//:FUNCTION the_short
// Return the short descriptions, with the word 'the' in front if appropriate
string the_short()
{
   if (!is_visible())
   {
      return invis_name();
   }

   if (!proper_name)
   {
      return "the " + short();
   }

   return evaluate(proper_name);
}

//:FUNCTION a_short
// Return the short descriptions, with the word 'a' in front if appropriate
string a_short()
{
   if (!is_visible())
   {
      return invis_name();
   }

   if (plural || unique)
   {
      return the_short();
   }

   if (!proper_name)
   {
      return add_article(short());
   }

   return evaluate(proper_name);
}

/****** the id() functions ******/
int id(string arg)
{
   if (!arrayp(ids) || !sizeof(ids))
   {
      return 0;
   }

   return member_array(arg, ({ lower_case(ids[0]) }) + ids) != -1;
}

int plural_id(mixed arg)
{
   if (!arrayp(plurals))
   {
      return 0;
   }

   return member_array(arg, ({ lower_case(plurals[0]) }) + plurals) != -1;
}

// In the following, id handles _both_ id and plural, except in the _no_plural cases.
/****** add_ ******/

//:FUNCTION add_adj
//:Adds adjectives.  The first one _DOES NOT_ become the prim primary adjective when using add_adj().
void add_adj(string array adj... )
{
   if (!arrayp(adjs))
   {
      adjs = adj;
   }
   else
   {
      adjs += adj;
   }

   resync();
}

//:FUNCTION add_plural
//Add a plural id
void add_plural(string array plural...)
{
   if (!arrayp(plurals))
   {
      plurals = plural;
   }
   else
   {
      plurals += plural;
   }

   resync();
}

//:FUNCTION add_id_no_plural
//Add an id without adding the corresponding plural
void add_id_no_plural(string array id...)
{
   // set new primary
   if (!arrayp(ids))
   {
      ids = id;
   }
   else
   {
      ids += id;
   }

   resync();
}

//:FUNCTION add_id
//Add an id and its corresponding plural
void add_id(string array id...)
{
   if (!arrayp(ids))
   {
      ids = id;
   }
   else
   {
      ids += id;
   }

   plurals += map(id, (: pluralize :));
   resync();
}

/****** set_ ******/
//These actually add, but the first argument becomes the primary id/adjective
void set_id(string array id...)
{
   ids = id + ids; // Ensure proper order for resync of primary id
   plurals += map(id, (: pluralize :));
   primary_id = 0;
   resync();
}

void set_adj(string array adj...)
{
   if (!arrayp(adjs))
   {
      adjs = adj;
   }
   else
   {
      adjs = adj + adjs; // Ensure proper order for resync of primary adj
   }

   primary_adj = 0;
   resync();
}

/****** remove_ ******/
//:FUNCTION remove_id
//Remove the given id
void remove_id(string array id...)
{
   if (!arrayp(ids))
   {
      return;
   }

   ids -= id;
   plurals -= map(id, (: pluralize :));
   primary_id = 0;
   resync();
}

void remove_adj(string array adj...)
{
   if (!arrayp(ids))
   {
      return;
   }

   adjs -= adj;
   primary_adj = 0;
   resync();
}

/****** clear_ ******/

//:FUNCTION clear_id
//removes all the ids of an object.
void clear_id()
{
   ids = ({ });
   plurals = ({ });
   primary_id = 0;
   resync();
}

//:FUNCTION clear_adj
//Remove all the adjectives from an object
void clear_adj()
{
   adjs = ({ });
   primary_adj = 0;
   resync();
}

/****** query_ ******/

//:FUNCTION query_id
//Returns an array containing the ids of an object
string array query_id()
{
   string array fake = this_object()->fake_item_id_list();

   if (fake)
   {
      return fake + ids;
   }

   return ids;
}

//:FUNCTION query_primary_id
//Returns the primary id of an object
string query_primary_id()
{
   return primary_id;
}

//:FUNCTION query_primary_adj
//Returns the primary adj of an object
string query_primary_adj()
{
   return primary_adj;
}

//:FUNCTION query_primary_name
//Returns the primary name (primary adj + primary id) of an object
string query_primary_name()
{
   return (primary_adj ? primary_adj + " " : "") + primary_id;
}

//:FUNCTION query_adj
//return the adjectives
string array query_adj()
{
   return adjs;
}

/****** parser interaction ******/

string array parse_command_id_list()
{
   if (test_flag(INVIS))
   {
      return ({ });
   }

   //### should strip non-alphanumerics here; might need an efun to do it efficiently
   return query_id();
}

string array parse_command_plural_id_list()
{
   if (test_flag(INVIS))
   {
      return ({ });
   }

   return plurals;
}

string array parse_command_adjectiv_id_list()
{
   if (test_flag(INVIS))
   {
      return ({ });
   }

   return adjs;
}