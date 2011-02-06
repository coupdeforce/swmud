inherit OBJ;
inherit M_DAMAGE_SOURCE;
inherit M_WIELDABLE;
inherit M_VALUABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;

void mudlib_setup()
{
   object::mudlib_setup();
   m_wieldable::mudlib_setup();
   add_id("weapon");
   add_save( ({ "persist_flags" }) );
}

mixed ob_state()
{
   return object::ob_state() + "/" + !!m_wieldable::ob_state();
}

void remove()
{
   object::remove();
   m_wieldable::unwield();
}

int indirect_kill_liv_with_obj()
{
   return 1;
}