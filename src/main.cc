#include <stdlib.h>
#include <stdio.h> 
#include <cmath>    
#include <fstream>
#include "util/CmdLineParser.h"
#include "util/Exception.h"
#include "util/Util.h"
#include "blde.h"
#include "definitions.h"

int main(int argc, char** argv)
{
   try {
      blde_init( argc, argv );
      blde_evolve();
   }
   catch( const CmdLine::E_Exception& e ) {
      std::cerr << e;
      return 1;
   }
   catch( const Exception& e ) {
      std::cerr << e;
      return 2;
   }
   catch( const std::exception& e ) {
      std::cerr << '\n' << "> Error: " << e.what() << std::endl;
      return 4;
   }
   catch( ... ) {
      std::cerr << '\n' << "> Error: " << "An unknown error occurred." << std::endl;
      return 8;
   }

   return 0;
}
