inherit "/std/effect";

int array damage_percent = ({ 1, 2, 3, 5, 8, 13, 22, 36, 60, 100 });
int damage_count = 0;
int jedi_level = 1;
int skill_rank = 1;

void setup(int rank, int level)
{
   jedi_level = level;
   skill_rank = rank;
   damage_count = 0;

   set_effect_duration(to_int(floor(((level + rank) * 2) / 12)) * 8);
   set_effect_delay(8);
   set_effect_type("poison damage");
   set_effect_name("poison");
}

void do_effect(object ob)
{
   if (ob->has_buff("/d/buffs/force_plague") && (damage_count < sizeof(damage_percent)))
   {
      int damage = ((jedi_level + skill_rank) * 2) * damage_percent[damage_count++] / 100;

      if (damage < (skill_rank * 2)) { damage = (skill_rank * 2); }
      else if (damage < 1) { damage = 1; }

      ob->simple_action("$N $vreel in pain from being plagued.");
      ob->weaken_us(damage);
   }
   else
   {
      destruct(this_object());
   }
}