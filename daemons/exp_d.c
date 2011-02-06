// Last edited by deforce on 05-10-2010

mapping exp_multipliers = ([ "primary" : 1, "secondary" : 2, "jedi" : 3, "quaternary" : 10, "jedi quaternary" : 15, "ronin jedi" : 4 ]);

int array exp_table =
({
   0,
   3000,
   6000,
   10000,
   15000,
   21000,
   28000,
   36000,
   45000,
   55000,
   66000,
   78000,
   91000,
   105000,
   120000,
   136000,
   153000,
   171000,
   190000,
   210000,
   231000,
   253000,
   276000,
   300000,
   325000,
   351000,
   378000,
   406000,
   435000,
   465000,
   496000,
   528000,
   561000,
   595000,
   630000,
   666000,
   703000,
   741000,
   780000,
   820000,
   861000,
   903000,
   946000,
   990000,
   1035000,
   1081000,
   1128000,
   1176000,
   1225000,
   1275000
});

int get_required_exp(string type, int level)
{
   if (level == 1)
   {
      if (type[<10..<1] != "quaternary")
      {
         return 1000;
      }
      else
      {
         return 5000;
      }
   }

   return exp_table[level - 1] * exp_multipliers[type];
}