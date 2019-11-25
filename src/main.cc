#include <stdlib.h>
#include <stdio.h> 
#include <cmath>    
#include <fstream>
#include "util/CmdLineParser.h"
#include "util/Exception.h"
#include "util/Util.h"
#include "blde.h"

//g++ -o main main.cc blde.cc follower/accelerator.cc util/CmdLineParser.cc -lOpenCL -I/home/amanda/public/projetos/blde/src/follower/CL -I/home/amanda/public/projetos/blde/src/follower -I. -I/home/amanda/public/projetos/blde/src; ./main -acc -v -pls 128 -pfs 128 -gf 300 -gl 5000 -df 8 -dl 8 -function 1001 -variant RAND -cl-p 2

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
