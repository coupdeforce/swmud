// Last edited by deforce on 05-30-2008
private string jedi_sponsor;
private int jedi_sponsor_alignment;
private int sponsor_date;
private mapping sponsored_by = ([ ]);
private mapping sponsored_alignment = ([ ]);
private mapping currently_sponsored = ([ ]);
private mapping sponsored = ([ ]);
private mapping unsponsored = ([ ]);

void save_me();

nomask string query_jedi_sponsor()
{
   return jedi_sponsor;
}

nomask int query_jedi_sponsor_alignment()
{
   return jedi_sponsor_alignment;
}

nomask int query_jedi_sponsored_when()
{
   return sponsor_date;
}

// This is done on the sponsored character
void set_jedi_sponsor(string new_sponsor, int alignment, int time)
{
/*
   if (this_body() && (this_body() != this_object()) && (wizardp(this_body()->query_userid()) < 3))
   {
      error("Invalid attempt to set " + this_object()->short() + "'s spouse to " + new_spouse + ", and married on " + ctime(time) + " at " + location + " by " + officiator + ".\n");
   }
*/

   jedi_sponsor = new_sponsor;
   jedi_sponsor_alignment = alignment;
   sponsor_date = time;

   save_me();
}

// This is done on the sponsored character
void remove_jedi_sponsor()
{
/*
   if (this_body() && (this_body() != this_object()) && (wizardp(this_body()->query_userid()) < 3))
   {
      error("Invalid attempt to divorce " + this_object()->short() + " from " + spouse + ".\n");
   }
*/

   // Save the current sponsor for the record
   if (strlen(jedi_sponsor))
   {
      sponsored_by[sponsor_date] = jedi_sponsor;
      sponsored_alignment[sponsor_date] = jedi_sponsor_alignment;
   }

   jedi_sponsor = "";
   jedi_sponsor_alignment = 0;
   sponsor_date = 0;

   save_me();
}

// This is done on the jedi sponsor
void unsponsor_jedi(string username, int time)
{
/*
   if (this_body() && (this_body() != this_object()) && (wizardp(this_body()->query_userid()) < 3))
   {
      error("Invalid attempt to divorce " + this_object()->short() + " from " + spouse + ".\n");
   }
*/

   // Save the current sponsor for the record
   if (strlen(username))
   {
      map_delete(currently_sponsored, username);
      unsponsored[time] = username;
   }

   save_me();
}

// This is done on the sponsored character
void reset_jedi_sponsors()
{
/*
   if (this_body() && (this_body() != this_object()) && (wizardp(this_body()->query_userid()) < 3))
   {
      error("Invalid attempt to reset " + this_object()->short() + "'s marriages.\n");
   }
*/

   sponsored_by = ([ ]);
   sponsored_alignment = ([ ]);

   save_me();
}

// This is done on the sponsored character
void remove_jedi_sponsorship(int when)
{
/*
   if (this_body() && (this_body() != this_object()) && (wizardp(this_body()->query_userid()) < 3))
   {
      error("Invalid attempt to remove " + this_object()->short() + "'s marriage from " + ctime(when) + ".\n");
   }
*/

   map_delete(sponsored_by, when);
   map_delete(sponsored_alignment, when);

   save_me();
}

int array query_previous_jedi_sponsor_dates()
{
   return keys(sponsored_by);
}

mapping query_previous_jedi_sponsors()
{
   return sponsored_by;
}

mapping query_previous_jedi_sponsor_alignments()
{
   return sponsored_alignment;
}

int array query_jedi_unsponsor_dates()
{
   return keys(unsponsored);
}

mapping query_jedi_unsponsored()
{
   return unsponsored;
}