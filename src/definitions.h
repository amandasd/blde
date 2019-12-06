#define EPS 1e-10
#define alpha_leader 10e-3
#define alpha_follower 10e-4
#define nEval 10e7 
#define stag 10e3/2

#define CONFIG_USE_DOUBLE 1 

#if defined(CONFIG_USE_DOUBLE)
   typedef double real_t;
#else
   typedef float real_t;
#endif
