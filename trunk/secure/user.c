// Last edited by deforce on 03-09-2010
// This object represents a particular user and that user's connection.
// 04-30-1995 - Deathblade.  Created.

#include <config.h>
#include <security.h>

inherit M_ACCESS;

inherit __DIR__ "user/login";
inherit __DIR__ "user/sw_body";
inherit __DIR__ "user/sw_user";
inherit __DIR__ "user/failures";
inherit __DIR__ "user/inputsys";
inherit __DIR__ "user/userinfo";
inherit __DIR__ "user/messages";
inherit __DIR__ "user/mailbase";
inherit __DIR__ "user/shell";
inherit __DIR__ "user/newsdata";
inherit __DIR__ "user/channel";
inherit __DIR__ "user/misc";
inherit __DIR__ "user/history";

private string userid; // This users's userid (login id)

nomask string query_userid()
{
   // NOTE: allow this to return 0 so that callers can know that we are still in the login sequence.

   return userid;
}
protected nomask void set_userid(string new_userid)
{
   userid = new_userid;
}

void remove()
{
   object body = query_body();

   if (body && !sizeof(users()))
   {
      return;
   }

   MAILBOX_D->unload_mailbox(query_userid());
   unload_mailer();

   if (body)
   {
      body->move(body->query_last_location());
      destruct(body);
   }

   remove_call_out();
   stop_shell();
   destruct();
}

void quit()
{
   object body = query_body();

   if (body)
   {
      body->quit();
   }

   remove();
}

nomask void save_me()
{
   unguarded(1, (: save_object, LINK_PATH(userid) :));
}

protected nomask void restore_me(string some_userid, int preserve_vars)
{
   unguarded(1, (: restore_object, LINK_PATH(some_userid), preserve_vars :));
}

private nomask void net_dead()
{
   object body = query_body();

   // Tell the body about the net death.  The user object should stick
   // around, though, so we can find it again if the user reconnects.
   // If there is no body yet, then just torch self.
   if (body)
   {
      body->save_me();
      body->net_dead();
      body->move(LINKDEAD_FREEZER);
      call_out((: remove :), 3600);
   }
   else
   {
      remove();
   }
}