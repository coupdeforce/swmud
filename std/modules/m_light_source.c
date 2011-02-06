// Last edited by deforce on 07-03-2008
// An object that provides light to its environment.
// Iizuka seperated from m_lightable on 06-26-1998

#include <hooks.h>
#include <flags.h>

string lighting_extra_short();
void set_light(int);
mixed call_hooks(string, int);
string the_short();
void set_flag(int);
int test_flag(int);
void clear_flag(int);

private int is_lit = 0;
private int light_level = 1;

int query_is_lit()
{
   return is_lit;
}

void set_light_level(int x)
{
   light_level = x;
}

protected void set_lit(int x)
{
   is_lit = x;
}

mixed extinguish()
{
   string tmp;

   //:HOOK prevent_extinguish
   //A yes/no/error hook that can prevent an object from being extinguished
   tmp = call_hooks("prevent_extinguish", HOOK_YES_NO_ERROR);

   if(!tmp || stringp(tmp))
   {
      return tmp;
   }

   set_lit(0);
   clear_flag(F_LIGHTED);
   set_light(0);

   //:HOOK extinguish
   //called when an object is extinguished.  The return value is ignored
   call_hooks("extinguish", HOOK_IGNORE);

   return 1;
}

varargs mixed light(object with)
{
   string tmp;

   //:HOOK prevent_light
   //A yes/no/error hook that can prevent an object from getting lit
   tmp = call_hooks("prevent_light", HOOK_YES_NO_ERROR);

   if (!tmp || stringp(tmp))
   {
      return tmp;
   }

   set_lit(1);
   set_flag(F_LIGHTED);
   set_light(light_level);

   //:HOOK light
   //called when an object is lit.  The return value is ignored.
   call_hooks("light", HOOK_IGNORE);

   return 1;
}