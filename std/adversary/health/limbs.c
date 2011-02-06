// limbs.c - Adversary module to control limb-based health.
// Body styles are stored in /daemons/body_d.c
#include <limbs.h>
#include <hooks.h>

inherit CLASS_LIMB;
inherit __DIR__ "diagnose_msg";

void die();
varargs void simple_action(string, string);
void update_health();
int hurt_limb(string limb, int amount);
void save_me();
int do_unwield(string);
int query_asleep();
int query_stunned();
mixed call_hooks(string, int);

private mapping health = BODY_D->get_body("humanoid");
private int max_hit_points = 100;
private int hit_points = 100;
private nosave int health_time = time();
private nosave int limb_health_time = health_time;
private nosave int fatigue_time = health_time;
private int heal_rate = 1;
private int max_fatigue = 50;
private int fatigue = 0;
private int dead = 0;

//:FUNCTION update_body_style
// int update_body_style(string body_style);
// Queries BODY_D for the number and type of limbs that will be used.
// e.g. update_body_style("humanoid") will give the body a torso, head,
// two arms, and two legs.
// Returns 0 if the body style doesn't exist or if it doesn't contain
// at least one vital or system limb.
int update_body_style(string body_style)
{
   mapping new_body = BODY_D->get_body(body_style);

   if (!new_body) { return 0; }

   // Make sure we have at least one vital or system limb..
   if (!filter(keys(new_body), (: ((class limb)$(new_body)[$1])->flags &
      (LIMB_VITAL | LIMB_SYSTEM) :)))
   {
      return 0;
   }

    health = new_body;

    return 1;
}

int is_vital_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_VITAL;
}

int is_system_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_SYSTEM;
}

int is_wielding_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_WIELDING;
}

int is_mobile_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_MOBILE;
}

int is_attacking_limb(string limb)
{
   return ((class limb) health[limb])->flags & LIMB_ATTACKING;
}

string underscores(string s)
{
   string tmp;
   // change all spaces to underscores
   tmp = replace_string( s, " ", "_" );

   return tmp;
}

//:FUNCTION query_limb_object
// Returns a dummy object usable in messages.
object query_limb_object(string limb)
{
   if(!limb) { return load_object("/std/limbs/body"); }
   else { return load_object("/std/limbs/" + underscores(limb)); }
}

//:FUNCTION query_limbs
// string array query_limbs();
// Returns a string array containing all limbs that health is applied to.
string array query_limbs()
{
   return keys(health);
}

//:FUNCTION query_wielding_limbs
// string array query_wielding_limbs();

// Returns a string array containing all the limbs that can wield weapons.
string array query_wielding_limbs()
{
   if(intp(health)) { health = BODY_D->get_body("humanoid"); }

   return filter(keys(health), (: ((class limb)health[$1])->flags&LIMB_WIELDING :));
}

//:FUNCTION query_attacking_limbs
// string array query_attacking_limbs();
// Returns a string array containing all the limba that can attack.
string array query_attacking_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags&LIMB_ATTACKING :));
}

//:FUNCTION query_vital_limbs
// string array query_vital_limbs();
// Returns a string array containing all the limbs that are considered
// vital for survival. If any one of these limbs is disabled, the
// adversary dies.
string array query_vital_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags&LIMB_VITAL:));
}

//:FUNCTION query_mobile_limbs
// string array query_mobile_limbs();
// Lima doesn't do anything with mobile limbs, but they're provided for
// those who want health of mobile limbs to affect movement and such.
string array query_mobile_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags&LIMB_MOBILE :));
}

//:FUNCTION query_system_limbs
// string array query_system_limbs();
// Returns a string array of 'system' limbs. When ALL system limbs are
// disabled, the adversary dies.
string array query_system_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->flags&LIMB_SYSTEM :));
}

//:FUNCTION query_non_limbs
// string array query_non_limbs();
// Returns a list of body parts that are not worth tracking health for.
// Such body parts are defined by having a max_health of -1.
string array query_non_limbs()
{
   return filter(keys(health), (: ((class limb)health[$1])->max_health == -1 :));
}

void set_heal_rate(int x)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s heal rate to " + x, previous_object(-1)); }

   if (x < 0)
   {
      x = 0; // A number of things depend on this
   }

   heal_rate = x;
}

int query_heal_rate()
{
   return (heal_rate + call_hooks("heal_rate_bonus", HOOK_SUM));
}

//:FUNCTION set_max_limb_health
// void set_max_limb_health(string limb, int x);
// Sets the maximum health for a given limb.
void set_max_limb_health(string limb, int x)
{
   class limb tmp = health[limb];

   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s max limb health for " + limb + " to " + x, previous_object(-1)); }

   if (!tmp) { error("Bad limb.\n"); }
   if (tmp->max_health == -1 || x < 0) { return; }

   update_health();

   tmp->max_health = x;
//   tmp->health = x;
}

//:FUNCTION set_max_health
// void set_max_health(int x);
// Set the maximum number of hit points of a monster, and also set it's
// hit points to the new max
void set_max_health(int x)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s max health to " + x, previous_object(-1)); }

   max_hit_points = x;

   if (!this_object()->is_body())
   {
      int constitution = this_object()->query_con();
      int max_limb_health = (x * (((constitution / 3) + (constitution * (constitution / 300))) / (133.3 - (constitution / 3)))) + (x / 3);

      foreach (string limb, class limb l in health)
      {
         set_max_limb_health(limb, max_limb_health);
      }
   }
}

//:FUNCTION kill_us
// void kill_us();
// Kills us. =)
void kill_us()
{
   dead = 1;

   die();
}

string query_random_limb()
{
   // We need to modify this to be weighted
   string array limbs = filter(keys(health), (: ((class limb)health[$1])->health > 0 :));

   return sizeof(limbs) ? choice(limbs) : 0;
}

//:FUNCTION disable_limb
// void disable_limb(string limb);
// Disables a limb. For effects on vital and system limbs, see
// query_vital_limbs() and query_system_limbs().
void disable_limb(string limb)
{
   int dont_kill_me_now = 0;

   if (this_object()->is_body()) { this_object()->check_wizard_set("disable " + this_object()->short() + "'s " + limb, previous_object(-1)); }

   if (health[limb]->health == -1) { return; }

   ((class limb)health[limb])->health = 0;

   if (((class limb)health[limb])->flags & LIMB_VITAL)
   {
      kill_us();
      return;
   }
   else if (((class limb)health[limb])->flags & LIMB_SYSTEM)
   {
      foreach (string l in query_system_limbs())
      {
         if (((class limb)health[l])->health != 0) { dont_kill_me_now = 1; }
      }

      if (dont_kill_me_now == 1) { simple_action("$N cannot use $p $o anymore.", limb); }
      else { kill_us(); }
   }
   else if (((class limb)health[limb])->flags & LIMB_WIELDING)
   {
      simple_action("$P $o is severed and falls to the ground.", limb);

      do_unwield(limb);
   }
   else if (member_array(limb, ({ "head", "torso" })) == -1)
   {
      simple_action("$P $o is severed and falls to the ground.", limb);
   }

   new("/std/severed_limb", this_object()->query_primary_id(), limb)->move(environment(this_object()));

   // This is hard-coded, but better than nothing for now
   if (limb == "left arm") { disable_limb("left hand"); }
   else if (limb == "right arm") { disable_limb("right hand"); }
   else if (limb == "left leg") { disable_limb("left foot"); }
   else if (limb == "right leg") { disable_limb("right foot"); }
}

//:FUNCTION enable_limb
// void enable_limb(string limb);
// Re-enables a disabled limb.
void enable_limb(string limb)
{
   class limb tmp = health[limb];

   if (this_object()->is_body()) { this_object()->check_wizard_set("enable " + this_object()->short() + "'s " + limb, previous_object(-1)); }

   if (tmp->health < 1) { tmp->health = 1; }

   health[limb] = tmp;
}

varargs void set_health(int x)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s health to " + x, previous_object(-1)); }

   hit_points = x;
   health_time = time();
}

varargs void set_limb_health(string limb, int x)
{
   class limb tmp = health[limb];

   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s limb health for " + limb + " to " + x, previous_object(-1)); }

   if (!tmp) { error("Bad limb.\n"); }

   update_health();

   if (x > tmp->max_health) { error("Attempt to set health > max_health.\n"); }

   if (tmp->health < 1 || dead) { return; }

   tmp->health = x;

   if (tmp->health <= 0) { disable_limb(limb); }

   limb_health_time = time();
}

//:FUNCTION hurt_us
// varargs int hurt_us(int x, string limb);
// Hurt us a specified amount.
varargs int hurt_us(int amount, string limb)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("hurt " + this_object()->short() + " for " + amount, previous_object(-1)); }

   if (amount > 0)
   {
      object body = this_object();
      string output;

      update_health();

      // Hurt our overall HP
      hit_points -= amount;

      // Do half damage to the limb that was hit
      if (limb)
      {
         if (limb == "random") { limb = query_random_limb(); }
         else if (limb == "all")
         {
            foreach (limb in keys(health))
            {
               if (!health[limb]) { continue; }

               if ((health[limb] && (health[limb]->health < 1)) || dead) { continue; }

               health[limb]->health -= (amount / 2);

               if (health[limb]->health <= 0)
               {
                  health[limb]->health = 0;
                  disable_limb(limb);
               }
            }

            return;
         }

         if (!health[limb]) { return 0; }

         if ((health[limb] && (health[limb]->health < 1)) || dead) { return 0; }

         health[limb]->health -= (amount / 2);

         if (health[limb]->health <= 0)
         {
            health[limb]->health = 0;
            disable_limb(limb);
         }
      }

      output = "hp: " + hit_points + " (-" + amount + ")";
      output += (limb && (amount / 2) ? ("  %^CYAN%^" + limb + "%^RESET%^: " + health[limb]->health + " (-" + (amount / 2) + ")") : "");

      output += "\n\n";

      tell(body, output);

      if (hit_points <= 0) { kill_us(); }
   }

   return amount;
}

//:FUNCTION weaken_us
// varargs int weaken_us(int x, string limb);
// Hurt us a specified amount, without going below 1.
varargs int weaken_us(int amount, string limb)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("weaken " + this_object()->short() + " for " + amount, previous_object(-1)); }

   if (amount > 0)
   {
      object body = this_object();
      string output;

      update_health();

      if ((hit_points - amount) < 1)
      {
         amount += hit_points - amount - 1;
      }

      // Hurt our overall HP
      hit_points -= amount;

      if (hit_points < 1)
      {
         hit_points = 1;
      }

      // Do half damage to the limb that was hit
      if (limb)
      {
         if (limb == "random") { limb = query_random_limb(); }
         if (!health[limb]) { return 0; }

         if ((health[limb] && (health[limb]->health < 1)) || dead) { return 0; }

         health[limb]->health -= (amount / 2);

         if (health[limb]->health < 1)
         {
            health[limb]->health = 1;
         }
      }

      if (amount > 0)
      {
         output = "hp: " + hit_points + " (-" + amount + ")";
         output += (limb && (amount / 2) ? ("  %^CYAN%^" + limb + "%^RESET%^: " + health[limb]->health + " (-" + (amount / 2) + ")") : "");

         output += "\n\n";

         tell(body, output);
      }
   }

   return amount;
}

//:FUNCTION hurt_limb
// varargs int hurt_limb(string limb, int x);
// Hurt a limb for a specified amount.
int hurt_limb(string limb, int amount)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("hurt " + this_object()->short() + "'s " + limb + " for " + amount, previous_object(-1)); }

   if (amount > 0)
   {
      object body = this_object();
      string output;

      update_health();

      if (limb == "random") { limb = query_random_limb(); }
      if (!health[limb]) { return 0; }

      if ((health[limb] && (health[limb]->health < 1)) || dead) { return 0; }

      health[limb]->health -= (amount / 2);

      if (health[limb]->health <= 0)
      {
         health[limb]->health = 0;
         disable_limb(limb);
      }

      tell(body, "%^CYAN%^" + limb + "%^RESET%^: " + health[limb]->health + " (-" + amount + ")\n\n");
   }

   return amount;
}

//:FUNCTION heal_limb
// protected void heal_limb(string limb, int x);
// Heal us a specified amount, truncating at max_health.
protected varargs void heal_limb(string limb, int x, int silent)
{
   class limb tmp = health[limb];

   if (this_object()->is_body()) { this_object()->check_wizard_set("heal " + this_object()->short() + "'s " + limb + " by " + x, previous_object(-1)); }

   if (!tmp) { error("Bad limb.\n"); }

   if ((tmp->health < 1) || dead) { return; }

   if (tmp->health < tmp->max_health)
   {
      tmp->health += x;

      if (tmp->health > tmp->max_health) { tmp->health = tmp->max_health; }

      if (!silent)
      {
         write(limb + ": " + tmp->health + "/" + tmp->max_health + "\n");
      }
   }
}

//:FUNCTION is_limb
// int is_limb(string s);
// Returns 1 if 's' is a valid limb.
int is_limb(string s)
{
   return !undefinedp(health[s]);
}

//:FUNCTION query_max_health
// varargs int query_max_health(string limb);
// Tells us the maximum health of a given limb.
varargs int query_max_health(string limb)
{
   int x = 1;
   if (!limb) { return max_hit_points; }
   if (limb) { return is_limb(limb) ? ((class limb)health[limb])->max_health : 0; }

   foreach(string l in keys(health))
   {
      if(health[l]->max_health > x)
      {
         x = health[l]->max_health;
      }
   }

   return x;
}

//:FUNCTION heal_us
// varargs void heal_us(int x, string limb, int enforce max);
// Heals all limbs by 'x' amount.
varargs void heal_us(int x, string limb, int silent)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("heal " + this_object()->short() + " by " + x, previous_object(-1)); }

   if (!limb || undefinedp(limb) || (limb == "none"))
   {
      if (hit_points < max_hit_points)
      {
         hit_points += x;

         if (hit_points > max_hit_points) { hit_points = max_hit_points; }

         if (!silent)
         {
            write("hp: " + hit_points + "/" + max_hit_points + "    dr: undrugged\n");
         }
      }
   }
   else
   {
      heal_limb(limb, x);
   }
}

//:FUNCTION heal_all
// void heal_all();
// Heal us entirely.
void heal_all()
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("completely heal " + this_object()->short(), previous_object(-1)); }

   foreach (string limb in keys(health))
   {
      if (!health[limb]->health) { enable_limb(limb); }

      health[limb]->health = health[limb]->max_health;
   }

   heal_us(query_max_health());
}

//:FUNCTION reincarnate
// void reincarnate();
// Makes us alive again!
void reincarnate()
{
   if (dead)
   {
//      foreach (string limb, class limb l in health) { enable_limb(limb); }
      set_health(1);
      enable_limb("head");
      enable_limb("torso");

      dead = 0;
      health_time = time();
      limb_health_time = health_time;
      fatigue_time = health_time;

      foreach (object target in this_object()->query_targets() + this_object()->query_unfinished_business())
      {
         this_object()->stop_fight(target);
      }

      environment(this_object())->do_looking(1, this_object());
   }
}

void update_health()
{
   int time;

   if (dead || (heal_rate < 1)) { return; }

   time = time();

   if ((time - health_time) > 15)
   {
      int heal_rate_temp = heal_rate + call_hooks("heal_rate_bonus", HOOK_SUM);
      int number_of_heals = ((time - health_time) - ((time - health_time) % 16)) / 16;
      int heal_amount = number_of_heals * heal_rate_temp;

      if (heal_amount < 1)
      {
         heal_amount = 1;
      }

      // Pretend we've been healing
      heal_us(heal_amount, "none", 1);

      health_time += (number_of_heals * 16);

      if ((time - fatigue_time) > 31)
      {
         number_of_heals = ((time - fatigue_time) - ((time - fatigue_time) % 32)) / 32;

         if (fatigue > 0)
         {
            fatigue -= (number_of_heals * (6 - this_object()->query_body_size()));

            if (fatigue < 0)
            {
               fatigue = 0;
            }
         }

         fatigue_time += (number_of_heals * 32);
      }

      if ((time - limb_health_time) > 63)
      {
         number_of_heals = ((time - health_time) - ((time - health_time) % 64)) / 64;
         heal_amount = number_of_heals * heal_rate_temp;

         foreach(string limb in keys(health))
         {
            heal_limb(limb, heal_amount, 1);
         }

         limb_health_time += (number_of_heals * 64);
      }
   }
}

//:FUNCTION query_health
// int query_health(string limb);
// Find the current number of hitpoints of a monster
int query_health(string limb)
{
   update_health();

   return hit_points;
}

int query_limb_health(string limb)
{
   update_health();

   return ((class limb)health[limb])->health;
}

int all_healthy()
{
   if (hit_points < max_hit_points)
   {
      return 0;
   }

   foreach (string limb in keys(health))
   {
      if ((health[limb]->health > 0) && (health[limb]->health < health[limb]->max_health))
      {
         return 0;
      }
   }

   return 1;
}

//:FUNCTION set_fatigue_amount
void set_fatigue_amount(int amount)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s fatigue to " + amount, previous_object(-1)); }

   fatigue_time = time();
   fatigue = amount;
}

//:FUNCTION add_fatigue_amount
void add_fatigue_amount(int amount)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("add " + amount + " to " + this_object()->short() + "'s fatigue", previous_object(-1)); }

   update_health();

   fatigue += amount;

   if (fatigue < 0)
   {
      fatigue = 0;
   }
}

//:FUNCTION query_fatigue_amount
int query_fatigue_amount()
{
   update_health();

   return fatigue;
}

//:FUNCTION set_max_fatigue_amount
void set_max_fatigue_amount(int amount)
{
   if (this_object()->is_body()) { this_object()->check_wizard_set("set " + this_object()->short() + "'s max fatigue to " + amount, previous_object(-1)); }

   max_fatigue = amount;
}

//:FUNCTION query_max_fatigue_amount
int query_max_fatigue_amount()
{
   return max_fatigue;
}

//:FUNCTION is_too_fatigued
int is_too_fatigued()
{
   return fatigue >= max_fatigue;
}

int query_ghost()
{
   return dead;
}

//:FUNCTION badly_wounded
// int badly_wounded();
// Returns 1 if we're near death.
int badly_wounded()
{
   foreach (string l, class limb lb in health)
   {
      if ((lb->flags & LIMB_VITAL) && (lb->health < lb->max_health/5)) { return 1; }
   }
}

string diagnose()
{
   string ret;
   string array damaged_limbs;

   if (query_ghost())
   {
      return "$N $vare dead. Other than that, things are going pretty well for $n.\n";
   }

   if (query_asleep()) { ret = "$N $vare asleep.\n"; }
   else if(query_stunned()) { ret = "$N $vare stunned.\n"; }
   else { ret = ""; }

   ret += "Overall condition: " + diagnose_msg(hit_points * 100 / max_hit_points);

   damaged_limbs = filter(query_limbs(),(: query_health($1) < health[$1]->max_health :));

   foreach(string limb in damaged_limbs)
   {
      ret += limb + ": " + diagnose_msg(health[limb]->health * 100 / health[limb]->max_health);
   }

   if (ret == "") { ret = "You are in excellent health.\n"; }

   return ret;
}