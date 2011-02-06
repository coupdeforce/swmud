inherit ARMOR;
inherit ROBE_CUSTOM;
inherit HOOD;

void setup()
{
   set_id("Jedi Grand Master robe", "jedi", "grand", "master", "robe");
   set_long("A plain, light-brown robe made of machine-spun wool.  It has a hood, which can be worn up or down by raising or lowering it.");

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
      case 2: return (["for" : 3, "int" : 1]);
      case 3: return (["for" : 5, "int" : 3]);
      case 4..1000: return (["for" : 10, "int" : 5]);
   }

   if (environment()->query_guild_level("jedi") >= 20)
   {
      return (["for" : 1]);
   }

   return ([ ]);
}