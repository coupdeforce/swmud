mapping exp_multipliers = ([ "primary" : 1, "secondary" : 1.5, "jedi" : 2, "quaternary" : 10 ]);

int array exp_table =
({
   1000,
   2000,
   4000,
   7000,
   11000,
   16000,
   22000,
   29000,
   37000,
   46000,
   56000,
   67000,
   79000,
   92000,
   106000,
   121000,
   137000,
   154000,
   172000,
   191000,
   211000,
   232000,
   254000,
   277000,
   301000,
   326000,
   352000,
   379000,
   407000,
   436000,
   466000,
   497000,
   529000,
   562000,
   596000,
   631000,
   667000,
   704000,
   742000,
   781000,
   821000,
   862000,
   904000,
   947000,
   991000,
   1036000,
   1082000,
   1129000,
   1177000,
   1226000
});

int get_required_exp(string type, int level)
{
   if ((level >= 1) && (level <= 50))
   {
      return to_int(exp_table[level - 1] * exp_multipliers[type]);
   }

   return to_int(exp_table[sizeof(exp_table) - 1] * exp_multipliers[type]);
}