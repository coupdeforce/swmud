mapping messages =
([
   "combat-unarmed" :
   ([
      "miss" : ({ "$N $vmiss $p1 $o2 with a weak punch.",
         "$N $vmiss $p1 $o2 with a weak kick." }),
      "none" : ({ "$N barely $vmiss $p1 $o2 with a weak punch.",
         "$N barely $vmiss $p1 $o2 with a weak kick." }),
      "dam1" : ({ "$N $vtap $p1 $o2 lightly with $p fist.",
         "$N $vkick $p1 $o2 lightly with $p foot." }),
      "dam2" : ({ "$N $vtap $p1 $o2 with $p fist.",
         "$N $vkick $p1 $o2 with $p foot." }),
      "dam3" : ({ "$N $vsting $p1 $o2 with $p fist.",
         "$N $vsting $p1 $o2 with $p foot." }),
      "dam4" : ({ "$N $vstrike $p1 $o2 with $p fist.",
         "$N $vstrike $p1 $o2 with $p foot." }),
      "dam5" : ({ "$N $vstrike $p1 $o2 badly with $p fist.",
         "$N $vstrike $p1 $o2 badly with $p foot." }),
      "dam6" : ({ "$N $vpound $p1 $o2 with $p fist.",
         "$N $vpound $p1 $o2 with $p foot." }),
      "dam7" : ({ "$N $vpound $p1 $o2 horribly with $p fist.",
         "$N $vpound $p1 $o2 horribly with $p foot." }),
      "dam8" : ({ "$N $vslam $p1 $o2 with $p fist.",
         "$N $vslam $p1 $o2 with $p foot." }),
      "dam9" : ({ "$N $vslam $p1 $o2 mercilessly with $p fist.",
         "$N $vslam $p1 $o2 mercilessly with $p foot." }),
      "dam10" : ({ "$N $vblast $p1 $o2 with $p fist.",
         "$N $vblast $p1 $o2 with $p foot." }),
      "dam11" : ({ "$N $vblast $p1 $o2 powerfully with $p fist.",
         "$N $vblast $p1 $o2 powerfully with $p foot." }),
      "dam12" : ({ "$N $vcrush $p1 $o2 with $p fist.",
         "$N $vcrush $p1 $o2 with $p foot." }),
      "dam13" : ({ "$N $vcrush $p1 $o2 mightily with $p fist.",
         "$N $vcrush $p1 $o2 mightily with $p foot." }),
      "dam14" : ({ "$N $vdestroy $p1 $o2 with $p fist.",
         "$N $vdestroy $p1 $o2 with $p foot." }),
      "dam15" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam16" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam17" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam18" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam19" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam20" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam21" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "dam22" : ({ "$N $vdestroy $p1 $o2 utterly with $p fist.",
         "$N $vdestroy $p1 $o2 utterly with $p foot." }),
      "parry" : ({ "$N $vblock $p1 fist with $p arm.",
         "$N $vblock $p1 fist with $p leg." }),
      "dodge" : ({ "$N $vdodge $p1 punch to $p $o2.",
         "$N $vdodge $p1 kick to $p $o2." }),
   ]),

   "combat-blade" :
   ([
      "miss" : "$N $vmiss $p1 $o2 with a slash from $p $o.",
      "none" : "$N barely $vmiss $p1 $o2 with a slash from $p $o.",
      "dam1" : "$N $vscratch $p1 $o2 lightly with the edge of $p $o.",
      "dam2" : "$N $vscrape $p1 $o2 lightly with the edge of $p $o.",
      "dam3" : "$N $vscrape $p1 $o2 with the edge of $p $o.",
      "dam4" : "$N $vgraze $p1 $o2 with $p $o.",
      "dam5" : "$N $vgraze $p1 $o2 sharply with $p $o.",
      "dam6" : "$N $vwound $p1 $o2 with $p $o.",
      "dam7" : "$N $vwound $p1 $o2 terribly with $p $o.",
      "dam8" : "$N $vcut $p1 $o2 with $p $o.",
      "dam9" : "$N $vcut $p1 $o2 badly with $p $o.",
      "dam10" : "$N $vslice $p1 $o2 with $p $o.",
      "dam11" : "$N $vslice $p1 $o2 horribly with $p $o.",
      "dam12" : "$N $vstrike $p1 $o2 letting blood with $p $o.",
      "dam13" : "$N $vmutilate $p1 $o2 with $p $o.",
      "dam14" : "$N $vdestroy $p1 $o2 with $p $o.",
      "dam15" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam16" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam17" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam18" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam19" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam20" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam21" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam22" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "parry" : "$N $vparry $p1 $o.",
      "dodge" : "$N $vdodge $p1 slash towards $p $o2.",
      "disarm " : "$N $vslash $p1 hand with $p $o, causing $t to drop $p1 $o1.",
   ]),

   "combat-blunt" :
   ([
      "miss" : "$N $vmiss $p1 $o2 with a blow from $p $o.",
      "none" : "$N barely $vmiss $p1 $o2 with a blow from $p $o.",
      "dam1" : "$N $vtap $p1 $o2 lightly with $p $o.",
      "dam2" : "$N $vpoke $p1 $o2 with $p $o.",
      "dam3" : "$N $vsting $p1 $o2 with $p $o.",
      "dam4" : "$N $vstrike $p1 $o2 with $p $o.",
      "dam5" : "$N $vstrike $p1 $o2 badly with $p $o.",
      "dam6" : "$N $vpound $p1 $o2 with $p $o.",
      "dam7" : "$N $vpound $p1 $o2 horribly with $p $o.",
      "dam8" : "$N $vslam $p1 $o2 with $p $o.",
      "dam9" : "$N $vslam $p1 $o2 mercilessly with $p $o.",
      "dam10" : "$N $vblast $p1 $o2 with $p $o.",
      "dam11" : "$N $vblast $p1 $o2 powerfully with $p $o.",
      "dam12" : "$N $vcrush $p1 $o2 with $p $o.",
      "dam13" : "$N $vcrush $p1 $o2 mightily with $p $o.",
      "dam14" : "$N $vdestroy $p1 $o2 with $p $o.",
      "dam15" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam16" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam17" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam18" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam19" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam20" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam21" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "dam22" : "$N $vdestroy $p1 $o2 utterly with $p $o.",
      "parry" : "$N $vparry $p1 $o.",
      "dodge" : "$N $vdodge $p1 blow towards $p $o2.",
      "disarm " : "$N $vstrike $p1 hand with $p $o, causing $t to drop $p1 $o1.",
   ]),

   "combat-blaster" :
   ([
      "miss" : "$N $vmiss $p1 $o2 with a blaster bolt from $p $o.",
      "none" : "$N barely $vmiss $p1 $o2 with a blaster bolt from $p $o.",
      "dam1" : "$N $vnick $p1 $o2 lightly with $p $o.",
      "dam2" : "$N $vnick $p1 $o2 with $p $o.",
      "dam3" : "$N $vinjure $p1 $o2 with $p $o.",
      "dam4" : "$N $vinjure $p1 $o2 severely with $p $o.",
      "dam5" : "$N $vwound $p1 $o2 with $p $o.",
      "dam6" : "$N $vwound $p1 $o2 badly with $p $o.",
      "dam7" : "$N $vnail $p1 $o2 with $p $o.",
      "dam8" : "$N $vdamage $p1 $o2 with $p $o.",
      "dam9" : "$N $vdamage $p1 $o2 terribly with $p $o.",
      "dam10" : "$N $vraze $p1 $o2 with $p $o.",
      "dam11" : "$N $vobliterate $p1 $o2 with $p $o.",
      "dam12" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam13" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam14" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam15" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam16" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam17" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam18" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam19" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam20" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam21" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dam22" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o.",
      "dodge" : "$N $vdodge a blaster bolt heading towards $p1 $o2 from $p $o.",
      "deflect" : "$N $vfire at $p1 $o2 with $p $o, and $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-none" : "$N barely $vmiss $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam1" : "$N $vnick $p1 $o2 lightly with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam2" : "$N $vnick $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam3" : "$N $vinjure $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam4" : "$N $vinjure $p1 $o2 severely with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam5" : "$N $vwound $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam6" : "$N $vwound $p1 $o2 badly with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam7" : "$N $vnail $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam8" : "$N $vdamage $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam9" : "$N $vdamage $p1 $o2 terribly with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam10" : "$N $vraze $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam11" : "$N $vobliterate $p1 $o2 with $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam12" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam13" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam14" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam15" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam16" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam17" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam18" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam19" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam20" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam21" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "deflect-dam22" : "$N $vannihilate $p1 $o2 with a blaster bolt from $p $o, as $n1 $v1deflect the energy bolt with $p1 lightsaber.",
      "reflect" : "$N $vfire at $p1 $o2 with $p $o, and $n1 $v1reflect the energy bolt with $p1 lightsaber towards $p $o3.",
   ]),

   "combat-blaster-reflect" :
   ([
      "miss" : "$N1 $v1fire at $n, and the reflected energy bolt from $p lightsaber misses $p1 $o1.",
      "none" : "$N1 $v1fire at $n, and the reflected energy bolt from $p lightsaber barely misses $p1 $o1.",
      "reflect" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o1 with $p lightsaber!",
      "dam1" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam2" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam3" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam4" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam5" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam6" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam7" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam8" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam9" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam10" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam11" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam12" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam13" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam14" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam15" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam16" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam17" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam18" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam19" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam20" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam21" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dam22" : "$N1 $v1fire at $n, and $n $vreflect the energy bolt back towards $p1 $o2 with $p lightsaber!",
      "dodge" : "$N1 $v1fire at $n, and $v1dodge the reflected energy bolt towards $p1 $o1 from $p lightsaber!",
   ]),

   "combat-lightning" :
   ([
      "miss" : "$N $vsend bolts of lightning from $p hands, missing $p target.",
      "none" : "$N $vsend bolts of lightning from $p hands, barely missing $p1 $o2.",
      "dam1" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam2" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam3" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam4" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam5" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam6" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam7" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam8" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam9" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam10" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam11" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam12" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam13" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam14" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam15" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam16" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam17" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam18" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam19" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam20" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam21" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
      "dam22" : "$N $vsend bolts of lightning from $p hands to $p1 $o2!",
   ]),

   "combat-choke" :
   ([
      "miss" : "$N $vconcentrate and $vfail to choke $p target.",
      "none" : "$N $vconcentrate and $vfail to constrict $p1 throat.",
      "dam1" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam2" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam3" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam4" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam5" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam6" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam7" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam8" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam9" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam10" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam11" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam12" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam13" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam14" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam15" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam16" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam17" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam18" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam19" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam20" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam21" : "$N $vconcentrate and $vconstrict $p1 throat.",
      "dam22" : "$N $vconcentrate and $vconstrict $p1 throat.",
   ]),

   "combat-force-throw" :
   ([
      "miss" : "$N $vuse the Force to throw the $o, missing $p target.",
      "none" : "$N $vuse the Force to throw the $o, barely missing $p1 $o2.",
      "dam1" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam2" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam3" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam4" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam5" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam6" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam7" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam8" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam9" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam10" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam11" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam12" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam13" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam14" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam15" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam16" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam17" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam18" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam19" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam20" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam21" : "$N $vuse the Force to throw the $o at $p1 $o2!",
      "dam22" : "$N $vuse the Force to throw the $o at $p1 $o2!",
   ]),

   "combat-reflect" :
   ([
      "miss" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "none" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam1" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam2" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam3" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam4" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam5" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam6" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam7" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam8" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam9" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam10" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam11" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam12" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam13" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam14" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam15" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam16" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam17" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam18" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam19" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam20" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam21" : "$P shield reflects $p1 attack onto $p1 $o2!",
      "dam22" : "$P shield reflects $p1 attack onto $p1 $o2!",
   ]),

   "combat-inject" :
   ([
      "miss" : "$N $vfail to inject $p1 $o2 with $p $o.",
      "none" : "$N $vinject $p1 $o2 with $p $o.",
      "dam1" : "$N $vinject $p1 $o2 with $p $o.",
      "dam2" : "$N $vinject $p1 $o2 with $p $o.",
      "dam3" : "$N $vinject $p1 $o2 with $p $o.",
      "dam4" : "$N $vinject $p1 $o2 with $p $o.",
      "dam5" : "$N $vinject $p1 $o2 with $p $o.",
      "dam6" : "$N $vinject $p1 $o2 with $p $o.",
      "dam7" : "$N $vinject $p1 $o2 with $p $o.",
      "dam8" : "$N $vinject $p1 $o2 with $p $o.",
      "dam9" : "$N $vinject $p1 $o2 with $p $o.",
      "dam10" : "$N $vinject $p1 $o2 with $p $o.",
      "dam11" : "$N $vinject $p1 $o2 with $p $o.",
      "dam12" : "$N $vinject $p1 $o2 with $p $o.",
      "dam13" : "$N $vinject $p1 $o2 with $p $o.",
      "dam14" : "$N $vinject $p1 $o2 with $p $o.",
      "dam15" : "$N $vinject $p1 $o2 with $p $o.",
      "dam16" : "$N $vinject $p1 $o2 with $p $o.",
      "dam17" : "$N $vinject $p1 $o2 with $p $o.",
      "dam18" : "$N $vinject $p1 $o2 with $p $o.",
      "dam19" : "$N $vinject $p1 $o2 with $p $o.",
      "dam20" : "$N $vinject $p1 $o2 with $p $o.",
      "dam21" : "$N $vinject $p1 $o2 with $p $o.",
      "dam22" : "$N $vinject $p1 $o2 with $p $o.",
   ]),

   "none" :
   ([
      "miss" : "",
      "none" : "",
      "dam1" : "",
      "dam2" : "",
      "dam3" : "",
      "dam4" : "",
      "dam5" : "",
      "dam6" : "",
      "dam7" : "",
      "dam8" : "",
      "dam9" : "",
      "dam10" : "",
      "dam11" : "",
      "dam12" : "",
      "dam13" : "",
      "dam14" : "",
      "dam15" : "",
      "dam16" : "",
      "dam17" : "",
      "dam18" : "",
      "dam19" : "",
      "dam20" : "",
      "dam21" : "",
      "dam22" : "",
   ]),

   "living-default" :
   ([
      "leave": "$N $vleave $o.",
      "mleave": "$N $vdisappear in a puff of smoke.",
      "enter": "$N $venter.",
      "menter": "$N $vappear in a puff of smoke.",
      "invis": "$N $vfade from view.",
      "vis": "$N $vfade into view.",
      "home": "$N $vgo home.",
      "clone": "$N $vclone the $o.",
      "destruct": "$N $vdest the $o.",
      "fol_leave": "$N $vfollow $t $o.",
      "fol_enter": "$N $venter following $t.",
      "clean": "$N $vtake a broom and $vclean $p surroundings.",
   ]),

   "nonsense" :
   ({
      "That doesn't seem to work.",
   }),
  "discarded" : ({ "%s" }),
  "discarded-plural" : ({ "%s" }),
]);

mixed get_messages(string type)
{
   return messages[type];
}