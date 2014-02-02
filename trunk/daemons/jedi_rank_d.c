inherit M_DAEMON_DATA;

string dark_lord_name = "";
string array jedi_grand_masters = ({ });

int is_highest_rank(string name, int alignment)
{
   if (alignment < 0)
   {
      if (name == dark_lord_name)
      {
         return 1;
      }
   }
   else if (alignment >= 0)
   {
      if (member_array(name, jedi_grand_masters) > -1)
      {
         return 1;
      }
   }

   return 0;
}

void set_dark_lord(string name)
{
   dark_lord_name = name;
}

void clear_dark_lord()
{
   string force_message = "%^BOLD%^%^RED%^You sense the Light Side of the Force grow stronger, as the previous Dark Lord of the Sith fades into obscurity.%^RESET%^\n";
   dark_lord_name = "";

   foreach (object jedi in filter_array(bodies(), (: $1->query_toggle("sense disturbance") == "on" :) ))
   {
      tell(jedi, force_message);
   }
}

string query_dark_lord()
{
   return dark_lord_name;
}

int dark_lord_exists()
{
   return strlen(dark_lord_name) ? 1 : 0;
}

void add_jedi_grand_master(string name)
{
   if (member_array(name, jedi_grand_masters) == -1)
   {
      jedi_grand_masters += ({ name });
   }
}

void remove_jedi_grand_master(string name)
{
   if (member_array(name, jedi_grand_masters) > -1)
   {
      string force_message = "%^BOLD%^%^RED%^You sense the Dark Side of the Force grow stronger, as a Jedi Grand Master fades into obscurity.%^RESET%^\n";
      jedi_grand_masters -= ({ name });

      foreach (object jedi in filter_array(bodies(), (: $1->query_toggle("sense disturbance") == "on" :) ))
      {
         tell(jedi, force_message);
      }
   }
}

string array query_jedi_grand_masters()
{
   return jedi_grand_masters;
}

int is_jedi_grand_master(string name)
{
   if (member_array(name, jedi_grand_masters) > -1)
   {
      return 1;
   }
}

void clean_up()
{
   if (strlen(dark_lord_name))
   {
      mixed array last = LAST_LOGIN_D->query_last(dark_lord_name);

      if (sizeof(last))
      {
         if (last[0] < (time() - 1209600))
         {
            clear_dark_lord();
         }
      }
      else
      {
         clear_dark_lord();
      }
   }

   if (sizeof(jedi_grand_masters))
   {
      foreach (string name in jedi_grand_masters)
      {
         mixed array last = LAST_LOGIN_D->query_last(name);

         if (sizeof(last))
         {
            if (last[0] < (time() - 5184000))
            {
               remove_jedi_grand_master(name);
            }
         }
         else
         {
            remove_jedi_grand_master(name);
         }
      }
   }

   call_out( (: clean_up :), 7200);
}

void create()
{
   ::create();

   clean_up();
}