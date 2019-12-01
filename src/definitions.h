#define EPS 1e-10
#define alpha 0.000001

#define CONFIG_USE_DOUBLE 1 

#if defined(CONFIG_USE_DOUBLE)
   typedef double real_t;
#else
   typedef float real_t;
#endif
