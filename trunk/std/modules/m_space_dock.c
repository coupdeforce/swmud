// Functions
string array get_visible_docks();
string array get_hidden_docks();
string array get_all_docks();
string get_dock_file(string name);
void add_dock(string name, string file);
void add_hidden_dock(string name, string file);

// Variables
mapping docks = ([ ]);
mapping hidden_docks = ([ ]);

string array get_visible_docks()
{
   return keys(docks);
}

string array get_hidden_docks()
{
   return keys(hidden_docks);
}

string array get_all_docks()
{
   return keys(docks) + keys(hidden_docks);
}

string get_dock_file(string name)
{
   if (docks[name])
   {
      return docks[name];
   }

   return hidden_docks[name];
}

int is_space_dock() { return 1; }

void add_dock(string name, string file)
{
   docks[name] = file;
}

void add_hidden_dock(string name, string file)
{
   hidden_docks[name] = file;
}