// Last edited by deforce on 03-22-2010
int query_guild_level(string guild);

int team_alignment = 0;
int jedi_alignment = 0;

//:FUNCTION query_team_alignment
//Returns numeric team alignment
int query_team_alignment()
{
   return team_alignment;
}

//:FUNCTION query_team
//Returns 1 for rebel, 0 for neutral, -1 for imperial
int query_team()
{
   return (team_alignment > 25 ? 1 : (team_alignment < -25 ? -1 : 0));
}

//:FUNCTION query_jedi_alignment
//Returns numeric jedi alignment
int query_jedi_alignment()
{
   if (!query_guild_level("jedi"))
   {
      return 0;
   }

   return jedi_alignment;
}

//:FUNCTION query_max_jedi_alignment
//Returns numeric maximum jedi alignment
int query_max_jedi_alignment()
{
   int jedi_level = query_guild_level("jedi");
   int max_alignment;

   if (!jedi_level) { return 0; }

   max_alignment = 25 + floor(jedi_level / 2);

   if (jedi_alignment < 0) { max_alignment *= -1; }

   return max_alignment;
}

//:FUNCTION set_team_alignment
//Value is from -100 to 100
void set_team_alignment(int value)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s team alignment from " + team_alignment + " to " + value, previous_object(-1)); }

   if ((value >= -100) && (value <= 100))
   {
      team_alignment = value;
   }
}

//:FUNCTION set_jedi_alignment
//Value is from -50 to 50
void set_jedi_alignment(int value)
{
   int jedi_level = query_guild_level("jedi");
   int max_alignment;

   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s team alignment from " + jedi_alignment + " to " + value, previous_object(-1)); }

   if (!jedi_level) { return; }

   max_alignment = 25 + floor(jedi_level / 2);

   if ((value >= (max_alignment * -1)) && (value <= max_alignment))
   {
      jedi_alignment = value;
   }
   else
   {
      if (value < 0)
      {
         jedi_alignment = max_alignment * -1;
      }
      else if (value > 0)
      {
         jedi_alignment = max_alignment;
      }
   }
}

//:FUNCTION adjust_team_alignment
//Value is positive or negative integer
void adjust_team_alignment(int value)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("adjust " + this_object()->short() + "'s team alignment from " + team_alignment + " to " + value, previous_object(-1)); }

   if ((team_alignment < -25) && ((team_alignment + value) > -26))
   {
      team_alignment = 0;

      // Make Imperials hate them
   }
   else if ((team_alignment > 25) && ((team_alignment + value) < 26))
   {
      team_alignment = 0;

      // Make Rebels hate them
   }
   else if ((team_alignment < 26) && (team_alignment > -26))
   {
      if ((team_alignment + value) < -25)
      {
         team_alignment = -100;
      }
      else if ((team_alignment + value) > 25)
      {
         team_alignment = 100;
      }
   }

   team_alignment += value;

   if (team_alignment < -100) { team_alignment = -100; }
   else if (team_alignment > 100) { team_alignment = 100; }
}

//:FUNCTION adjust_jedi_alignment
//Value is positive or negative integer
void adjust_jedi_alignment(int value)
{
   int jedi_level = query_guild_level("jedi");
   int max_alignment;

   if (this_object()->is_body()) { this_object()->check_wizard_set("adjust " + this_object()->short() + "'s jedi alignment from " + jedi_alignment + " to " + value, previous_object(-1)); }

   if ((team_alignment < -25) && ((team_alignment + value) > -26))
   if (!jedi_level) { return; }

   max_alignment = 25 + floor(jedi_level / 2);

   if ((jedi_alignment < 0) && ((jedi_alignment + value) > -1))
   {
      jedi_alignment = max_alignment;
   }
   else if ((jedi_alignment > 0) && ((jedi_alignment + value) < 1))
   {
      jedi_alignment = max_alignment * -1;
   }

   jedi_alignment += value;

   if ((jedi_alignment < 0) && (jedi_alignment < (max_alignment * -1)))
   {
      jedi_alignment = max_alignment * -1;
   }
   else if ((jedi_alignment > 0) && (jedi_alignment > max_alignment))
   {
      jedi_alignment = max_alignment;
   }
}