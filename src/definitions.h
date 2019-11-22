#define EPS 1e-10
#define alpha 0.01

#define CONFIG_USE_DOUBLE 1 

#if defined(CONFIG_USE_DOUBLE)
   typedef double real_t;
#else
   typedef float real_t;
#endif
