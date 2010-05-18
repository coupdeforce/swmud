// Last edited by deforce on 05-06-2010
inherit OBJ;
inherit M_DAMAGE_SOURCE;

void setup(string name, string id)
{
   if (strlen(name) && strlen(id))
   {
      set_id(name, id, "inject_damage");
   }
   else
   {
      set_id("inject_damage");
   }

   set_combat_messages("combat-inject");
   set_chance_to_be_damaged(0);
   set_attached(1);
   hook_state("prevent_drop", "", 1);
}

int is_weapon() { return 0; }
mixed direct_wield_obj() { return 0; }
mixed direct_wield_obj_in_str(object ob, string limb) { return 0; }
mixed direct_unwield_obj() { return 0; }
int valid_wield() { return 0; }
int valid_unwield() { return 0; }

class event_info source_modify_event(class event_info evt)
{
   return evt;
}