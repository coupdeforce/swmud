// Last edited by deforce on 07-02-2008

// Note: this is now a wrapper for the efun, allowing "on the fly" modification
#define VOWELS ({'a', 'e', 'i', 'o', 'u'})

//:MODULE
//The grammar module contains a number of functions for producing high quality english output.

//:FUNCTION pluralize
//Convert a noun to it's plural form.  Oddly enough, it's also quite
//good at converting verbs to their singular form, since going from plural to
//singular often entails adding an 's'.  Eg, they run, he runs.

string pluralize(string str)
{
   switch (str)
   {
      case "were": return "was";
      case "don't": return "doesn't";
      case "aren't": return "isn't";
      case "possum": return "possums";
      case "staff": return "staves";
      case "die": return "dies";
      case "laf": return "lafs";
      case "barf": return "barfs";
      case "snarf": return "snarfs";
      case "hum": return "hums";
   }

   if (str[<2..<1] == "ff")  // e.g. "bluff"
   {
      return str + "s";
   }

   if (str[<5..<1] == "penis")
   {
      return str + "es";
   }

   return efun::pluralize(str);
}

// Written by Rust
//:FUNCTION punctuate
//Adds a period to a sentence if needed.
string punctuate(string str)
{
   int last_char;

   if (!stringp(str) || !strlen(str))
   {
      return "";
   }

   while (strlen(str) && (str[<1] == ' '))
   {
      str = str[0..<2];
   }

   if (!strlen(str))
   {
      return "";
   }

   last_char = str[<1];

   if ((strsrch(str, "http://") == -1) &&
      (((last_char >= 'a') && (last_char <= 'z')) ||
      ((last_char >= 'A') && (last_char <= 'Z' ))))
   {
      return str + ".";
   }

   return str;
}

//:FUNCTION number_of
//Handles the common operation: "0 bogs", "1 bog", "2 bogs", ...
//number_of(num, what)
string number_of(int num, string what)
{
   if (num == 1) { return "1 " + what; }

   return num + " " + pluralize(what);
}

//:FUNCTION number_word
//number_word(1) == "one", etc.  number_word(10) == "10" and similarly
//for numbers greater than 10, which is consistent with most English
//style guidelines
string number_word(int num)
{
   switch (num)
   {
      case 1: return "one";
      case 2: return "two";
      case 3: return "three";
      case 4: return "four";
      case 5: return "five";
      case 6: return "six";
      case 7: return "seven";
      case 8: return "eight";
      case 9: return "nine";
      case 10: return "ten";
      case 11: return "eleven";
      case 12: return "twelve";
      case 13: return "thirteen";
      case 14: return "fourteen";
      case 15: return "fifteen";
      case 16: return "sixteen";
      case 17: return "seventeen";
      case 18: return "eighteen";
      case 19: return "nineteen";
      case 20: return "twenty";
      default: return num + "";
   }
}