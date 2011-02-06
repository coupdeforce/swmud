// Last edited by deforce on 03-18-2010
//:MODULE
//Nickname module for the shell.
private nosave mapping nicknames = ([ ]);
private nosave mapping nickname_save = ([ ]);

protected void init_nickname_editor()
{
   new(NICKNAMEMENU)->begin_menu(this_object());
}

// Access functions for the nickname editor
nomask int query_nickname_count()
{
   return sizeof(nicknames);
}

nomask string * query_nickname_names()
{
   return keys(nicknames);
}

nomask string query_one_nickname(string nickname_name)
{
   return nicknames[nickname_name];
}

//:FUNCTION add_nickname_simple
//Add a nickname to a nickname set.
nomask void add_nickname_simple(string nickname_name, string the_nickname)
{
   if (base_name(previous_object()) != NICKNAMEMENU)
   {
      error("Invalid attempt to add a nickname.\n");
   }

#ifdef MAX_NUMBER_OF_NICKNAMES
   if (query_nickname_count() >= MAX_NUMBER_OF_NICKNAMES)
   {
      write("Sorry, you already have the max number of nicknames.\n");
      return;
   }
#endif

   nicknames[nickname_name] = the_nickname;

//### really do this? it was in here... leaving it (DB)
   this_object()->save();
}

private nomask void internal_remove_nickname(string nickname_name)
{
   map_delete(nicknames, nickname_name);
}

//:FUNCTION remove_nickname
//Remove a nickname from a nickname set.
nomask void remove_nickname(string nickname_name)
{
   if (base_name(previous_object()) != NICKNAMEMENU)
   {
      error("Invalid attempt to remove a nickname.\n");
   }

   internal_remove_nickname(nickname_name);
}

// Save functions
private string array prep_nicknames_for_save(string state)
{
   nickname_save = ([]);

   if (state == "saving")
   {
      foreach(string name in keys(nicknames))
      {
         nickname_save[name] = nicknames[name];
      }
   }

   return ({ "nickname_save" });
}

// Save functions
//:FUNCTION setup_for_save
//Sets up M_SAVE to save some variables
void setup_for_save()
{
   // Use the call_other() interface so that we are not protectedally
   // bound to require M_SAVE.  This object this modules is applied
   // to may save natively rather than via M_SAVE.
   this_object()->add_save(({ (: prep_nicknames_for_save :) }));
}

//:FUNCTION add_nickname
//Add a nickname to a nickname set.
varargs void add_nickname(string name, string nickname)
{
#ifdef MAX_NUMBER_OF_NICKNAMES
   if (query_nickname_count() >= MAX_NUMBER_OF_NICKNAMES)
   {
      write("Sorry, you already have too many nicknames.\n");
      return;
   }
#endif

   nicknames[name] = nickname;

   this_object()->save();
}

//:FUNCTION expand_nickname
//Expand an argv with any nicknames if applicable.
mixed expand_nickname(string input)
{
   string array argv = explode(input, " ");
   string array nickname_list = keys(nicknames);
   int index;

   for (int count = 1; count < sizeof(argv); count++)
   {
      index = member_array(argv[count], nickname_list);

      if (index > -1)
      {
         argv[count] = nicknames[nickname_list[index]];
      }
   }

   return implode(argv, " ");
}

private void initialize_nicknames()
{
   nicknames = nickname_save;
}

void create()
{
   if (clonep(this_object()) && (base_name(this_object()) != M_NICKNAME))
   {
#if 1
      initialize_nicknames();
#else
      call_out((: initialize_nicknames :), 0);
#endif
   }
}

varargs protected void cmd_nickname(mixed argv)
{
   switch(sizeof(argv))
   {
      case 1:
//         init_nickname_editor();
         write("Usage: nickname <nickname> <translation>\n");
         return;
      case 2:
//         NICKNAMEMENU->display_one_nickname(argv[1], this_object());
         if (strlen(this_object()->query_one_nickname(argv[1])))
         {
            printf("Nickname: '%s' becomes '%s'\n", argv[1], this_object()->query_one_nickname(argv[1]));
         }
         else
         {
            printf("You have no nickname '%s'.\n", argv[1]);
         }
         return;
      default:
         if (strlen(argv[1]) && strlen(argv[2]))
         {
            if (argv[1] != argv[2])
            {
               add_nickname(argv[1], argv[2]);

               printf("Nickname '%s' set to: %s.\n", argv[1], argv[2]);
            }
            else
            {
               printf("Cannot nickname '%s' to '%s'.\n", argv[1], argv[2]);
            }
         }
   }
}

varargs protected void cmd_list_nicknames()
{
   foreach (string key in sort_array(keys(nicknames), 1))
   {
      write(sprintf("%-12s : %-60s\n", key, nicknames[key]));
   }
}

varargs protected void cmd_remove_nickname(mixed argv)
{
   if (sizeof(argv) != 2)
   {
      printf("Usage: unnickname <nickname>\n");
      return;
   }

   if (!nicknames[argv[1]])
   {
      printf("You don't have '%s' as a nickname.\n", argv[1]);
   }
   else
   {
      internal_remove_nickname(argv[1]);
      printf("Nickname '%s' removed.\n", argv[1]);
   }
}