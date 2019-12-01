#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <cmath> 
#include <limits>
#include <string>   
#include <vector>
#include <utility>
#include <iostream> 
#include <fstream> 
#include "accelerator.h"
#include "../util/CmdLineParser.h"
#include "../util/Util.h"

//TODO
//#include <Poco/Path.h>

//TODO
/* Macros to stringify an expansion of a macro/definition */
#define xstr(a) str(a)
#define str(a) #a

using namespace std;

//TODO
std::string getAbsoluteDirectory(std::string filepath) 
{
   //Poco::Path p(filepath);
   //string filename = p.getFileName();
   //p.makeAbsolute();
   //string absolute_path = p.toString();

   ///* The absolute_path consists of: directory + filename; so, to get
   //   only the directory, it suffices to do: absolute_path - filename */
   //return std::string(absolute_path, 0, absolute_path.length() - filename.length());
   return std::string("/home/amanda/public/projetos/blde/src");
}

/** ****************************************************************** **/
/** ***************************** TYPES ****************************** **/
/** ****************************************************************** **/

namespace { static struct t_data { int population_leader_size; int population_follower_size; int leader_dimension; int follower_dimension; int num_generation_follower; real_t crossover_rate; real_t f; int r; int p; int q; int s; std::string variant; std::string function; std::string function_str; unsigned local_size; unsigned global_size; cl::Device device; cl::Context context; cl::Kernel kernel_seed; cl::Kernel kernel_follower; cl::Kernel kernel_leader; cl::CommandQueue queue; cl::Buffer seed_buffer; cl::Buffer follower_buffer_popL; cl::Buffer follower_buffer_popLValoresF; cl::Buffer follower_buffer_popF; cl::Buffer follower_buffer_vf; cl::Buffer follower_buffer_vl; cl::Buffer leader_buffer_fit_popL; cl::Buffer leader_buffer_fit_popLValoresF; std::string executable_directory; bool verbose; } data; };

/** ****************************************************************** **/
/** *********************** AUXILIARY FUNCTION *********************** **/
/** ****************************************************************** **/

// -----------------------------------------------------------------------------
int opencl_init( int platform_id, int device_id, cl_device_type type )
{
   vector<cl::Platform> platforms;

   /* Iterate over the available platforms and pick the list of compatible devices
      from the first platform that offers the device type we are querying. */
   cl::Platform::get( &platforms );

   vector<cl::Device> devices;

   /*

   Possible options:

   INV = CL_INVALID_DEVICE_TYPE

        type  device  platform

   1.   INV     X        X
   2.   INV    -1        X     --> device = 0
   3.   INV     X       -1     --> platform = 0
   4.    X     -1        X     
   5.    X     -1       -1 
   6.   INV    -1       -1     --> platform = last_platform; device = 0 
   7.    X      X        X     --> reset type = CL_INVALID_DEVICE_TYPE 

   */

   if( platform_id >= 0 && device_id >= 0 ) // option 7
   {
      type = CL_INVALID_DEVICE_TYPE; 
   }

   if( platform_id < 0 && device_id >= 0 ) // option 3
   {
      platform_id = 0; 
   }

   // Check if the user gave us a valid platform 
   if( platform_id >= (int) platforms.size() )
   {
      fprintf(stderr, "Valid platform range: [0, %d].\n", (int) (platforms.size()-1));
      return 1;
   }

   bool leave = false;

   int first_platform = platform_id >= 0 ? platform_id : 0;
   int last_platform  = platform_id >= 0 ? platform_id + 1 : platforms.size();
   for( int m = first_platform; m < last_platform; m++ )
   {
      platforms[m].getDevices( CL_DEVICE_TYPE_ALL, &devices );

      // Check if the user gave us a valid device 
      if( device_id >= (int) devices.size() ) 
      {
         fprintf(stderr, "Valid device range: [0, %d].\n", (int) (devices.size()-1));
         return 1;
      }

      int first_device = device_id >= 0 ? device_id : 0;
      data.device = devices[first_device];

      if( type != CL_INVALID_DEVICE_TYPE && device_id < 0 ) // options 4 e 5
      {
         for ( int n = 0; n < devices.size(); n++ )
         {
           /*
        
           Possible options:
        
           int   bits      type

            1    0001  --> default
            2    0010  --> CPU
            4    0100  --> GPU
            8    1000  --> ACC

            3    0011  --> CPU+default  --> 0010
                                                 | (or)
                                            0001
                                           ------
                                            0011

            5   0101  --> GPU+default  --> 0100
                                                 | (or)
                                           0001
                                          ------
                                           0101


            Examples:

            if ( 0011 & 0010 ) return 0011 (CPU+default)
                                           & (and)
                                      0010 (type = CPU)
                                     ------
                                      0010 --> CPU

            if ( 0011 & 0100 ) return 0011 (CPU+default)
                                           & (and)
                                      0100 (type = GPU)
                                     ------
                                      0000 
           */

            if ( devices[n].getInfo<CL_DEVICE_TYPE>() & type ) 
            {
               leave = true;
               data.device = devices[n];
               break;
            }
         }
         if( leave ) break;
      }
   }

   if( type != CL_INVALID_DEVICE_TYPE && !leave )
   {
      fprintf(stderr, "Not a single compatible type found.\n");
      return 1;
   }

   data.context = cl::Context( devices );

   data.queue = cl::CommandQueue( data.context, data.device);

   return 0;
}

// -----------------------------------------------------------------------------
int build_kernel( int maxlocalsize )
{
   /* Use a prefix (the given label) to minimize the likelihood of collisions
    * when two or more problems are built into the same build directory.
      The directory of the executable binary is prefixed here so that OpenCL
      will find the kernels regardless of user's current directory. */
   //TODO
   //std::string opencl_file = data.executable_directory + std::string(std::string(xstr(LABEL)) + "-accelerator.cl");
   std::string opencl_file = data.executable_directory +  std::string("/follower/accelerator.cl");
   ifstream file(opencl_file.c_str());
   string kernel_str( istreambuf_iterator<char>(file), ( istreambuf_iterator<char>()) );

   string program_str;
   program_str = 
      "#define POPL_SIZE " + util::ToString( data.population_leader_size ) + "\n#define POPF_SIZE " + util::ToString( data.population_follower_size ) + "\n" +
      "#define DIML " + util::ToString( data.leader_dimension ) + "\n#define DIMF " + util::ToString( data.follower_dimension ) + "\n" +
      "#define GENF_NUM " + util::ToString( data.num_generation_follower ) + "\n#define CR " + util::ToString( data.crossover_rate ) + "\n" + 
      "#define F " + util::ToString( data.f ) + "\n#define r " + util::ToString( data.r ) + "\n" + 
      "#define p " + util::ToString( data.p ) + "\n#define q " + util::ToString( data.q ) + "\n#define s " + util::ToString( data.s ) + "\n" + 
      data.function_str + kernel_str;
   //cout << program_str << endl;

   cl::Program::Sources source( 1, make_pair( program_str.c_str(), program_str.size() ) );
   
   cl::Program program( data.context, source );

   vector<cl::Device> device; device.push_back( data.device );
   try 
   {
      /* Pass the following definition to the OpenCL compiler:
            -I<executable_absolute_directory>/INCLUDE_RELATIVE_DIR
         where <executable_absolute_directory> is the current directory
         of the executable binary and INCLUDE_RELATIVE_DIR is a relative
         subdirectory where the assembled source files will be put by CMake. */
      //TODO
      //std::string flags = std::string(" -I" + data.executable_directory + std::string(xstr(INCLUDE_RELATIVE_DIR)));
      std::string flags = std::string( " -I" + data.executable_directory + std::string("/follower") 
#ifdef CONFIG_USE_DOUBLE
      + " -DCONFIG_USE_DOUBLE " 
#endif
      + " -DVARIANT_" + util::ToLower( data.variant ) );
      program.build( device, flags.c_str() );
   }
   catch( cl::Error& e )
   {
      if( e.err() == CL_BUILD_PROGRAM_FAILURE )
      {
         cerr << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>( data.device ) << std::endl;
      }
      throw;
   }

   unsigned max_cu = data.device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
   unsigned max_local_size;
   if( maxlocalsize > 0 )
   {
      max_local_size = maxlocalsize;
   }
   else 
   {
      max_local_size = fmin( data.device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>(), data.device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0] );
   
      //It is necessary to respect the local memory size. Depending on the
      //maximum local size, there will not be enough space to allocate the
      //local variables.  The local size depends on the maximum local size. 
      //The division by 4: 1 local vector in the DP and PDP kernels (both are float vectors, so the division by 4 bytes)
      max_local_size = fmin( max_local_size, data.device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() / 4 );
   }

   if( data.population_follower_size < max_local_size )
   {
      data.local_size = data.population_follower_size;
   }
   else
   {
      data.local_size = max_local_size;
   }
   // One leader individual per work-group
   data.global_size = data.population_leader_size * data.local_size;
   data.kernel_seed     = cl::Kernel( program, "seed" );
   data.kernel_follower = cl::Kernel( program, "follower" );
   data.kernel_leader   = cl::Kernel( program, "leader" );

   if (data.verbose) {
      std::cout << "\nDevice: " << data.device.getInfo<CL_DEVICE_NAME>() << ", Compute units: " << max_cu << ", Max local size: " << max_local_size << std::endl;
      std::cout << "Local size: " << data.local_size << ", Global size: " << data.global_size << ", Work groups: " << data.global_size/data.local_size << std::endl;
   }

   return 0;
}

// -----------------------------------------------------------------------------

void create_buffers( int seed )
{
   // Buffer (memory on the device) of the programs
   data.seed_buffer = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.global_size * sizeof( uint ) );

   data.follower_buffer_popL = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * data.leader_dimension * sizeof( real_t ) );
   data.follower_buffer_popLValoresF = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * data.follower_dimension * sizeof( real_t ) );
   data.follower_buffer_popF = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_follower_size * data.follower_dimension * data.population_leader_size * sizeof( real_t ) );
   data.follower_buffer_vf = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * data.follower_dimension * sizeof( real_t ) );
   data.follower_buffer_vl = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * data.leader_dimension * sizeof( real_t ) );

   data.leader_buffer_fit_popL = cl::Buffer( data.context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * sizeof( real_t ) );
   data.leader_buffer_fit_popLValoresF = cl::Buffer( data.context, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, data.population_leader_size * sizeof( real_t ) );

   data.kernel_seed.setArg( 0, seed );
   data.kernel_seed.setArg( 1, data.seed_buffer );

   data.kernel_follower.setArg( 0, data.follower_buffer_popL );
   data.kernel_follower.setArg( 1, data.follower_buffer_popLValoresF );
   data.kernel_follower.setArg( 2, data.follower_buffer_popF );
   data.kernel_follower.setArg( 3, data.population_follower_size * data.follower_dimension * sizeof( real_t ), NULL );
   data.kernel_follower.setArg( 4, data.population_follower_size * sizeof( real_t ), NULL );
   data.kernel_follower.setArg( 5, data.population_follower_size * sizeof( int ), NULL );
   data.kernel_follower.setArg( 6, data.leader_dimension * sizeof( real_t ), NULL );
   data.kernel_follower.setArg( 7, data.seed_buffer );
   data.kernel_follower.setArg( 8, data.follower_buffer_vf );
   data.kernel_follower.setArg( 9, data.follower_buffer_vl );

   data.kernel_leader.setArg( 0, data.follower_buffer_popL );
   data.kernel_leader.setArg( 1, data.follower_buffer_popLValoresF );
   data.kernel_leader.setArg( 2, data.follower_buffer_vf );
   data.kernel_leader.setArg( 3, data.follower_buffer_vl );
   data.kernel_leader.setArg( 4, data.leader_buffer_fit_popL );
   data.kernel_leader.setArg( 5, data.leader_buffer_fit_popLValoresF );
}

// -----------------------------------------------------------------------------
int build_function()
{
   string header = 
  "\n// Check for the availability (and enable) of double precision support\n"
  "#ifdef CONFIG_USE_DOUBLE\n"
  "#if defined(cl_khr_fp64)\n"
  "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
  "#define DOUBLE_SUPPORT_AVAILABLE\n"
  "#elif defined(cl_amd_fp64)\n"
  "#pragma OPENCL EXTENSION cl_amd_fp64 : enable\n"
  "#define DOUBLE_SUPPORT_AVAILABLE\n"
  "#endif\n"
  "#endif /* CONFIG_USE_DOUBLE */\n\n"
  "#if defined(CONFIG_USE_DOUBLE)\n"
  "   #if ! defined(DOUBLE_SUPPORT_AVAILABLE)\n"
  "      #error ""The device does not support double""\n"
  "   #else\n"
  "      typedef double real_t;\n"
  "   #endif\n"
  "#else\n"
  "   typedef float real_t;\n"
  "#endif\n\n"
  "#define LCG_RAND_MAX 2147483647\n\n"
  "/* Generates a pseudo random number using a special case of Linear Congruential\n"
  "   Generator (Lehmer). For more information, please refer to:\n\n" 
  "   https://en.wikipedia.org/wiki/Lehmer_random_number_generator\n\n"
  "   seed must be:  0 < seed < 2147483647\n"
  "*/\n\n"
  "uint lcg( uint seed )\n"
  "{\n"
  "   ulong product = (ulong) seed * 48271;\n"
  "   uint x = (product & 0x7fffffff) + (product >> 31);\n"
  "   return (x & 0x7fffffff) + (x >> 31);\n"
  "}\n\n"
  "/* Returns a float random number in (0,1] */\n"
  "real_t Real( uint * seed )\n"
  "{\n"
  "   *seed = lcg( *seed );\n"
  "   return *seed / (real_t) LCG_RAND_MAX;\n"
  "}\n\n"
  "/* Returns an integer (unsigned) random number in [0,n) */\n"
  "uint Int( uint * seed, uint n )\n"
  "{\n"
  "   *seed = lcg( *seed );\n"
  "   return *seed % n;\n"
  "}\n\n";

   string function_getLower_level_1 =
  "real_t getLower_level_1(int indice)\n"
  "{\n"
  "   return -5.;\n"
  "}\n\n";

   string function_1004_getLower_level_1 =
  "real_t getLower_level_1(int indice)\n"
  "{\n"
  "   if (indice < p)\n"
  "   {\n"
  "       return -5.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "       return -1.;\n"
  "   }\n"
  "}\n\n";

   string function_1001_e_1003_getLower_level_2 =
  "real_t getLower_level_2( int indice )\n"
  "{\n"
  "   if (indice < q)\n"
  "   {\n"
  "     return -5.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "     return -(M_PI_F/2.) + 0.000000000001;\n"
  "   }\n"
  "}\n\n";

   string function_1002_e_1004_e_1007_getLower_level_2 =
  "real_t getLower_level_2( int indice )\n"
  "{\n"
  "   if (indice < q)\n"
  "   {\n"
  "     return -5.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "     return 0.000000000001;\n"
  "   }\n"
  "}\n\n";

   string function_1005_e_1006_e_1008_getLower_level_2 =
  "real_t getLower_level_2( int indice )\n"
  "{\n"
  "   return -5.;\n"
  "}\n\n";

   string function_getUpper_level_1 = 
  "real_t getUpper_level_1(int indice)\n"
  "{\n"
  "   return 10.;\n"
  "}\n\n";

   string function_1002_e_1004_e_1007_getUpper_level_1 = 
  "real_t getUpper_level_1( int indice )\n"
  "{\n"
  "   if (indice < p)\n"
  "   {\n"
  "       return 10.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "       return 1.;\n"
  "   }\n"
  "}\n\n";

   string function_1001_e_1003_getUpper_level_2 =
  "real_t getUpper_level_2( int indice )\n"
  "{\n"
  "   if (indice < q)\n"
  "   {\n"
  "       return 10.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "       return M_PI_F/2. - 0.000000000001;\n"
  "   }\n"
  "}\n\n";

   string function_1002_e_1004_e_1007_getUpper_level_2 =
  "real_t getUpper_level_2( int indice )\n"
  "{\n"
  "   if (indice < q)\n"
  "   {\n"
  "       return 10.;\n"
  "   } \n"
  "   else \n"
  "   {\n"
  "       return M_E_F;\n"
  "   }\n"
  "}\n\n";

   string function_1005_e_1006_e_1008_getUpper_level_2 =
  "real_t getUpper_level_2( int indice )\n"
  "{\n"
  "   return 10.;\n"
  "}\n\n";

   string function_evaluate_transpose_leader_level_1 = 
   "real_t evaluate_transpose_leader_level_1( int idx, global real_t* uL, global real_t* uF )\n";

   string function_evaluate_transpose_leader_level_2 = 
   "real_t evaluate_transpose_leader_level_2( int idx, global real_t* uL, global real_t* uF )\n";

   string function_evaluate_transpose_follower_level_1 = 
   "real_t evaluate_transpose_follower_level_1( int idx, local real_t* uL, local real_t* popF )\n";

   string function_evaluate_transpose_follower_level_2 = 
   "real_t evaluate_transpose_follower_level_2( int idx, local real_t* uL, local real_t* popF )\n";

   string header_evaluate_transpose_leader = 
   "{\n"
   "   real_t F1 = 0.0, F2 = 0.0, F3 = 0.0;\n\n"
   "   real_t x[DIML];\n"
   "   real_t y[DIMF];\n\n"
   "   for(int i = 0; i < DIML; i++)\n"
   "   {\n"
   "      x[i] = uL[idx + i * POPL_SIZE];\n"
   "   }\n"
   "   for(int i = 0; i < DIMF; i++)\n"
   "   {\n"
   "      y[i] = uF[idx + i * POPL_SIZE];\n"
   "   }\n";
   
   string header_evaluate_transpose_follower = 
   "{\n"
   "   real_t F1 = 0.0, F2 = 0.0, F3 = 0.0;\n\n"
   "   real_t x[DIML];\n"
   "   real_t y[DIMF];\n\n"
   "   for(int i = 0; i < DIML; i++)\n"
   "   {\n"
   "      x[i] = uL[i];\n"
   "   }\n"
   "   for(int i = 0; i < DIMF; i++)\n"
   "   {\n"
   "      y[i] = popF[idx + i * POPF_SIZE];\n"
   "   }\n";
   
   string function_1001_evaluate_level_1 =
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));\n"
   "   }\n"
   "   F3 = sum1+sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";
   
   string function_1001_evaluate_level_2 =
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((x[p+i]-tan(y[q+i]))*(x[p+i]-tan(y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1002_evaluate_level_1 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   F2 = F2*(-1);\n"
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += ((x[p+i]-log(y[q+i]))*(x[p+i]-log(y[q+i])));\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1002_evaluate_level_2 =
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((x[p+i]-log(y[q+i]))*(x[p+i]-log(y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1003_evaluate_level_1 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += (((x[p+i]*x[p+i])-tan(y[q+i]))*((x[p+i]*x[p+i])-tan(y[q+i])));\n"
   "   }\n"
   "   F3 = sum1+sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1003_evaluate_level_2 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += ((y[i]*y[i])-cos(2*M_PI_F*y[i]));\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += (((x[p+i]*x[p+i])-tan(y[q+i]))*((x[p+i]*x[p+i])-tan(y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1004_evaluate_level_1 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   F2 = F2*(-1);\n"
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += ((fabs(x[p+i])-log(1+y[q+i]))*(fabs(x[p+i])-log(1+y[q+i])));\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1004_evaluate_level_2 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   F2 = q;\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += ((y[i]*y[i])-cos(2*M_PI_F*y[i]));\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((fabs(x[p+i])-log(1+y[q+i]))*(fabs(x[p+i])-log(1+y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1005_evaluate_level_1 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (((y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i])))+((y[i]-1)*(y[i]-1)));\n"
   "      //F2 += ((y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "   }\n"
   "   F2 = F2*(-1);\n"
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += ((fabs(x[p+i])-(y[q+i]*y[q+i]))*(fabs(x[p+i])-(y[q+i]*y[q+i])));\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1005_evaluate_level_2 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q-1; i++)\n"
   "   {\n"
   "      F2 += (((y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i])))+((y[i]-1)*(y[i]-1)));\n"
   "      //F2 += ((y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((fabs(x[p+i])-(y[q+i]*y[q+i]))*(fabs(x[p+i])-(y[q+i]*y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1006_evaluate_level_1 = 
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      sum1 += (y[i]*y[i]);\n"
   "   }\n"
   "   for(int i = q; i < q+s; i++)\n"
   "   {\n"
   "      sum2 += (y[i]*y[i]);\n"
   "   }\n"
   "   F2 = sum2-sum1;\n"
   "   sum1 = 0.0; sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[p+i]*x[p+i]);\n"
   "      sum2 += ((x[p+i]-y[q+s+i])*(x[p+i]-y[q+s+i]));\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1006_evaluate_level_2 = 
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      sum1 += (y[i]*y[i]);\n"
   "   }\n"
   "   //O 'sum2' foi alterado da ref original\n"
   "   for(int i = q; i < q+s; i = i+2)\n"
   "   {\n"
   "      sum2 += ((y[i+1]-y[i])*(y[i+1]-y[i]));\n"
   "   }\n"
   "   F2 = sum1+sum2;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((x[p+i]-y[q+s+i])*(x[p+i]-y[q+s+i]));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   // TODO
   string function_1007_evaluate_level_1 = 
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   real_t sum1 = 0.0, sum2 = 0.0, prod = 1.0;\n"
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      sum1 += (x[i]*x[i]);\n"
   "      prod *= (cos(x[i]/sqrt((real_t)(i+1))));\n"
   "   }\n"
   "   F1 = 1. + ((1/400.)*sum1)-prod;\n"
   "   F2 = F2*(-1);\n"
   "   sum1 = 0.0; \n"
   "   int j = p, w = q;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += (x[j]*x[j]);\n"
   "      sum2 += ((x[j]-log(y[w]))*(x[j]-log(y[w])));\n"
   "      j++; w++;\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1007_evaluate_level_2 = 
   "   for(int i = 0; i < q; i++)\n"
   "   {\n"
   "      F2 += (y[i]*y[i]);\n"
   "   }\n"
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += (x[i]*x[i]*x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((x[p+i]-log(y[q+i]))*(x[p+i]-log(y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1008_evaluate_level_1 = 
   "   real_t sum1 = 0.0, sum2 = 0.0;\n"
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      sum1 += (x[i]*x[i]);\n"
   "      sum2 += (cos(2*M_PI_F*x[i]));\n"
   "   }\n"
   "   F1 = (20+M_E_F-(20*exp(-0.2*sqrt((1/p)*sum1)))-exp((1/p)*sum2));\n"
   "   for(int i = 0; i < q-1; i++)\n"
   "   {\n"
   "      F2 += ((y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "      //F2 += ((y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "   }\n"
   "   F2 = F2*(-1);\n"
   "   sum1 = 0.0; sum2 = 0.0;\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      sum1 += ((x[p+i]*x[p+i]));\n"
   "      sum2 += ((x[p+i]-(y[q+i]*y[q+i]*y[q+i]))*(x[p+i]-(y[q+i]*y[q+i]*y[q+i])));\n"
   "   }\n"
   "   F3 = sum1-sum2;\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";

   string function_1008_evaluate_level_2 = 
   "   for(int i = 0; i < p; i++)\n"
   "   {\n"
   "      F1 += fabs(x[i]);\n"
   "   }\n"
   "   for(int i = 0; i < q-1; i++)\n"
   "   {\n"
   "      F2 += ((y[i+1]-(y[i]*y[i]))*(y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "      //F2 += ((y[i+1]-(y[i]*y[i]))+((y[i]-1)*(y[i]-1)));\n"
   "   }\n"
   "   for(int i = 0; i < r; i++)\n"
   "   {\n"
   "      F3 += ((x[p+i]-(y[q+i]*y[q+i]*y[q+i]))*(x[p+i]-(y[q+i]*y[q+i]*y[q+i])));\n"
   "   }\n\n"
   "   return F1+F2+F3;\n"
   "}\n\n";


   if( data.function == str(1001) )
      data.function_str = header + function_getLower_level_1 + function_1001_e_1003_getLower_level_2 + function_getUpper_level_1 + function_1001_e_1003_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1001_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1001_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1001_evaluate_level_2;
   else if( data.function == str(1002) )
      data.function_str = header + function_getLower_level_1 + function_1002_e_1004_e_1007_getLower_level_2 + function_1002_e_1004_e_1007_getUpper_level_1 + function_1002_e_1004_e_1007_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1002_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1002_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1002_evaluate_level_2;
   else if( data.function == str(1003) )
      data.function_str = header + function_getLower_level_1 + function_1001_e_1003_getLower_level_2 + function_getUpper_level_1 + function_1001_e_1003_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1003_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1003_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1003_evaluate_level_2;
   else if( data.function == str(1004) )
      data.function_str = header + function_1004_getLower_level_1 + function_1002_e_1004_e_1007_getLower_level_2 + function_1002_e_1004_e_1007_getUpper_level_1 + function_1002_e_1004_e_1007_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1004_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1004_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1004_evaluate_level_2;

   else if( data.function == str(1005) )
      data.function_str = header + function_getLower_level_1 + function_1005_e_1006_e_1008_getLower_level_2 + function_getUpper_level_1 + function_1005_e_1006_e_1008_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1005_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1005_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1005_evaluate_level_2;
   else if( data.function == str(1006) )
      data.function_str = header + function_getLower_level_1 + function_1005_e_1006_e_1008_getLower_level_2 + function_getUpper_level_1 + function_1005_e_1006_e_1008_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1006_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1006_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1006_evaluate_level_2;
   else if( data.function == str(1007) )
      data.function_str = header + function_getLower_level_1 + function_1002_e_1004_e_1007_getLower_level_2 + function_1002_e_1004_e_1007_getUpper_level_1 + function_1002_e_1004_e_1007_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1007_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1007_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1007_evaluate_level_2;
   else if( data.function == str(1008) )
         data.function_str = header + function_getLower_level_1 + function_1005_e_1006_e_1008_getLower_level_2 + function_getUpper_level_1 + function_1005_e_1006_e_1008_getUpper_level_2 + function_evaluate_transpose_leader_level_1 + header_evaluate_transpose_leader + function_1008_evaluate_level_1 + function_evaluate_transpose_leader_level_2 + header_evaluate_transpose_leader + function_1008_evaluate_level_2 + function_evaluate_transpose_follower_level_2 + header_evaluate_transpose_follower + function_1008_evaluate_level_2;
   else
      fprintf(stderr, "Valid function: 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008 e 1009.\n");

   return 0;
}

/** ****************************************************************** **/
/** ************************* MAIN FUNCTION ************************** **/
/** ****************************************************************** **/

// -----------------------------------------------------------------------------
int acc_follower_init( int argc, char** argv, int r, int p, int q, int s )
{
   CmdLine::Parser Opts( argc, argv );

   // Get the executable directory so we can find the kernels later, regardless
   // of the current directory (user directory).
   data.executable_directory = getAbsoluteDirectory(argv[0]);

   Opts.Bool.Add( "-v", "--verbose" );

   Opts.Int.Add( "-cl-p", "--cl-platform-id", -1, 0 );
   Opts.Int.Add( "-cl-d", "--cl-device-id", -1, 0 );
   Opts.Int.Add( "-cl-mls", "--cl-max-local-size", -1 );
   Opts.String.Add( "-type" );

   Opts.Int.Add( "-seed", "", 0, 0, std::numeric_limits<long>::max() );

   Opts.Int.Add( "-gf", "--generation-follower", 10, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-pfs", "--population-follower-size", 64, 1, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-pls", "--population-leader-size", 64, 1, std::numeric_limits<int>::max() );

   Opts.Int.Add( "-dl", "--dimension-leader", 8, 0, std::numeric_limits<int>::max() );
   Opts.Int.Add( "-df", "--dimension-follower", 8, 0, std::numeric_limits<int>::max() );

   //TODO: checar valores com a Jaque
   Opts.Float.Add( "-cr", "--crossover-rate", 0.90, 0.0, 1.0 );
   Opts.Float.Add( "-f", "--constant-f", 0.90, 0.5, 1.0 );

   //TODO
   Opts.String.Add( "-variant", "", "RAND", "rand", "TARGET_TO_RAND", "target_to_rand", NULL );
   //Opts.String.Add( "-variant", "", "RAND", "rand", "BEST", "best", "TARGET_TO_RAND", "target_to_rand", "TARGET_TO_BEST", "target_to_best", NULL );
   Opts.String.Add( "-function", "", "", "1001", "1002", "1003", "1004", "1005", "1006", "1007", "1008", "1009", NULL );

   Opts.Process();

   data.verbose = Opts.Bool.Get("-v");

   data.num_generation_follower = Opts.Int.Get("-gf");
   data.population_follower_size = Opts.Int.Get("-pfs");

   data.population_leader_size = Opts.Int.Get("-pls");

   data.leader_dimension = Opts.Int.Get("-dl");
   data.follower_dimension = Opts.Int.Get("-df");

   data.crossover_rate = Opts.Float.Get("-cr");
   data.f = Opts.Float.Get("-f");

   data.variant = Opts.String.Get("-variant");
   data.function = Opts.String.Get("-function");

   data.r = r;
   data.p = p;
   data.q = q;
   data.s = s;

   cl_device_type type = CL_INVALID_DEVICE_TYPE;
   if( Opts.String.Found("-type") )
   {
      if( Opts.String.Get("-type") == "CPU" || Opts.String.Get("-type") == "cpu" )
      {
         type = CL_DEVICE_TYPE_CPU;
      }
      else 
      {
         if( Opts.String.Get("-type") == "GPU" || Opts.String.Get("-type") == "gpu" )
         {
            type = CL_DEVICE_TYPE_GPU;
         }
         else
         {
            fprintf(stderr, "Not a single compatible device found.\n");
            return 1;
         }
      }
   }

   if ( opencl_init( Opts.Int.Get("-cl-p"), Opts.Int.Get("-cl-d"), type ) )
   {
      fprintf(stderr,"Error in OpenCL initialization phase.\n");
      return 1;
   }

   if ( build_function() )
   {
      fprintf(stderr,"Error in build the function string.\n");
      return 1;
   }

   if ( build_kernel( Opts.Int.Get("-cl-mls") ) )
   {
      fprintf(stderr,"Error in build the kernel.\n");
      return 1;
   }

   int seed = Opts.Int.Get("-seed") == 0 ? time( NULL ) : Opts.Int.Get("-seed");
   create_buffers( seed );

   return 0;
}

// -----------------------------------------------------------------------------
void acc_seed()
{
   try 
   {
      // ---------- begin kernel execution
      data.queue.enqueueNDRangeKernel( data.kernel_seed, cl::NDRange(), cl::NDRange( data.global_size ), cl::NDRange(), NULL );
   }
   catch( cl::Error& e )
   {
      cerr << "\nERROR(kernel_seed): " << e.what() << " ( " << e.err() << " )\n";
      throw;
   }
   // Wait until the kernel has finished
   data.queue.finish();
}

// -----------------------------------------------------------------------------
void acc_follower( int initialization )
{
   data.kernel_follower.setArg( 10, initialization );

   try {
      // Begin kernel execution
      data.queue.enqueueNDRangeKernel( data.kernel_follower, cl::NDRange(), cl::NDRange( data.global_size ), cl::NDRange( data.local_size ), NULL );
   }
   catch( cl::Error& e )
   {
      cerr << "\nERROR(kernel_follower): " << e.what() << " ( " << e.err() << " )\n";
      throw;
   }
   // Wait until the kernel has finished
   data.queue.finish();
}

// -----------------------------------------------------------------------------
void acc_leader( real_t* fit_popL, real_t* fit_popLValoresF, int generation, real_t* popL, real_t* popLValoresF )
{
   data.kernel_leader.setArg( 6, generation );

   try {
      // Begin kernel execution
      data.queue.enqueueNDRangeKernel( data.kernel_leader, cl::NDRange(), cl::NDRange( data.population_leader_size ), cl::NDRange(), NULL );
   }
   catch( cl::Error& e )
   {
      cerr << "\nERROR(kernel_leader): " << e.what() << " ( " << e.err() << " )\n";
      throw;
   }
   // Wait until the kernel has finished
   data.queue.finish();

   data.queue.enqueueReadBuffer( data.leader_buffer_fit_popL, CL_TRUE, 0, data.population_leader_size * sizeof( real_t ), fit_popL );
   data.queue.enqueueReadBuffer( data.leader_buffer_fit_popLValoresF, CL_TRUE, 0, data.population_leader_size * sizeof( real_t ), fit_popLValoresF );
   data.queue.enqueueReadBuffer( data.follower_buffer_popL, CL_TRUE, 0, data.population_leader_size * data.leader_dimension * sizeof( real_t ), popL );
   data.queue.enqueueReadBuffer( data.follower_buffer_popLValoresF, CL_TRUE, 0, data.population_leader_size * data.follower_dimension * sizeof( real_t ), popLValoresF );
}
