// Last edited by deforce on 07-02-2008
inherit M_DAEMON_DATA;

string get_parent(string file);

mapping areas = ([]);

mapping get_areas()
{
   return areas;
}

void add_area(string name, string file)
{
   if (strlen(name) && (strlen(file) > 3))
   {
      areas[name] = file;

      save_me();
   }
}

void remove_area(string name)
{
   if (strlen(name) > 2)
   {
      map_delete(areas, name);

      save_me();
   }
}

string query_morgue(string room_file)
{
   string array dirs = keys(areas);
   string test_dir = get_parent(room_file);
   int match = member_array(test_dir, dirs);

   if (test_dir[0..0] != "/") { test_dir = "/" + test_dir; }

   while (strlen(test_dir) > 1)
   {
      match = member_array(test_dir, dirs);

      if (match > -1) { return areas[dirs[match]]; }

      test_dir = get_parent(test_dir);
   }

   return room_file;
}

string get_parent(string file)
{
   if (strlen(file) < 2) { return "/"; }

   return file[0..(strsrch(file[0..(strlen(file) - 2)], "/", -1) - 1)];
}

void create()
{
   ::create();
}