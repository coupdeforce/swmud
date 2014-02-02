inherit CMD;

void list_unsponsored_history();

void main(string player_name)
{
   if (this_body()->query_guild_rank("jedi") >= 2)
   {
      out("You have become proficient enough in the ways of the Force that you no longer need a sponsor.\n");
   }
   else
   {
      string sponsor_name = JEDI_SPONSOR_D->query_sponsor(this_body()->query_userid());

      if (strlen(sponsor_name))
      {
         mapping sponsor_data = JEDI_SPONSOR_D->query_sponsored(sponsor_name);
         string elapsed_time = convert_time((time() - sponsor_data[this_body()->query_userid()]) / 86400 * 86400);

         if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsor_data[this_body()->query_userid()]) / 3600 * 3600); }
         if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsor_data[this_body()->query_userid()]) / 60 * 60); }
         if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - sponsor_data[this_body()->query_userid()])); }

         outf("You have been sponsored by %s since %s (%s ago).\n", capitalize(sponsor_name), ctime(sponsor_data[this_body()->query_userid()]), elapsed_time);
      }
      else
      {
         out("You do not currently have a Jedi sponsor.\n");
      }
   }

   list_unsponsored_history();
}

void list_unsponsored_history()
{
   string array unsponsors = JEDI_SPONSOR_D->query_unsponsors(this_body()->query_userid());

   if (sizeof(unsponsors))
   {
      foreach (string unsponsor_name in unsponsors)
      {
         mapping unsponsor_data = JEDI_SPONSOR_D->query_unsponsored(unsponsor_name);
         int unsponsor_time = unsponsor_data[this_body()->query_userid()];

         if (unsponsor_time)
         {
            string elapsed_time = convert_time((time() - unsponsor_time) / 86400 * 86400);

            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsor_time) / 3600 * 3600); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsor_time) / 60 * 60); }
            if (!strlen(elapsed_time)) { elapsed_time = convert_time((time() - unsponsor_time)); }

            outf("You were unsponsored by %s on %s (%s ago).\n", capitalize(unsponsor_name), ctime(unsponsor_time), elapsed_time);
         }
      }
   }
}