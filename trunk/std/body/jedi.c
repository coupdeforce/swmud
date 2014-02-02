private mapping sponsored_start_time = ([ ]); // sponsored : ({ start_time })
private mapping sponsored_complete_time = ([ ]); // sponsored: ({ complete_time })

void save_me();

void add_jedi_training_complete(string sponsored, int start_time)
{
   if (sizeof(sponsored_start_time[sponsored]))
   {
      sponsored_start_time[sponsored] += ({ start_time });
      sponsored_complete_time[sponsored] += ({ time() });
   }
   else
   {
      sponsored_start_time[sponsored] = ({ start_time });
      sponsored_complete_time[sponsored] = ({ time() });
   }

   save_me();
}

void remove_jedi_training_complete(string sponsored, int start_time)
{
   if (sizeof(sponsored_start_time[sponsored]))
   {
      if (sizeof(sponsored_start_time[sponsored]) == 1)
      {
         map_delete(sponsored_start_time, sponsored);
         map_delete(sponsored_complete_time, sponsored);
      }
      else
      {
         int index = member_array(start_time, sponsored_start_time[sponsored]);

         if (index > -1)
         {
            sponsored_start_time[sponsored] -= ({ start_time });
            sponsored_complete_time[sponsored] -= ({ sponsored_complete_time[sponsored][index] });
         }
      }
   }
}

void clear_jedi_training_complete()
{
   sponsored_start_time = ([ ]);
   sponsored_complete_time = ([ ]);
}

mixed array get_jedi_sponsored_data()
{
   mixed array data = ({ });

   foreach (string sponsored in keys(sponsored_start_time))
   {
      for (int count = 0; count < sizeof(sponsored_start_time[sponsored]); count++)
      {
         data += ({ ({ sponsored, sponsored_start_time[sponsored][count], sponsored_complete_time[sponsored][count] }) });
      }
   }

   return data;
}

mapping get_jedi_sponsored_start_time()
{
   return sponsored_start_time;
}

mapping get_jedi_sponsored_complete_time()
{
   return sponsored_complete_time;
}