#define BUFHEADLEN 6
#define EVENTHEADLEN 3
#define CHANHEADLEN 9


/* different runtypes */

/* standard list mode */
#define STD_LM               0
#define STD_LM_RUNTASK     256
#define STD_LM_BUFFORMAT   256

/* compressed list mode */
#define COMP_LM              1
#define COMP_LM_RUNTASK    257
#define COMP_LM_BUFFORMAT  257

/* standard fast list mode */
#define STD_FLM              2
#define STD_FLM_RUNTASK    512
#define STD_FLM_BUFFORMAT  512

/* compressed fast list mode */
#define COMP_FLM             3
#define COMP_FLM_RUNTASK   513
#define COMP_FLM_BUFFORMAT 513

/* list mode - Compression 3 */
#define COMP3_LM             4
#define COMP3_LM_RUNTASK   259
#define COMP3_LM_BUFFORMAT 259

/* default run mode */
#define DEFAULT_RUNTASK   STD_LM_RUNTASK
#define DEFAULT_BUFFORMAT STD_LM_BUFFORMAT

/* XIA subevent type */
#define XIA_EVENT          666

#define XIA_STD_LM         (XIA_EVENT+STD_LM)
#define XIA_COMP_LM        (XIA_EVENT+COMP_LM)
#define XIA_STD_FLM         (XIA_EVENT+STD_FLM)
#define XIA_COMP_FLM        (XIA_EVENT+COMP_FLM)


