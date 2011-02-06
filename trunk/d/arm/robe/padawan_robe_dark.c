inherit ARMOR;
inherit ROBE_CUSTOM;
inherit HOOD;

void setup()
{
   set_id("padawan robe", "padawan", "robe");
   set_adj("dark");
   set_long("A plain, dark-brown robe made of machine-spun wool.  It has a hood, which can be worn up or down by raising or lowering it.");

   set_slots("torso", "left arm", "right arm", "left leg", "right leg");

   set_armor_class(1);

   set_mass(3000);
   set_value(300);
}

int is_jedi_armor() { return 1; }