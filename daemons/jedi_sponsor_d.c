inherit CLASS_PLAYER_GUILD_DATA;
inherit M_DAEMON_DATA;

mapping sponsored_sponsor = ([ ]); // Sponsored userid : sponsor userid
mapping sponsored_alignment = ([ ]); // Sponsored userid : sponsor alignment
mapping sponsored_time = ([ ]); // Sponsored userid : unix time when sponsorship was given
mapping unsponsored_sponsor = ([ ]); // Sponsored userid : ({ sponsor userid })
mapping unsponsored_time = ([ ]); // Sponsored userid : ({ unix time when sponsorship was revoked })
mapping sponsor_alignment = ([ ]); // Sponsor userid : alignment
mapping sponsor_rank = ([ ]); // Sponsor userid : rank

int count_sponsored(string sponsor);

void add_sponsored(string sponsored, string sponsor, int alignment)
{
   if (strlen(sponsored) && strlen(sponsor) && !sponsored_sponsor[sponsored])
   {
      sponsored_sponsor[sponsored] = sponsor;
      sponsored_alignment[sponsored] = alignment;
      sponsored_time[sponsored] = time();

      save_me();
   }
}

void remove_sponsored(string sponsored)
{
   if (strlen(sponsored) && strlen(sponsored_sponsor[sponsored]))
   {
      map_delete(sponsored_sponsor, sponsored);
      map_delete(sponsored_alignment, sponsored);
      map_delete(sponsored_time, sponsored);

      save_me();
   }
}

void add_unsponsored(string sponsored, string sponsor)
{
   if (strlen(sponsored) && strlen(sponsor) && (sponsored_sponsor[sponsored] == sponsor))
   {
      string array sponsors = unsponsored_sponsor[sponsored];

      remove_sponsored(sponsored);

      if (sizeof(unsponsored_sponsor[sponsored]))
      {
         int index = member_array(sponsor, unsponsored_sponsor[sponsored]);

         if (index > -1)
         {
            unsponsored_time[sponsored][index] = time();
         }
         else
         {
            unsponsored_sponsor[sponsored] += ({ sponsor });
            unsponsored_time[sponsored] += ({ time() });
         }
      }
      else
      {
         unsponsored_sponsor[sponsored] = ({ sponsor });
         unsponsored_time[sponsored] = ({ time() });
      }

      save_me();
   }
}

void remove_unsponsored(string sponsored, string sponsor)
{
   if (strlen(sponsored) && strlen(sponsored) && sizeof(unsponsored_sponsor[sponsored]))
   {
      string array sponsors = unsponsored_sponsor[sponsored];
      int array times = unsponsored_time[sponsored];
      int index = member_array(sponsor, sponsors);

      if (index > -1)
      {
         unsponsored_sponsor[sponsored] -= ({ sponsor });
         unsponsored_time[sponsored] -= ({ times[index] });
      }

      save_me();
   }
}

void clear_unsponsored(string sponsored)
{
   if (strlen(sponsored) && sizeof(unsponsored_sponsor[sponsored]))
   {
      map_delete(unsponsored_sponsor, sponsored);
      map_delete(unsponsored_time, sponsored);

      save_me();
   }
}

void update_sponsor_data(string sponsor, int alignment, int rank)
{
   if (strlen(sponsor))
   {
      if (((alignment == 1) || (alignment == -1)) && ((rank >= 2) && (rank <= 4)))
      {
         sponsor_alignment[sponsor] = alignment;
         sponsor_rank[sponsor] = rank;
      }

      save_me();
   }
}

void remove_sponsor_data(string sponsor)
{
   if (strlen(sponsor))
   {
      map_delete(sponsor_alignment, sponsor);

      save_me();
   }
}

string query_sponsor(string sponsored)
{
   if (strlen(sponsored) && sponsored_sponsor[sponsored])
   {
      return sponsored_sponsor[sponsored];
   }

   return "";
}

string array query_unsponsors(string sponsored)
{
   if (sizeof(unsponsored_sponsor[sponsored]))
   {
      return unsponsored_sponsor[sponsored];
   }

   return ({ });
}

mapping query_sponsored(string sponsor)
{
   mapping sponsored = ([ ]);
   string array all_sponsored = keys(sponsored_sponsor);
   string array all_sponsors = values(sponsored_sponsor);
   int last_index = member_array(sponsor, all_sponsors, 0);

   while (last_index > -1)
   {
      sponsored[all_sponsored[last_index]] = sponsored_time[all_sponsored[last_index]];

      last_index = member_array(sponsor, all_sponsors, last_index + 1);
   }

   return sponsored;
}

mapping query_unsponsored(string sponsor)
{
   mapping unsponsored = ([ ]);
   string array all_unsponsored = keys(unsponsored_sponsor);

   foreach (string name in all_unsponsored)
   {
      if (sizeof(unsponsored_sponsor[name]) && (sizeof(unsponsored_sponsor[name]) == sizeof(unsponsored_time[name])))
      {
         int index = member_array(sponsor, unsponsored_sponsor[name]);

         if (index > -1)
         {
            unsponsored[name] = unsponsored_time[name][index];
         }
      }
   }

   return unsponsored;
}

mapping query_sponsor_data(int alignment)
{
   mapping data = ([ ]);

   foreach (string sponsor in keys(sponsor_alignment))
   {
      if ((alignment == 0) || (sponsor_alignment[sponsor] == alignment))
      {
         data[sponsor] = ({ sponsor_alignment[sponsor], count_sponsored(sponsor), sponsor_rank[sponsor] });
      }
   }

   return data;
}

int count_sponsored(string sponsor)
{
   int count = 0;

   if (strlen(sponsor))
   {
      foreach (string sponsored in keys(sponsored_sponsor))
      {
         if (sponsored_sponsor[sponsored] == sponsor)
         {
            count++;
         }
      }
   }

   return count;
}

mapping get_sponsored_sponsor()
{
   return sponsored_sponsor;
}

mapping get_sponsored_alignment()
{
   return sponsored_alignment;
}

mapping get_sponsored_time()
{
   return sponsored_time;
}

mapping get_unsponsored_sponsor()
{
   return unsponsored_sponsor;
}

mapping get_unsponsored_time()
{
   return unsponsored_time;
}

mapping get_sponsor_data_alignment()
{
   return sponsor_alignment;
}

mapping get_sponsor_data_rank()
{
   return sponsor_rank;
}

void clean_up()
{
   foreach (string name in keys(sponsor_alignment))
   {
      class player_guild_data result = PLAYER_DATA_D->get_player_guild_data(name);

      if (result)
      {
         if ((result->guild_levels["jedi"] < 20) || (result->guild_ranks["jedi"] < 2))
         {
            remove_sponsor_data(name);
         }
         else
         {
            int alignment = result->jedi_alignment;
            int rank = result->guild_ranks["jedi"];

            if (alignment > 0)
            {
               alignment = 1;
            }
            else if (alignment < 0)
            {
               alignment = -1;
            }

            if (rank > 4)
            {
               rank = 4;
            }

            update_sponsor_data(name, alignment, rank);
         }
      }
      else
      {
         remove_sponsor_data(name);
      }
   }

   call_out( (: clean_up :), 86400);
}

void create()
{
   ::create();

   clean_up();
}