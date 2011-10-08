inherit OBJ;
inherit M_WEARABLE;
inherit M_GETTABLE;
inherit M_DAMAGE_SINK;
inherit M_VALUABLE;

//:MODULE
// This is the base for creating a piece of armor.  It uses M_WEARABLE to
// allow it to be worn, and M_DAMAGE_SINK to allow it to absorb damage.

void mudlib_setup()
{
   object::mudlib_setup();
   m_damage_sink::mudlib_setup();
   m_valuable::mudlib_setup();
   m_wearable::mudlib_setup();
//   add_id("armor", "armour");
   add_save( ({ "persist_flags" }) );
}

//:FUNCTION set_worn
// set_worn(1) causes this object to be worn by whatever is holding it.
// set_worn(0) takes it back off again.
void set_worn(int attempt_wear)
{
   if (attempt_wear)
   {
      environment()->add_armor(this_object());
      ::set_worn(1);
   }
   else
   {
      environment()->remove_armor(this_object());
      ::set_worn(0);
   }
}

void remove()
{
   m_wearable::remove();
   object::remove();
}

mixed ob_state()
{
   return m_wearable::ob_state();
}