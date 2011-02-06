//:FUNCTION set_wearing
// void set_wearing(string);
// A quick and easy way to give monsters armor. Pass the filename of an
// armor object from within the setup() function of your monster.
void set_wearing(string s)
{
   object ob = new(s);

   if (!ob) { return; }

   ob->move(this_object());
   ob->do_wear();
}