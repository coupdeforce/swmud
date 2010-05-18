inherit M_LIGHTSABER_COMPONENT;

#include <lightsaber.h>

varargs void setup(string color)
{
   if (!strlen(color))
   {
      color = "blue";
   }

   set_id("crystal", color, "lightsaber_color_crystal");
   set_adj(get_lightsaber_color_code(color) + "%^RESET%^");
   set_long("A facetted crystal used in the construction of a lightsaber.  It glows faintly with an inner " + color + " light.");
   set_component_name(color);
   set_component_type("color");
   set_mass(50);
   set_value(5000);
}