

#if defined(USE_PAPI)                                           // -----------------------------------------------------------
#include <papi.h>

#define PAPI_EVENTS_NUM 7
typedef struct {
  int   event_num;
  int   event_ratio;
  char *event_name;
} papi_myevent;

char * const event_names[] = {"TOT INS", "TOT CYC", "TOT LD INS", "L1 Dm", "L2 Dm", "TLB Dm", "CYC no ins completed"};
papi_myevent papi_events[PAPI_EVENTS_NUM]   = {\
  {PAPI_TOT_INS, -1, event_names[0]},
  {PAPI_TOT_CYC, -1, event_names[1]},
  {PAPI_LD_INS,  -1, event_names[2]},
  {PAPI_L1_DCM,   2, event_names[3]},
  {PAPI_L2_DCM,   2, event_names[4]},
  {PAPI_TLB_DM,   2, event_names[5]},
  {PAPI_STL_CCY,  1, event_names[6]}};

int       papi_SetEvents_num                = 0;                // how many events have been set 
int       papi_event_set[PAPI_EVENTS_NUM]   = {0};              // record whether each event has been set
int       papi_EventSet                     = PAPI_NULL;        // the handle for the events' set
long long papi_buffer[PAPI_EVENTS_NUM]      = {0};              // storage for the counters' values
long long papi_values[PAPI_EVENTS_NUM]      = {0};              // accumulate the counters' values

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
#define PAPI_WARN_EVENT( R, E, S1, S2 ) {				\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem  with PAPI (code %d) event %d arise at line %d: %s %s\n", \
	     (R), (E),  __LINE__, (S1), (S2)); fflush(stdout); }}

#define PAPI_CHECK_SETMULTIPLEX( V ) { if( (V) != PAPI_OK ) { switch( (V)) { \
      case PAPI_EINVAL: printf("invalid argument for setting up multiplexing;"); break; \
      case PAPI_ENOCMP: printf("specified eventset is not yet bound to a component"); break; \
      case PAPI_ENOEVST: printf("specified eventset does not exist"); break; \
      case PAPI_EISRUN: printf("specified eventset is counting"); break; \
      case PAPI_ENOMEM: printf("insufficient memory for multiplexing"); break;}}}

#define PAPI_ADD_EVENTS_to_SET { for ( int i = 0; i < PAPI_EVENTS_NUM; i++) { \
    retval = PAPI_query_event(papi_events[i].event_num);		\
    if ( retval == PAPI_OK ) {						\
      retval = PAPI_add_event(papi_EventSet, papi_events[i].event_num); \
      papi_SetEvents_num += (papi_event_set[i] = (retval == PAPI_OK));	\
      PAPI_WARN_EVENT(retval, papi_events[i].event_num, "adding event", papi_events[i].event_name);} else { \
      PAPI_WARN_EVENT(retval, papi_events[i].event_num,"querying event", papi_events[i].event_name)}  } }

#define PAPI_INIT {							\
    int retval = PAPI_library_init(PAPI_VER_CURRENT);			\
    if (retval != PAPI_VER_CURRENT)					\
      printf("wrong PAPI initialization: version %d instead of %d has been found\n", retval, PAPI_VER_CURRENT); \
    retval = PAPI_multiplex_init();					\
    if ( retval == PAPI_ENOSUPP ) {					\
      printf("multiplexing not supported\n"); } else			\
      PAPI_WARN( retval, "init multiplexing support");			\
    retval = PAPI_create_eventset(&papi_EventSet); PAPI_WARN(retval,"creating event set"); \
    retval = PAPI_assign_eventset_component(papi_EventSet, 0); PAPI_WARN(retval, "assign to component");\
    retval = PAPI_set_multiplex(papi_EventSet);				\
    PAPI_CHECK_SETMULTIPLEX( retval );					\
    PAPI_ADD_EVENTS_to_SET; }

// to use HIGH-LEVEL API
//#define PAPI_START_CNTR { int res = PAPI_start_counters(papi_events, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }
//#define PAPI_STOP_CNTR  { int res = PAPI_stop_counters(papi_values, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }

// to use NORMAL API
#define PAPI_START_CNTR { if(papi_SetEvents_num) {			\
      int retval = PAPI_start(papi_EventSet); PAPI_WARN(retval, "starting counters"); }}

#define PAPI_STOP_CNTR { if(papi_SetEvents_num) {			\
      int retval = PAPI_stop(papi_EventSet, papi_buffer);		\
      if( retval == PAPI_OK ) {						\
      for( int jj = 0; jj < papi_SetEvents_num; jj++)			\
	papi_values[jj] += papi_buffer[jj]; } else PAPI_WARN(retval, "reading counters"); }}


#define PAPI_WRITE_COUNTERS { printf("\tPAPI events:\n");		\
    int max_len = 0;							\
    for ( int i = 0; i < PAPI_EVENTS_NUM; i++ )				\
      if( papi_event_set[i] ) {						\
	int _len=strlen(papi_events[i].event_name);			\
	max_len = (max_len>=_len?max_len:_len);}			\
    for ( int i = 0; i < PAPI_EVENTS_NUM; i++ )				\
      if( papi_event_set[i] ) {						\
	printf("\t%*s  : %12lld ", max_len,				\
	       papi_events[i].event_name, papi_values[i]);		\
	if( papi_events[i].event_ratio >= 0 )				\
	  printf("(%5.3g)", (double)papi_values[i] / papi_values[papi_events[i].event_ratio]); \
	printf("\n");} }

#else                                                           // -----------------------------------------------------------

#define PAPI_INIT
#define PCHECK( e )
#define PAPI_CHECK_RES( R )
#define PAPI_START_CNTR
#define PAPI_STOP_CNTR
#define PAPI_WRITE_COUNTERS

#endif                                                          // -----------------------------------------------------------
