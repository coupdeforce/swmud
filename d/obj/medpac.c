// Last edited by deforce on 03-03-2010
inherit OBJ;
inherit M_GETTABLE;
inherit M_VALUABLE;

void setup()
{
   set_id("medpac");
   set_long("A small package of medical supplies containing a bacta vial and bandages.");
   set_mass(1);
   set_value(100);
}

int is_surgery_medpac() { return 1; }