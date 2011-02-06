// Last edited by deforce on 02-17-2010
inherit CONTAINER;
inherit M_GETTABLE;
inherit M_VALUABLE;
inherit M_WEARABLE;

int is_holster() { return 1; }

int has_weapons() { return sizeof(all_inventory(this_object())); }