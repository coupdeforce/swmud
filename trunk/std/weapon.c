inherit OBJ;
inherit M_DAMAGE_SOURCE;
inherit M_WIELDABLE;
inherit M_VALUABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;

int cortosis_weave = 0;

void mudlib_setup()
{
   object::mudlib_setup();
   m_wieldable::mudlib_setup();
   add_id("weapon");
   add_save( ({ "cortosis_weave", "persist_flags" }) );
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

void set_cortosis_weave(int value)
{
   cortosis_weave = value;
}

int has_cortosis_weave()
{
   if (cortosis_weave == 2)
   {
      return random(2);
   }

   return cortosis_weave;
}

int indirect_kill_liv_with_obj()
{
   return 1;
}