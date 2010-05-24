// Last edited by deforce on 04-23-2010
inherit CMD;

private void main()
{
   object body = this_body();
   string array guild_names = sort_array(keys(body->query_guild_specializations()), 1);
   int count;

   if (!sizeof(guild_names))
   {
      out("You haven't specialized in any guilds.\n");

      return;
   }

   outf("\n\n %-13s   %-20s   Rank\n" + repeat_string("=", 45) + "\n", "Guild", "Specialization");

   for (count = 0; count < sizeof(guild_names); count++)
   {
      string specialization = body->query_guild_specialization(guild_names[count]);
      int rank = body->query_guild_specialization_rank(guild_names[count], specialization);

      outf("  %13s   %-20s   %2i\n", title_capitalize(guild_names[count]), title_capitalize(specialization), rank);
   }
}