// Last edited by deforce on 11-21-2007
inherit "/obj/admtool/internal/base.c";


nomask string module_name()
{
   return "skill";
}

nomask string module_key()
{
   return "S";
}

private nomask void list_skills()
{
   string * skills = sort_array(SKILL_D->query_skills(), 1);
   string *result;
   string skill, capname;
   int advrate, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for;
   string line;
   mixed tmp;

   if (sizeof(skills) == 0)
   {
      write("No skills are defined yet.\n");

      return;
   }

   //result = skills;
   line = "";
   result = ({ "Skill          Full Name               Adv   Str  Con  Agi  Dex  Per  Int  Cha  Luc  For\n" });

   foreach (skill in skills)
   {
      tmp = SKILL_D->query_skill(skill);
      capname = tmp[0];
      advrate = tmp[1];

      if (sizeof(tmp) > 2)
      {
         stat_str = tmp[2];
         stat_con = tmp[3];
         stat_agi = tmp[4];
         stat_dex = tmp[5];
         stat_per = tmp[6];
         stat_int = tmp[7];
         stat_cha = tmp[8];
         stat_luc = tmp[9];
         stat_for = tmp[10];
      }
      else
      {
         stat_str = 0;
         stat_con = 0;
         stat_agi = 0;
         stat_dex = 0;
         stat_per = 0;
         stat_int = 0;
         stat_cha = 0;
         stat_luc = 0;
         stat_for = 0;
      }

      line = sprintf("%-13s  %-21s:  %-4d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d  %-3d",
         skill[0..12], capname[0..18], advrate,
         stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for);

      result += ({ line });
   }

   more(result);
}

private nomask void rcv_add_skill(string skill_name)
{
   string * result;
   string name, capname;
   int req, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for;
   int ret;

   ret = sscanf(skill_name,"%s:%s:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d", name, capname, req,
      stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for);

   if (ret == 0)
   {
      name = skill_name;
      capname = skill_name;
      req = 1;
      stat_str = 0;
      stat_con = 0;
      stat_agi = 0;
      stat_dex = 0;
      stat_per = 0;
      stat_int = 0;
      stat_cha = 0;
      stat_luc = 0;
      stat_for = 0;
   }

   skill_name = lower_case(trim_spaces(skill_name));

   if (skill_name == "") { return; }

   result = SKILL_D->register_skill(name, ({capname, req, 
      stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for}) );

   if (!result) { write("ERROR: skill already exists.\n"); }
   else { printf("Added: %s.\n", skill_name); }
}

private nomask void rcv_remove_skill(string skill_name)
{
   string * result;

   skill_name = lower_case(trim_spaces(skill_name));

   result = SKILL_D->remove_skill(skill_name);

   if (!result) { write("ERROR: no such skill exists.\n"); }
   else { printf("Removed: %s.\n", skill_name); }
}

private nomask void rcv_view_skill(string skill_name)
{
   mixed result;

   result = SKILL_D->query_skill(skill_name);

   if (!result)
   {
      write("ERROR: no such skill.\n");

      return;
   }

   printf("      Internal name: %s\n   Capitalized name: %s\n   Advancement Rate: %d\n    Strength Weight: %d\nConstitution Weight: %d\n     Agility Weight: %d\n   Dexterity Weight: %d\n  Perception Weight: %d\nIntelligence Weight: %d\n    Charisma Weight: %d\n        Luck Weight: %d\n       Force Weight: %d\n",
      skill_name, result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10]);
}

private nomask void rcv_edit_skill(string params)
{
   string skill_name;
   string cap_name;
   int adv_rate, stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for;

   if (!params)
   {
      write("Syntax: e skill_name:capital_name:advancement_rate:str:con:agi:dex:per:int:cha:luc:for\n");

      return;
   }

   sscanf(params,"%s:%s:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",skill_name,cap_name,adv_rate,
      stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for);

   //write("We got " + skill_name + " " + cap_name + " " + adv_rate + ".\n");
   if ((!skill_name) || (!cap_name))
   {
      write("Syntax: e skill_name:capital_name:advancement_rate:str:con:agi:dex:per:int:cha:luc:for\n");

      return;
   }

   if (!adv_rate || (adv_rate > 10000))
   {
      write("Error: Advancement rate must be within 1 and 10000.\n");

      return;
   }

   SKILL_D->edit_skill(skill_name, cap_name, adv_rate,
      stat_str, stat_con, stat_agi, stat_dex, stat_per, stat_int, stat_cha, stat_luc, stat_for);

   printf("Skill %s successfully changed.\n", skill_name);
}

private nomask void list_skill_categories()
{
   more("Category List\n" + SKILL_D->list_skill_categories());
}

private nomask void list_uncategorized_skills()
{
   more(SKILL_D->list_uncategorized_skills());
}

private nomask void rcv_add_skill_to_category(string s)
{
   string cat, skl;

   if (sscanf(s,"%s:%s", cat, skl) != 2)
   {
      write("Format: A category:skill_name\n");

      return;
   }

   if (!SKILL_D->is_skill(skl))
   {
      write("Error! " + skl + " is not a valid skill.\n");

      return;
   }

   SKILL_D->add_skill_to_category(cat, skl);

   write("Added skill " + skl + " to category " + cat + ".\n");
}

private nomask void rcv_remove_skill_from_category(string s)
{
   string cat,skl;

   if (sscanf(s,"%s:%s", cat, skl) != 2)
   {
      write("Format: R category:skill_name\n");

      return;
   }

   if (!SKILL_D->is_skill(skl))
   {
      write("Error! " + skl + " is not a valid skill.\n");

      return;
   }

   SKILL_D->remove_skill_from_category(cat, skl);

   write("Removed skill " + skl + " from category " + cat + ".\n");
}

private nomask void rcv_remove_category(string s)
{
   string cat;

   if (sscanf(s,"%s", cat) != 1)
   {
      write("Format: D category\n");

      return;
   }

   if (member_array(cat, SKILL_D->query_skill_categories()) == -1)
   {
      write("Error! " + cat + " is not a valid category.\n");

      return;
   }

   SKILL_D->remove_category(cat);

   write("Removed category " + cat + ".\n");
}

nomask class command_info array module_commands()
{
   return ({
      new(class command_info,
         key : "l",
         desc : "list skills",
         action : (: list_skills :)),
      new(class command_info,
         key : "a", proto : "[name]",
         desc : "add a skill",
         args : ({ "New skill name? " }),
         action : (: rcv_add_skill :)),
      new(class command_info,
         key : "r",
         proto : "[name]",
         desc : "remove a skill",
         args : ({ "Remove which skill? " }),
         action : (: rcv_remove_skill :)),
      new(class command_info,
         key : "v",
         proto : "[name]",
         desc : "view a skill",
         args : ({ "View which skill? " }),
         action : (: rcv_view_skill :)),
      new(class command_info,
         key: "e",
         proto : "[name:CapName:adv_rate]",
         desc : "edit a skill",
         args : ({ "Edit which skill? " }),
         action : (: rcv_edit_skill :)),
      new(class command_info,
         key : "L",
         desc : "list skill categories",
         action: (: list_skill_categories :)),
      new(class command_info,
         key : "A",
         proto : "[category:skill]",
         desc : "add a skill to a category",
         args : ({ "Add which skill to which category?" }),
         action : (: rcv_add_skill_to_category :)),
      new(class command_info,
         key : "R",
         proto : "[category:skill]",
         desc : "remove a skill from a category",
         args : ({ "Remove which skill from which category?" }),
         action : (: rcv_remove_skill_from_category :)),
      new(class command_info,
         key : "D",
         proto : "[category]",
         desc : "remove a category",
         args : ({ "Remove which category?" }),
         action : (: rcv_remove_category :)),
      new(class command_info,
         key : "U",
         desc : "list uncategorized skills",
         action : (: list_uncategorized_skills :))
   });
}