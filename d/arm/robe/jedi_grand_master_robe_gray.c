inherit ARMOR;
inherit ROBE_CUSTOM;
inherit HOOD;

void setup()
{
   set_id("Jedi Grand Master robe", "jedi", "grand", "master", "robe");
   set_adj("gray");
   set_long("A plain, medium-brown robe made of machine-spun wool.  It has a hood, which can be worn up or down by raising or lowering it.");

   set_slots("torso", "left arm", "right arm", "left leg", "right leg");

   set_armor_class(4);

   set_mass(3000);
   set_value(11200);
}

int is_jedi_armor() { return 1; }

mapping query_attribute_modifiers()
{
   switch (environment()->query_guild_rank("jedi"))
   {
      case 2: return (["int" : 3, "cha" : 3]);
      case 3: return (["int" : 5, "cha" : 5]);
      case 4..1000: return (["int" : 10, "cha" : 10]);
   }

   if (environment()->query_guild_level("jedi") >= 20)
   {
      return (["int" : 1, "cha" : 1]);
   }

   return ([ ]);
}