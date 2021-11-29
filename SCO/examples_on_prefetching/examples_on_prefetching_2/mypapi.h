

#if defined(USE_PAPI)                                           // -----------------------------------------------------------
#include <papi.h>

typedef unsigned long long int uLint;

typedef struct {
  int   event_num;
  int   event_ratio;
  int   set;
  char *event_name;
} papi_myevent;

#define PAPI_EVENTS_NUM 7
char * const event_names[] = {"TOT INS", "TOT CYC", "TOT LD INS", "L1 Dm", "L2 Dm", "TLB Dm", "CYC no ins completed"};

papi_myevent papi_events[PAPI_EVENTS_NUM] = {				\
					     {PAPI_TOT_INS, -1, 0, event_names[0]},
					     {PAPI_TOT_CYC, -1, 0, event_names[1]},
					     {PAPI_LD_INS,  -1, 0, event_names[2]},
					     {PAPI_L1_DCM,   2, 0, event_names[3]},
					     {PAPI_L2_DCM,   2, 0, event_names[4]},
					     {PAPI_TLB_DM,   2, 0, event_names[5]},
					     {PAPI_STL_CCY,  1, 0, event_names[6]} };

//int   papi_events[PAPI_EVENTS_NUM] = {PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_LD_INS, PAPI_L1_DCM, PAPI_L2_DCM, PAPI_TLB_DM, PAPI_STL_CCY };
int   papi_EventSet                = PAPI_NULL;             // the handle for the events' set
uLint papi_buffer[PAPI_EVENTS_NUM] = {0};                   // storage for the counters' values
uLint papi_values[PAPI_EVENTS_NUM] = {0};                   // accumulate the counters' values
long double papi_stddev_values[PAPI_EVENTS_NUM] = {0};      // accumulate stddev

                                                                // check that PAPI is OK, exit if not
#define PAPI_CHECK( R ) {						\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem with PAPI (code %d) arise at line %d\n",	\
	     (R), __LINE__);fflush(stdout); return (R); }}


                                                                // check that PAPI is OK,
                                                                // issue a warning if not with a
                                                                // provided message
#define PAPI_WARN( R, S ) {						\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem  with PAPI (code %d) arise at line %d: %s\n",	\
	     (R),  __LINE__, (S)); fflush(stdout); }}

                                                                // check that PAPI is OK about an event
                                                                // issue a warning if not with a
                                                                // provided message
#define PAPI_WARN_EVENT( R, E, S1, n ) {				\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem  with PAPI (code %d) : event %d arise at line %d: %s (%d)\n", \
	     (R), (E),  __LINE__, (S1), (n)); fflush(stdout); }}

#define PAPI_CHECK_SETMULTIPLEX( V ) { if( (V) != PAPI_OK ) { switch( (V)) { \
      case PAPI_EINVAL: printf("invalid argument for setting up multiplexing;"); break; \
      case PAPI_ENOCMP: printf("specified eventset is not yet bound to a component"); break; \
      case PAPI_ENOEVST: printf("specified eventset does not exist"); break; \
      case PAPI_EISRUN: printf("specified eventset is counting"); break; \
      case PAPI_ENOMEM: printf("insufficient memory for multiplexing"); break;}}}

#define PAPI_ADD_EVENTS_to_SET { for ( int i = 0; i < PAPI_EVENTS_NUM; i++) { \
      retval = PAPI_query_event(papi_events[i].event_num);		\
      if ( retval == PAPI_OK ) {					\
	retval = PAPI_add_event(papi_EventSet, papi_events[i].event_num); \
	PAPI_WARN_EVENT(retval, papi_events[i].event_num, "adding event", i); \
	papi_events[i].set = (retval == PAPI_OK);\
      } else {								\
	PAPI_WARN_EVENT(retval, papi_events[i].event_num,"querying event", i)}  } }

#define PAPI_INIT {							\
    int retval = PAPI_library_init(PAPI_VER_CURRENT);			\
    if (retval != PAPI_VER_CURRENT)					\
      printf("wrong PAPI initialization: version %d instead of %d has been found\n", retval, PAPI_VER_CURRENT); \
    retval = PAPI_create_eventset(&papi_EventSet); PAPI_WARN(retval,"creating event set"); \
    retval = PAPI_assign_eventset_component(papi_EventSet, 0); PAPI_WARN(retval, "assign to component"); \
    retval = PAPI_set_multiplex(papi_EventSet);				\
    PAPI_CHECK_SETMULTIPLEX( retval );					\
    PAPI_ADD_EVENTS_to_SET; }

// to use HIGH-LEVEL API
//#define PAPI_START_CNTR { int res = PAPI_start_counters(papi_events, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }
//#define PAPI_STOP_CNTR  { int res = PAPI_stop_counters(papi_values, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }

// to use NORMAL API
#define PAPI_START_CNTR {						\
    int retval = PAPI_start(papi_EventSet); PAPI_WARN(retval, "starting counters"); }

#define PAPI_STOP_CNTR {						\
int retval = PAPI_stop(papi_EventSet, papi_buffer);			\
if( retval == PAPI_OK ) {						\
  for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++) {			\
    papi_values[jj] += papi_buffer[jj];					\
    papi_stddev_values[jj] += (long double)papi_buffer[jj]*papi_buffer[jj];}} \
 else PAPI_WARN(retval, "reading counters"); }

#define PAPI_GET_CNTR( i ) ( papi_values[(i)] )

#define PAPI_FLUSH_BUFFER {				\
    for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++)	\
      papi_buffer[ jj] = 0; }

#define PAPI_FLUSH {					\
    for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++)	\
      papi_values[jj] = papi_buffer[ jj] = 0; }


#define PAPI_WRITE_COUNTERS { printf("\tPAPI events:\n");		\
    int max_len = 0;							\
    for ( int i = 0; i < PAPI_EVENTS_NUM; i++ )				\
      if( papi_events[i].set ) {					\
	int _len=strlen(papi_events[i].event_name);			\
	max_len = (max_len>=_len?max_len:_len);}			\
    for ( int i = 0; i < PAPI_EVENTS_NUM; i++ )				\
      if( papi_events[i].set ) {					\
	printf("\t%*s  : %12lld ", max_len,				\
	       papi_events[i].event_name, papi_values[i]);		\
	if( papi_events[i].event_ratio >= 0 )				\
	  printf("(%5.3g)", (double)papi_values[i] / papi_values[papi_events[i].event_ratio]); \
	printf("\n");} }


#else                                                           // -----------------------------------------------------------

#define PAPI_EVENTS_NUM 0
#define PAPI_INIT
#define PAPI_START_CNTR
#define PAPI_STOP_CNTR
#define PAPI_FLUSH
#define PAPI_GET_CNTR( i ) 0
#define PAPI_WRITE_COUNTERS

#endif                                                          // -----------------------------------------------------------
