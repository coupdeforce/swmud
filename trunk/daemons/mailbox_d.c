/*
** mailbox.c -- mailbox daemon
**
** This daemon will manage the mailboxes in the system.  Each mailbox
** contains a list of message keys that a particular user has in their
** mailbox.  The actual messages are managed by MAIL_D.
**
** Public API:
**   get_mailbox(): return a MAILBOX object for the specified user
**   unload_mailbox(): the specified mailbox is no longer needed; close it
**   close_mailboxes(): close all unneeded mailboxes
**
** Deathblade created from code in MAIL_D, 01-19-1996
*/

#include <clean_up.h>

private nosave mapping mailboxes = ([ ]);

private nomask void create()
{
   object mailbox;

   foreach (mailbox in children(MAILBOX))
   {
      string owner;

      if (mailbox && (owner = mailbox->query_owner()))
      {
         mailboxes[owner] = mailbox;
      }
   }
}

nomask object get_mailbox(string the_owner)
{
   object mailbox;

   if (!(mailbox = mailboxes[the_owner]))
   {
      mailbox = mailboxes[the_owner] = new(MAILBOX, the_owner);

      if (!mailbox)
      {
         error("mailbox wasn't created");
      }
   }

   return mailbox;
}

nomask void unload_mailbox(string the_owner)
{
   object mailbox;

   if (mailbox = mailboxes[the_owner])
   {
      destruct(mailbox);
      map_delete(mailboxes, the_owner);
   }
}

nomask void close_mailboxes()
{
   string * names;
   string name;

   names = keys(mailboxes) - users()->query_userid();

   foreach (name in names)
   {
      if (mailboxes[name]) // Might have been destructed
      {
         destruct(mailboxes[name]);
      }

      map_delete(mailboxes, name);
   }
}

// We don't want to disappear, but do some cleaning anyway
int clean_up()
{
   close_mailboxes();
   return ASK_AGAIN;
}