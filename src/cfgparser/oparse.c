/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "src/cfgparser/oparse.y"


/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas Tonnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */


#include "olsrd_conf.h"
#include "../defs.h"
#include "../ipcalc.h"
#include "../net_olsr.h"
#include "../link_set.h"
#include "../olsr.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define PARSER_DEBUG 1

#if defined PARSER_DEBUG && PARSER_DEBUG
#define PARSER_DEBUG_PRINTF(x, args...)   printf(x, ##args)
#else
#define PARSER_DEBUG_PRINTF(x, args...)   do { } while (0)
#endif

#define SET_IFS_CONF(ifs, ifcnt, field, value) do { \
	for (; ifcnt>0; ifs=ifs->next, ifcnt--) { \
    ifs->cnfi->field = (value); \
    ifs->cnf->field = (value); \
	} \
} while (0)

#define YYSTYPE struct conf_token *

void yyerror(const char *);
int yylex(void);

static int ifs_in_curr_cfg = 0;

static int add_ipv6_addr(YYSTYPE ipaddr_arg, YYSTYPE prefixlen_arg);

static int lq_mult_helper(YYSTYPE ip_addr_arg, YYSTYPE mult_arg)
{
  union olsr_ip_addr addr;
  int i;
  struct olsr_if *walker;

#if defined PARSER_DEBUG && PARSER_DEBUG > 0
  printf("\tLinkQualityMult %s %0.2f\n",
         (ip_addr_arg != NULL) ? ip_addr_arg->string : "any",
         (double)mult_arg->floating);
#endif

  memset(&addr, 0, sizeof(addr));

  if (ip_addr_arg != NULL &&
     inet_pton(olsr_cnf->ip_version, ip_addr_arg->string, &addr) <= 0) {
    fprintf(stderr, "Cannot parse IP address %s.\n", ip_addr_arg->string);
    return -1;
  }

  walker = olsr_cnf->interfaces;

  for (i = 0; i < ifs_in_curr_cfg; i++) {
    struct olsr_lq_mult *mult = malloc(sizeof(*mult));
    if (mult == NULL) {
      fprintf(stderr, "Out of memory (LQ multiplier).\n");
      return -1;
    }

    mult->addr = addr;
    mult->value = (uint32_t)(mult_arg->floating * LINK_LOSS_MULTIPLIER);

    mult->next = walker->cnf->lq_mult;
    walker->cnfi->lq_mult = walker->cnf->lq_mult = mult;
    walker->cnf->orig_lq_mult_cnt++;
    walker->cnfi->orig_lq_mult_cnt=walker->cnf->orig_lq_mult_cnt;

    walker = walker->next;
  }

  if (ip_addr_arg != NULL) {
    free(ip_addr_arg->string);
    free(ip_addr_arg);
  }

  free(mult_arg);

  return 0;
}

static int add_ipv6_addr(YYSTYPE ipaddr_arg, YYSTYPE prefixlen_arg)
{
  union olsr_ip_addr ipaddr;
  PARSER_DEBUG_PRINTF("HNA IPv6 entry: %s/%d\n", ipaddr_arg->string, prefixlen_arg->integer);

  if (olsr_cnf->ip_version != AF_INET6) {
    fprintf(stderr, "IPv6 addresses can only be used if \"IpVersion\" == 6, skipping HNA6.\n");
    olsr_startup_sleep(3);
  }
	else {
	  if(inet_pton(AF_INET6, ipaddr_arg->string, &ipaddr) <= 0) {
      fprintf(stderr, "ihna6entry: Failed converting IP address %s\n", ipaddr_arg->string);
      return 1;
    }

		if (prefixlen_arg->integer > 128) {
			fprintf(stderr, "ihna6entry: Illegal IPv6 prefix length %d\n", prefixlen_arg->integer);
			return 1;
		}

		/* Queue */
		ip_prefix_list_add(&olsr_cnf->hna_entries, &ipaddr, prefixlen_arg->integer);
	}
  free(ipaddr_arg->string);
  free(ipaddr_arg);
  free(prefixlen_arg);

  return 0;
}



/* Line 268 of yacc.c  */
#line 238 "src/cfgparser/oparse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_SLASH = 258,
     TOK_OPEN = 259,
     TOK_CLOSE = 260,
     TOK_STRING = 261,
     TOK_INTEGER = 262,
     TOK_FLOAT = 263,
     TOK_BOOLEAN = 264,
     TOK_IPV6TYPE = 265,
     TOK_DEBUGLEVEL = 266,
     TOK_IPVERSION = 267,
     TOK_HNA4 = 268,
     TOK_HNA6 = 269,
     TOK_PLUGIN = 270,
     TOK_INTERFACE_DEFAULTS = 271,
     TOK_INTERFACE = 272,
     TOK_NOINT = 273,
     TOK_TOS = 274,
     TOK_OLSRPORT = 275,
     TOK_RTPROTO = 276,
     TOK_RTTABLE = 277,
     TOK_RTTABLE_DEFAULT = 278,
     TOK_RTTABLE_TUNNEL = 279,
     TOK_RTTABLE_PRIORITY = 280,
     TOK_RTTABLE_DEFAULTOLSR_PRIORITY = 281,
     TOK_RTTABLE_TUNNEL_PRIORITY = 282,
     TOK_RTTABLE_DEFAULT_PRIORITY = 283,
     TOK_WILLINGNESS = 284,
     TOK_IPCCON = 285,
     TOK_FIBMETRIC = 286,
     TOK_USEHYST = 287,
     TOK_HYSTSCALE = 288,
     TOK_HYSTUPPER = 289,
     TOK_HYSTLOWER = 290,
     TOK_POLLRATE = 291,
     TOK_NICCHGSPOLLRT = 292,
     TOK_TCREDUNDANCY = 293,
     TOK_MPRCOVERAGE = 294,
     TOK_LQ_LEVEL = 295,
     TOK_LQ_FISH = 296,
     TOK_LQ_AGING = 297,
     TOK_LQ_PLUGIN = 298,
     TOK_LQ_NAT_THRESH = 299,
     TOK_LQ_MULT = 300,
     TOK_CLEAR_SCREEN = 301,
     TOK_PLPARAM = 302,
     TOK_MIN_TC_VTIME = 303,
     TOK_LOCK_FILE = 304,
     TOK_USE_NIIT = 305,
     TOK_SMART_GW = 306,
     TOK_SMART_GW_ALWAYS_REMOVE_SERVER_TUNNEL = 307,
     TOK_SMART_GW_USE_COUNT = 308,
     TOK_SMART_GW_TAKEDOWN_PERCENTAGE = 309,
     TOK_SMART_GW_POLICYROUTING_SCRIPT = 310,
     TOK_SMART_GW_EGRESS_IFS = 311,
     TOK_SMART_GW_OFFSET_TABLES = 312,
     TOK_SMART_GW_OFFSET_RULES = 313,
     TOK_SMART_GW_ALLOW_NAT = 314,
     TOK_SMART_GW_PERIOD = 315,
     TOK_SMART_GW_STABLECOUNT = 316,
     TOK_SMART_GW_THRESH = 317,
     TOK_SMART_GW_WEIGHT_EXITLINK_UP = 318,
     TOK_SMART_GW_WEIGHT_EXITLINK_DOWN = 319,
     TOK_SMART_GW_WEIGHT_ETX = 320,
     TOK_SMART_GW_DIVIDER_ETX = 321,
     TOK_SMART_GW_UPLINK = 322,
     TOK_SMART_GW_UPLINK_NAT = 323,
     TOK_SMART_GW_SPEED = 324,
     TOK_SMART_GW_PREFIX = 325,
     TOK_SRC_IP_ROUTES = 326,
     TOK_MAIN_IP = 327,
     TOK_SET_IPFORWARD = 328,
     TOK_HOSTLABEL = 329,
     TOK_NETLABEL = 330,
     TOK_MAXIPC = 331,
     TOK_IFMODE = 332,
     TOK_IPV4MULTICAST = 333,
     TOK_IP4BROADCAST = 334,
     TOK_IPV4BROADCAST = 335,
     TOK_IPV6MULTICAST = 336,
     TOK_IPV4SRC = 337,
     TOK_IPV6SRC = 338,
     TOK_IFWEIGHT = 339,
     TOK_HELLOINT = 340,
     TOK_HELLOVAL = 341,
     TOK_TCINT = 342,
     TOK_TCVAL = 343,
     TOK_MIDINT = 344,
     TOK_MIDVAL = 345,
     TOK_HNAINT = 346,
     TOK_HNAVAL = 347,
     TOK_AUTODETCHG = 348,
     TOK_IPV4_ADDR = 349,
     TOK_IPV6_ADDR = 350,
     TOK_DEFAULT = 351,
     TOK_AUTO = 352,
     TOK_NONE = 353,
     TOK_COMMENT = 354
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 379 "src/cfgparser/oparse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNRULES -- Number of states.  */
#define YYNSTATES  323

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   354

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    50,    52,    54,    56,    58,
      60,    62,    64,    66,    68,    70,    72,    74,    76,    78,
      80,    82,    84,    86,    88,    90,    92,    94,    96,    98,
     100,   102,   104,   106,   108,   110,   112,   114,   116,   118,
     120,   122,   125,   128,   131,   134,   137,   140,   144,   145,
     148,   150,   152,   156,   157,   160,   162,   164,   168,   169,
     172,   174,   176,   178,   180,   183,   184,   187,   191,   195,
     196,   199,   201,   203,   205,   208,   211,   214,   216,   218,
     220,   222,   224,   226,   228,   230,   232,   234,   236,   238,
     240,   244,   245,   248,   250,   252,   254,   257,   260,   264,
     269,   272,   275,   277,   280,   283,   288,   291,   294,   297,
     300,   303,   306,   309,   312,   315,   318,   322,   326,   330,
     333,   336,   339,   342,   346,   349,   353,   355,   357,   360,
     363,   366,   369,   372,   375,   378,   381,   384,   387,   390,
     393,   396,   399,   402,   405,   408,   411,   414,   417,   420,
     423,   426,   429,   432,   435,   438,   441,   444,   447,   450,
     453,   456,   459,   462,   465,   468,   471,   474,   477,   480,
     483,   486,   489,   492,   495,   496,   499,   501,   504,   507,
     510,   513,   516,   519,   522,   525,   528,   531,   534,   538,
     541,   545,   550,   553,   556,   559,   562,   565,   569
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     101,     0,    -1,    -1,   101,   103,    -1,   101,   102,    -1,
     142,    -1,   143,    -1,   144,    -1,   149,    -1,   150,    -1,
     151,    -1,   152,    -1,   153,    -1,   154,    -1,   155,    -1,
     156,    -1,   159,    -1,   158,    -1,   157,    -1,   160,    -1,
     161,    -1,   162,    -1,   163,    -1,   164,    -1,   165,    -1,
     166,    -1,   167,    -1,   168,    -1,   169,    -1,   174,    -1,
     170,    -1,   175,    -1,   171,    -1,   176,    -1,   205,    -1,
     172,    -1,   173,    -1,   177,    -1,   178,    -1,   179,    -1,
     180,    -1,   181,    -1,   182,    -1,   186,    -1,   187,    -1,
     188,    -1,   189,    -1,   190,    -1,   191,    -1,   192,    -1,
     193,    -1,   194,    -1,   195,    -1,   196,    -1,   198,    -1,
     197,    -1,   199,    -1,   200,    -1,   201,    -1,   202,    -1,
     183,    -1,    13,   104,    -1,    14,   107,    -1,    30,   110,
      -1,   122,   116,    -1,   113,   115,    -1,   203,   119,    -1,
       4,   105,     5,    -1,    -1,   105,   106,    -1,   205,    -1,
     145,    -1,     4,   108,     5,    -1,    -1,   108,   109,    -1,
     205,    -1,   146,    -1,     4,   111,     5,    -1,    -1,   111,
     112,    -1,   205,    -1,   123,    -1,   124,    -1,   125,    -1,
     147,   114,    -1,    -1,   114,   148,    -1,     4,   117,     5,
      -1,     4,   117,     5,    -1,    -1,   117,   118,    -1,   205,
      -1,   126,    -1,   127,    -1,    79,   128,    -1,    80,   128,
      -1,    78,   128,    -1,   129,    -1,   130,    -1,   131,    -1,
     132,    -1,   133,    -1,   134,    -1,   135,    -1,   136,    -1,
     137,    -1,   138,    -1,   139,    -1,   140,    -1,   141,    -1,
       4,   120,     5,    -1,    -1,   120,   121,    -1,   204,    -1,
     205,    -1,    16,    -1,    76,     7,    -1,    74,    94,    -1,
      75,    94,    94,    -1,    75,    94,     3,     7,    -1,    84,
       7,    -1,    77,     6,    -1,    94,    -1,    81,    95,    -1,
      82,    94,    -1,    83,    95,     3,     7,    -1,    83,    95,
      -1,    85,     8,    -1,    86,     8,    -1,    87,     8,    -1,
      88,     8,    -1,    89,     8,    -1,    90,     8,    -1,    91,
       8,    -1,    92,     8,    -1,    93,     9,    -1,    45,    96,
       8,    -1,    45,    94,     8,    -1,    45,    95,     8,    -1,
      11,     7,    -1,    12,     7,    -1,    31,     6,    -1,    94,
      94,    -1,    94,     3,     7,    -1,    95,     7,    -1,    95,
       3,     7,    -1,    17,    -1,     6,    -1,    18,     9,    -1,
      19,     7,    -1,    20,     7,    -1,    21,     7,    -1,    22,
       7,    -1,    22,    97,    -1,    23,     7,    -1,    23,    97,
      -1,    24,     7,    -1,    24,    97,    -1,    25,     7,    -1,
      25,    97,    -1,    25,    98,    -1,    28,     7,    -1,    28,
      97,    -1,    28,    98,    -1,    27,     7,    -1,    27,    97,
      -1,    27,    98,    -1,    26,     7,    -1,    26,    97,    -1,
      26,    98,    -1,    29,     7,    -1,    32,     9,    -1,    33,
       8,    -1,    34,     8,    -1,    35,     8,    -1,    36,     8,
      -1,    37,     8,    -1,    38,     7,    -1,    39,     7,    -1,
      40,     7,    -1,    41,     7,    -1,    42,     8,    -1,    48,
       8,    -1,    49,     6,    -1,    43,     6,    -1,    44,     8,
      -1,    46,     9,    -1,    50,     9,    -1,    51,     9,    -1,
      52,     9,    -1,    53,     7,    -1,    54,     7,    -1,    55,
       6,    -1,    56,   184,    -1,    -1,   184,   185,    -1,     6,
      -1,    57,     7,    -1,    58,     7,    -1,    59,     9,    -1,
      60,     7,    -1,    61,     7,    -1,    62,     7,    -1,    63,
       7,    -1,    64,     7,    -1,    65,     7,    -1,    66,     7,
      -1,    67,     6,    -1,    69,     7,     7,    -1,    68,     9,
      -1,    70,    95,     7,    -1,    70,    95,     3,     7,    -1,
      71,     9,    -1,    72,    94,    -1,    72,    95,    -1,    73,
       9,    -1,    15,     6,    -1,    47,     6,     6,    -1,    99,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   274,   274,   275,   276,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   337,   338,   339,   340,   341,   342,   345,   348,   348,
     351,   352,   355,   358,   358,   361,   362,   365,   368,   368,
     371,   372,   373,   374,   377,   380,   380,   383,   386,   400,
     400,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     424,   427,   427,   430,   431,   434,   472,   479,   496,   519,
     543,   564,   580,   600,   620,   640,   663,   684,   696,   708,
     720,   732,   745,   757,   769,   781,   794,   801,   808,   816,
     825,   845,   865,   899,   935,   941,   949,   956,  1015,  1023,
    1032,  1040,  1048,  1054,  1062,  1068,  1076,  1082,  1090,  1096,
    1102,  1110,  1116,  1122,  1130,  1136,  1142,  1150,  1156,  1162,
    1170,  1179,  1187,  1195,  1203,  1211,  1219,  1227,  1235,  1243,
    1251,  1259,  1267,  1275,  1282,  1290,  1298,  1306,  1314,  1322,
    1330,  1338,  1346,  1354,  1357,  1357,  1360,  1434,  1442,  1450,
    1458,  1466,  1474,  1482,  1490,  1498,  1506,  1514,  1537,  1547,
    1555,  1567,  1581,  1593,  1605,  1618,  1627,  1673,  1696
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_SLASH", "TOK_OPEN", "TOK_CLOSE",
  "TOK_STRING", "TOK_INTEGER", "TOK_FLOAT", "TOK_BOOLEAN", "TOK_IPV6TYPE",
  "TOK_DEBUGLEVEL", "TOK_IPVERSION", "TOK_HNA4", "TOK_HNA6", "TOK_PLUGIN",
  "TOK_INTERFACE_DEFAULTS", "TOK_INTERFACE", "TOK_NOINT", "TOK_TOS",
  "TOK_OLSRPORT", "TOK_RTPROTO", "TOK_RTTABLE", "TOK_RTTABLE_DEFAULT",
  "TOK_RTTABLE_TUNNEL", "TOK_RTTABLE_PRIORITY",
  "TOK_RTTABLE_DEFAULTOLSR_PRIORITY", "TOK_RTTABLE_TUNNEL_PRIORITY",
  "TOK_RTTABLE_DEFAULT_PRIORITY", "TOK_WILLINGNESS", "TOK_IPCCON",
  "TOK_FIBMETRIC", "TOK_USEHYST", "TOK_HYSTSCALE", "TOK_HYSTUPPER",
  "TOK_HYSTLOWER", "TOK_POLLRATE", "TOK_NICCHGSPOLLRT", "TOK_TCREDUNDANCY",
  "TOK_MPRCOVERAGE", "TOK_LQ_LEVEL", "TOK_LQ_FISH", "TOK_LQ_AGING",
  "TOK_LQ_PLUGIN", "TOK_LQ_NAT_THRESH", "TOK_LQ_MULT", "TOK_CLEAR_SCREEN",
  "TOK_PLPARAM", "TOK_MIN_TC_VTIME", "TOK_LOCK_FILE", "TOK_USE_NIIT",
  "TOK_SMART_GW", "TOK_SMART_GW_ALWAYS_REMOVE_SERVER_TUNNEL",
  "TOK_SMART_GW_USE_COUNT", "TOK_SMART_GW_TAKEDOWN_PERCENTAGE",
  "TOK_SMART_GW_POLICYROUTING_SCRIPT", "TOK_SMART_GW_EGRESS_IFS",
  "TOK_SMART_GW_OFFSET_TABLES", "TOK_SMART_GW_OFFSET_RULES",
  "TOK_SMART_GW_ALLOW_NAT", "TOK_SMART_GW_PERIOD",
  "TOK_SMART_GW_STABLECOUNT", "TOK_SMART_GW_THRESH",
  "TOK_SMART_GW_WEIGHT_EXITLINK_UP", "TOK_SMART_GW_WEIGHT_EXITLINK_DOWN",
  "TOK_SMART_GW_WEIGHT_ETX", "TOK_SMART_GW_DIVIDER_ETX",
  "TOK_SMART_GW_UPLINK", "TOK_SMART_GW_UPLINK_NAT", "TOK_SMART_GW_SPEED",
  "TOK_SMART_GW_PREFIX", "TOK_SRC_IP_ROUTES", "TOK_MAIN_IP",
  "TOK_SET_IPFORWARD", "TOK_HOSTLABEL", "TOK_NETLABEL", "TOK_MAXIPC",
  "TOK_IFMODE", "TOK_IPV4MULTICAST", "TOK_IP4BROADCAST",
  "TOK_IPV4BROADCAST", "TOK_IPV6MULTICAST", "TOK_IPV4SRC", "TOK_IPV6SRC",
  "TOK_IFWEIGHT", "TOK_HELLOINT", "TOK_HELLOVAL", "TOK_TCINT", "TOK_TCVAL",
  "TOK_MIDINT", "TOK_MIDVAL", "TOK_HNAINT", "TOK_HNAVAL", "TOK_AUTODETCHG",
  "TOK_IPV4_ADDR", "TOK_IPV6_ADDR", "TOK_DEFAULT", "TOK_AUTO", "TOK_NONE",
  "TOK_COMMENT", "$accept", "conf", "stmt", "block", "hna4body",
  "hna4stmts", "hna4stmt", "hna6body", "hna6stmts", "hna6stmt", "ipcbody",
  "ipcstmts", "ipcstmt", "ifblock", "ifnicks", "ifbody", "ifdbody",
  "ifstmts", "ifstmt", "plbody", "plstmts", "plstmt", "ifdblock",
  "imaxipc", "ipchost", "ipcnet", "iifweight", "isetifmode", "isetipv4mc",
  "isetipv6mc", "isetipv4src", "isetipv6src", "isethelloint",
  "isethelloval", "isettcint", "isettcval", "isetmidint", "isetmidval",
  "isethnaint", "isethnaval", "isetautodetchg", "isetlqmult", "idebug",
  "iipversion", "fibmetric", "ihna4entry", "ihna6entry", "ifstart",
  "ifnick", "bnoint", "atos", "aolsrport", "irtproto", "irttable",
  "irttable_default", "irttable_tunnel", "irttable_priority",
  "irttable_default_priority", "irttable_tunnel_priority",
  "irttable_defaultolsr_priority", "awillingness", "busehyst",
  "fhystscale", "fhystupper", "fhystlower", "fpollrate", "fnicchgspollrt",
  "atcredundancy", "amprcoverage", "alq_level", "alq_fish", "alq_aging",
  "amin_tc_vtime", "alock_file", "alq_plugin", "anat_thresh",
  "bclear_screen", "suse_niit", "bsmart_gw",
  "bsmart_gw_always_remove_server_tunnel", "ismart_gw_use_count",
  "ismart_gw_takedown_percentage", "ssmart_gw_policyrouting_script",
  "ssgw_egress_ifs", "sgw_egress_ifs", "sgw_egress_if",
  "ismart_gw_offset_tables", "ismart_gw_offset_rules",
  "bsmart_gw_allow_nat", "ismart_gw_period", "asmart_gw_stablecount",
  "asmart_gw_thresh", "asmart_gw_weight_exitlink_up",
  "asmart_gw_weight_exitlink_down", "asmart_gw_weight_etx",
  "asmart_gw_divider_etx", "ssmart_gw_uplink", "ismart_gw_speed",
  "bsmart_gw_uplink_nat", "ismart_gw_prefix", "bsrc_ip_routes", "amain_ip",
  "bset_ipforward", "plblock", "plparam", "vcomment", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   101,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   103,   103,   103,   103,   103,   103,   104,   105,   105,
     106,   106,   107,   108,   108,   109,   109,   110,   111,   111,
     112,   112,   112,   112,   113,   114,   114,   115,   116,   117,
     117,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     119,   120,   120,   121,   121,   122,   123,   124,   125,   125,
     126,   127,   128,   129,   130,   131,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   141,   141,   142,
     143,   144,   145,   145,   146,   146,   147,   148,   149,   150,
     151,   152,   153,   153,   154,   154,   155,   155,   156,   156,
     156,   157,   157,   157,   158,   158,   158,   159,   159,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   199,   200,   201,   201,   202,   203,   204,   205
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     3,     0,     2,
       1,     1,     3,     0,     2,     1,     1,     3,     0,     2,
       1,     1,     1,     1,     2,     0,     2,     3,     3,     0,
       2,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     2,     1,     1,     1,     2,     2,     3,     4,
       2,     2,     1,     2,     2,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     2,
       2,     2,     2,     3,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
       3,     4,     2,     2,     2,     2,     2,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,   115,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   218,     4,     3,     0,     0,     5,
       6,     7,    85,     8,     9,    10,    11,    12,    13,    14,
      15,    18,    17,    16,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    30,    32,    35,    36,    29,    31,
      33,    37,    38,    39,    40,    41,    42,    60,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    55,
      54,    56,    57,    58,    59,     0,    34,   139,   140,    68,
      61,    73,    62,   216,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   167,   168,   169,
     164,   165,   166,   161,   162,   163,   170,    78,    63,   141,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   184,   185,   186,   182,   183,   187,   188,   189,   190,
     191,   192,   193,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   209,     0,     0,   212,   213,   214,
     215,    89,    65,    89,    64,    84,   111,    66,     0,     0,
       0,   196,   195,   208,     0,   210,     0,     0,   147,    86,
       0,    67,     0,    69,    71,    70,    72,     0,    74,    76,
      75,    77,     0,     0,     0,    79,    81,    82,    83,    80,
     211,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    92,    93,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    91,    88,   110,     0,
     112,   113,   114,     0,   142,     0,   144,   117,     0,   116,
       0,     0,     0,   121,   122,    96,    94,    95,   123,   124,
     126,   120,   127,   128,   129,   130,   131,   132,   133,   134,
     135,     0,   143,   145,     0,   118,   137,   138,   136,     0,
     217,   119,   125
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    65,    66,   130,   208,   223,   132,   209,   228,
     158,   210,   235,    67,   205,   202,   204,   216,   260,   207,
     220,   280,    68,   236,   237,   238,   261,   262,   295,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,    69,    70,    71,   224,   229,    72,   219,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   182,   212,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   281,   276
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -236
static const yytype_int16 yypact[] =
{
    -236,     1,  -236,   110,   111,   115,   116,   117,  -236,  -236,
     113,   118,   119,   120,   103,   106,   107,    90,    92,    97,
      99,   121,   125,   124,   122,   126,   127,   129,   130,   131,
     134,   135,   136,   137,   138,   139,   156,   123,   157,   160,
     158,   159,   161,   164,   165,   168,  -236,   169,   170,   166,
     171,   172,   173,   174,   175,   176,   177,   179,   182,   185,
      16,   184,   -89,   192,  -236,  -236,  -236,   198,   201,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,   202,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,   180,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,   203,   102,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,   205,  -236,  -236,     4,    41,
       2,  -236,  -236,  -236,   206,  -236,     3,    70,  -236,  -236,
      74,  -236,   104,  -236,  -236,  -236,  -236,   109,  -236,  -236,
    -236,  -236,    30,    39,   207,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,   -92,   209,   128,   128,   128,   132,   140,   133,
     210,   204,   208,   212,   213,   215,   216,   217,   218,   220,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,   224,
    -236,  -236,  -236,   211,  -236,   225,  -236,  -236,   105,  -236,
     223,   227,   228,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
     230,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,   231,  -236,  -236,   232,  -236,  -236,  -236,  -236,   233,
    -236,  -236,  -236
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,    35,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -235,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,    -1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     126,     2,   290,   291,   292,   198,   199,   231,   241,   221,
     296,   297,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,   226,    37,   242,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,   277,   232,   233,   234,   278,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   144,   222,   147,
      64,    64,    64,    64,   150,   214,   153,   283,   314,   215,
     138,   196,   285,   140,   142,   242,   286,   127,   128,   129,
     131,   279,   134,   133,   287,   135,   136,   137,   156,   157,
     159,   160,   173,   288,   161,   162,   227,   163,   164,   165,
      64,   166,   167,   168,   169,   171,   170,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   172,   174,   175,   176,   177,    64,
     178,   179,   180,    64,   181,   185,   183,   184,   186,   187,
     188,   189,   190,   191,   192,   193,   211,   145,   146,   148,
     149,   194,   195,   197,   151,   152,   154,   155,   284,   315,
     139,   200,   201,   141,   143,   203,   206,   225,   230,   239,
     213,   218,   302,   240,   289,   293,   303,   301,   312,   282,
     304,   305,   294,   306,   307,   308,   309,   298,   300,   310,
     311,   316,   313,   319,   299,   317,   318,   320,   217,   321,
     322
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-236))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
       1,     0,    94,    95,    96,    94,    95,     5,     5,     5,
     245,   246,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,     5,    46,    45,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     5,    74,    75,    76,     5,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,     7,    94,     7,
      99,    99,    99,    99,     7,     3,     7,     3,     3,     7,
       7,    95,     3,     7,     7,    45,     7,     7,     7,     4,
       4,    47,     9,     6,    94,     7,     7,     7,     7,     4,
       6,     9,     9,    94,     8,     8,    95,     8,     8,     8,
      99,     7,     7,     7,     7,     6,     8,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     8,     8,     6,     9,     9,    99,
       9,     7,     7,    99,     6,     9,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     6,     6,    97,    98,    97,
      98,     9,     7,     9,    97,    98,    97,    98,    94,    94,
      97,     9,     4,    97,    97,     4,     4,   208,   209,   210,
       7,     6,     8,     7,     7,     6,     8,     7,     7,   220,
       8,     8,    94,     8,     8,     8,     8,    95,    95,     9,
       6,     8,     7,     3,    94,     8,     8,     6,   203,     7,
       7
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   101,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    46,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    99,   102,   103,   113,   122,   142,
     143,   144,   147,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   205,     7,     7,     4,
     104,     4,   107,     6,     9,     7,     7,     7,     7,    97,
       7,    97,     7,    97,     7,    97,    98,     7,    97,    98,
       7,    97,    98,     7,    97,    98,     7,     4,   110,     6,
       9,     8,     8,     8,     8,     8,     7,     7,     7,     7,
       8,     6,     8,     9,     8,     6,     9,     9,     9,     7,
       7,     6,   184,     7,     7,     9,     7,     7,     7,     7,
       7,     7,     7,     6,     9,     7,    95,     9,    94,    95,
       9,     4,   115,     4,   116,   114,     4,   119,   105,   108,
     111,     6,   185,     7,     3,     7,   117,   117,     6,   148,
     120,     5,    94,   106,   145,   205,     5,    95,   109,   146,
     205,     5,    74,    75,    76,   112,   123,   124,   125,   205,
       7,     5,    45,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
     118,   126,   127,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   205,     5,     5,    47,
     121,   204,   205,     3,    94,     3,     7,    94,    94,     7,
      94,    95,    96,     6,    94,   128,   128,   128,    95,    94,
      95,     7,     8,     8,     8,     8,     8,     8,     8,     8,
       9,     6,     7,     7,     3,    94,     8,     8,     8,     3,
       6,     7,     7
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 88:

/* Line 1806 of yacc.c  */
#line 387 "src/cfgparser/oparse.y"
    {
  struct olsr_if *in = olsr_cnf->interfaces;
  printf("\nInterface Defaults");
  /*remove Interface Defaults from Interface list as they are no interface!*/
  olsr_cnf->interfaces = in->next;
  ifs_in_curr_cfg=0;
  /*free interface but keep its config intact?*/
  free(in->cnfi);
  free(in);

}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 435 "src/cfgparser/oparse.y"
    {
  struct olsr_if *in = malloc(sizeof(*in));

  if (in == NULL) {
    fprintf(stderr, "Out of memory(ADD IF)\n");
    YYABORT;
  }

  in->cnf = get_default_if_config();
  in->cnfi = get_default_if_config();

  if (in->cnf == NULL || in->cnfi == NULL) {
    fprintf(stderr, "Out of memory(ADD DEFIFRULE)\n");
    if (in->cnf) {
      free(in->cnf);
    }
    if (in->cnfi) {
      free(in->cnfi);
    }
    free(in);
    YYABORT;
  }

  //should not need a name any more, as we free it on "}" again
  //in->name = strdup(interface_defaults_name);

  olsr_cnf->interface_defaults = in->cnf;

  /* Queue */
  in->next = olsr_cnf->interfaces;
  olsr_cnf->interfaces = in;
  ifs_in_curr_cfg=1;
  
  fflush(stdout);
}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 473 "src/cfgparser/oparse.y"
    {
  olsr_cnf->ipc_connections = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 480 "src/cfgparser/oparse.y"
    {
  union olsr_ip_addr ipaddr;
  PARSER_DEBUG_PRINTF("\tIPC host: %s\n", (yyvsp[(2) - (2)])->string);
  
  if (inet_pton(AF_INET, (yyvsp[(2) - (2)])->string, &ipaddr.v4) == 0) {
    fprintf(stderr, "Failed converting IP address IPC %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }

  ip_prefix_list_add(&olsr_cnf->ipc_nets, &ipaddr, olsr_cnf->maxplen);

  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 497 "src/cfgparser/oparse.y"
    {
  union olsr_ip_addr ipaddr, netmask;

  PARSER_DEBUG_PRINTF("\tIPC net: %s/%s\n", (yyvsp[(2) - (3)])->string, (yyvsp[(3) - (3)])->string);
  
  if (inet_pton(AF_INET, (yyvsp[(2) - (3)])->string, &ipaddr.v4) == 0) {
    fprintf(stderr, "Failed converting IP net IPC %s\n", (yyvsp[(2) - (3)])->string);
    YYABORT;
  }

  if (inet_pton(AF_INET, (yyvsp[(3) - (3)])->string, &netmask.v4) == 0) {
    fprintf(stderr, "Failed converting IP mask IPC %s\n", (yyvsp[(3) - (3)])->string);
    YYABORT;
  }

  ip_prefix_list_add(&olsr_cnf->ipc_nets, &ipaddr, olsr_netmask_to_prefix(&netmask));

  free((yyvsp[(2) - (3)])->string);
  free((yyvsp[(2) - (3)]));
  free((yyvsp[(3) - (3)])->string);
  free((yyvsp[(3) - (3)]));
}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 520 "src/cfgparser/oparse.y"
    {
  union olsr_ip_addr ipaddr;

  PARSER_DEBUG_PRINTF("\tIPC net: %s/%s\n", (yyvsp[(2) - (4)])->string, (yyvsp[(3) - (4)])->string);
  
  if (inet_pton(AF_INET, (yyvsp[(2) - (4)])->string, &ipaddr.v4) == 0) {
    fprintf(stderr, "Failed converting IP net IPC %s\n", (yyvsp[(2) - (4)])->string);
    YYABORT;
  }

  if ((yyvsp[(4) - (4)])->integer > olsr_cnf->maxplen) {
    fprintf(stderr, "ipcnet: Prefix len %u > %d is not allowed!\n", (yyvsp[(4) - (4)])->integer, olsr_cnf->maxplen);
    YYABORT;
  }

  ip_prefix_list_add(&olsr_cnf->ipc_nets, &ipaddr, (yyvsp[(4) - (4)])->integer);

  free((yyvsp[(2) - (4)])->string);
  free((yyvsp[(2) - (4)]));
  free((yyvsp[(4) - (4)]));
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 544 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("Fixed willingness: %d\n", (yyvsp[(2) - (2)])->integer);

  while (ifcnt) {
    ifs->cnf->weight.value = (yyvsp[(2) - (2)])->integer;
    ifs->cnf->weight.fixed = true;
    ifs->cnfi->weight.value = (yyvsp[(2) - (2)])->integer;
    ifs->cnfi->weight.fixed = true;

    ifs = ifs->next;
    ifcnt--;
  }

  free((yyvsp[(2) - (2)]));
}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 565 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;
	int mode = (strcmp((yyvsp[(2) - (2)])->string, "ether") == 0)?IF_MODE_ETHER:IF_MODE_MESH;

  PARSER_DEBUG_PRINTF("\tMode: %s\n", (yyvsp[(2) - (2)])->string);

	SET_IFS_CONF(ifs, ifcnt, mode, mode);
	
  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 581 "src/cfgparser/oparse.y"
    {
  struct in_addr in;
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tIPv4 broadcast: %s\n", (yyvsp[(1) - (1)])->string);

  if (inet_pton(AF_INET, (yyvsp[(1) - (1)])->string, &in) == 0) {
    fprintf(stderr, "isetipv4br: Failed converting IP address %s\n", (yyvsp[(1) - (1)])->string);
    YYABORT;
  }

	SET_IFS_CONF(ifs, ifcnt, ipv4_multicast.v4, in);

  free((yyvsp[(1) - (1)])->string);
  free((yyvsp[(1) - (1)]));
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 601 "src/cfgparser/oparse.y"
    {
  struct in6_addr in6;
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tIPv6 multicast: %s\n", (yyvsp[(2) - (2)])->string);

  if (inet_pton(AF_INET6, (yyvsp[(2) - (2)])->string, &in6) <= 0) {
    fprintf(stderr, "isetipv6mc: Failed converting IP address %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }

	SET_IFS_CONF(ifs, ifcnt, ipv6_multicast.v6, in6);

  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 621 "src/cfgparser/oparse.y"
    {
  struct in_addr in;
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tIPv4 src: %s\n", (yyvsp[(2) - (2)])->string);

  if (inet_pton(AF_INET, (yyvsp[(2) - (2)])->string, &in) == 0) {
    fprintf(stderr, "isetipv4src: Failed converting IP address %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }

	SET_IFS_CONF(ifs, ifcnt, ipv4_src.v4, in);

  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 641 "src/cfgparser/oparse.y"
    {
  struct olsr_ip_prefix pr6;
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tIPv6 src prefix: %s/%d\n", (yyvsp[(2) - (4)])->string, (yyvsp[(4) - (4)])->integer);

  if (inet_pton(AF_INET6, (yyvsp[(2) - (4)])->string, &pr6.prefix.v6) <= 0) {
    fprintf(stderr, "isetipv6src: Failed converting IP address %s\n", (yyvsp[(2) - (4)])->string);
    YYABORT;
  }
  if ((yyvsp[(4) - (4)])->integer > 128) {
    fprintf(stderr, "isetipv6src: Illegal Prefixlength %d\n", (yyvsp[(4) - (4)])->integer);
    YYABORT;
  }
  pr6.prefix_len = (yyvsp[(4) - (4)])->integer;

	SET_IFS_CONF(ifs, ifcnt, ipv6_src, pr6);

  free((yyvsp[(2) - (4)])->string);
  free((yyvsp[(2) - (4)]));
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 664 "src/cfgparser/oparse.y"
    {
  struct olsr_ip_prefix pr6;
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tIPv6 src prefix: %s/%d\n", (yyvsp[(2) - (2)])->string, 128);

  if (inet_pton(AF_INET6, (yyvsp[(2) - (2)])->string, &pr6.prefix.v6) <= 0) {
    fprintf(stderr, "isetipv6src: Failed converting IP address %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }
  pr6.prefix_len = 128;

  SET_IFS_CONF(ifs, ifcnt, ipv6_src, pr6);

  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 685 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tHELLO interval: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);

	SET_IFS_CONF(ifs, ifcnt, hello_params.emission_interval, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 697 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tHELLO validity: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);

	SET_IFS_CONF(ifs, ifcnt, hello_params.validity_time, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 709 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tTC interval: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);

	SET_IFS_CONF(ifs, ifcnt, tc_params.emission_interval, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 721 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;
  
  PARSER_DEBUG_PRINTF("\tTC validity: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  
 SET_IFS_CONF(ifs, ifcnt, tc_params.validity_time, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 733 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;


  PARSER_DEBUG_PRINTF("\tMID interval: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  
  SET_IFS_CONF(ifs, ifcnt, mid_params.emission_interval, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 746 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tMID validity: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  
  SET_IFS_CONF(ifs, ifcnt, mid_params.validity_time, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 758 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;
  
  PARSER_DEBUG_PRINTF("\tHNA interval: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);

  SET_IFS_CONF(ifs, ifcnt, hna_params.emission_interval, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 770 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tHNA validity: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);

  SET_IFS_CONF(ifs, ifcnt, hna_params.validity_time, (yyvsp[(2) - (2)])->floating);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 782 "src/cfgparser/oparse.y"
    {
  int ifcnt = ifs_in_curr_cfg;
  struct olsr_if *ifs = olsr_cnf->interfaces;

  PARSER_DEBUG_PRINTF("\tAutodetect changes: %s\n", (yyvsp[(2) - (2)])->boolean ? "YES" : "NO");

  SET_IFS_CONF(ifs, ifcnt, autodetect_chg, (yyvsp[(2) - (2)])->boolean);

  free((yyvsp[(2) - (2)]));
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 795 "src/cfgparser/oparse.y"
    {
  if (lq_mult_helper((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])) < 0) {
    YYABORT;
  }
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 802 "src/cfgparser/oparse.y"
    {
  if (lq_mult_helper((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])) < 0) {
    YYABORT;
  }
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 809 "src/cfgparser/oparse.y"
    {
  if (lq_mult_helper((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])) < 0) {
    YYABORT;
  }
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 817 "src/cfgparser/oparse.y"
    {
  olsr_cnf->debug_level = (yyvsp[(2) - (2)])->integer;
  PARSER_DEBUG_PRINTF("Debug level: %d\n", olsr_cnf->debug_level);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 826 "src/cfgparser/oparse.y"
    {
  if ((yyvsp[(2) - (2)])->integer == 4) {
    olsr_cnf->ip_version = AF_INET;
    olsr_cnf->ipsize = sizeof(struct in_addr);
    olsr_cnf->maxplen = 32;
  } else if ((yyvsp[(2) - (2)])->integer == 6) {
    olsr_cnf->ip_version = AF_INET6;
    olsr_cnf->ipsize = sizeof(struct in6_addr);
    olsr_cnf->maxplen = 128;
  } else {
    fprintf(stderr, "IPversion must be 4 or 6!\n");
    YYABORT;
  }

  PARSER_DEBUG_PRINTF("IpVersion: %d\n", (yyvsp[(2) - (2)])->integer);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 846 "src/cfgparser/oparse.y"
    {
  int i;
  PARSER_DEBUG_PRINTF("FIBMetric: %s\n", (yyvsp[(2) - (2)])->string);
  for (i=0; i<FIBM_CNT; i++) {
    if (strcmp((yyvsp[(2) - (2)])->string, FIB_METRIC_TXT[i]) == 0) {
      olsr_cnf->fib_metric = i;
      break;
    }
  }
  if (i == FIBM_CNT) {
    fprintf(stderr, "Bad FIBMetric value: %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }
  free((yyvsp[(1) - (2)]));
  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 866 "src/cfgparser/oparse.y"
    {
  union olsr_ip_addr ipaddr, netmask;

  if (olsr_cnf->ip_version == AF_INET6) {
    fprintf(stderr, "IPv4 addresses can only be used if \"IpVersion\" == 4, skipping HNA.\n");
    olsr_startup_sleep(3);
  }
  else {
    PARSER_DEBUG_PRINTF("HNA IPv4 entry: %s/%s\n", (yyvsp[(1) - (2)])->string, (yyvsp[(2) - (2)])->string);

    if (inet_pton(AF_INET, (yyvsp[(1) - (2)])->string, &ipaddr.v4) <= 0) {
      fprintf(stderr, "ihna4entry: Failed converting IP address %s\n", (yyvsp[(1) - (2)])->string);
      YYABORT;
    }
    if (inet_pton(AF_INET, (yyvsp[(2) - (2)])->string, &netmask.v4) <= 0) {
      fprintf(stderr, "ihna4entry: Failed converting IP address %s\n", (yyvsp[(1) - (2)])->string);
      YYABORT;
    }

    /* check that the given IP address is actually a network address */
    if ((ipaddr.v4.s_addr & ~netmask.v4.s_addr) != 0) {
      fprintf(stderr, "ihna4entry: The ipaddress \"%s\" is not a network address!\n", (yyvsp[(1) - (2)])->string);
      YYABORT;
    }

    /* Queue */
    ip_prefix_list_add(&olsr_cnf->hna_entries, &ipaddr, olsr_netmask_to_prefix(&netmask));
  }
  free((yyvsp[(1) - (2)])->string);
  free((yyvsp[(1) - (2)]));
  free((yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 900 "src/cfgparser/oparse.y"
    {
  union olsr_ip_addr ipaddr, netmask;

  if (olsr_cnf->ip_version == AF_INET6) {
    fprintf(stderr, "IPv4 addresses can only be used if \"IpVersion\" == 4, skipping HNA.\n");
    olsr_startup_sleep(3);
  }
  else {
    PARSER_DEBUG_PRINTF("HNA IPv4 entry: %s/%d\n", (yyvsp[(1) - (3)])->string, (yyvsp[(3) - (3)])->integer);

    if (inet_pton(AF_INET, (yyvsp[(1) - (3)])->string, &ipaddr.v4) <= 0) {
      fprintf(stderr, "ihna4entry: Failed converting IP address %s\n", (yyvsp[(1) - (3)])->string);
      YYABORT;
    }
    if ((yyvsp[(3) - (3)])->integer > olsr_cnf->maxplen) {
      fprintf(stderr, "ihna4entry: Prefix len %u > %d is not allowed!\n", (yyvsp[(3) - (3)])->integer, olsr_cnf->maxplen);
      YYABORT;
    }

    /* check that the given IP address is actually a network address */
    olsr_prefix_to_netmask(&netmask, (yyvsp[(3) - (3)])->integer);
    if ((ipaddr.v4.s_addr & ~netmask.v4.s_addr) != 0) {
      fprintf(stderr, "ihna4entry: The ipaddress \"%s\" is not a network address!\n", (yyvsp[(1) - (3)])->string);
      YYABORT;
    }

    /* Queue */
    ip_prefix_list_add(&olsr_cnf->hna_entries, &ipaddr, (yyvsp[(3) - (3)])->integer);
  }
  free((yyvsp[(1) - (3)])->string);
  free((yyvsp[(1) - (3)]));
  free((yyvsp[(3) - (3)]));
}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 936 "src/cfgparser/oparse.y"
    {
  if (add_ipv6_addr((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]))) {
    YYABORT;
  }
}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 942 "src/cfgparser/oparse.y"
    {
  if (add_ipv6_addr((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))) {
    YYABORT;
  }
}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 950 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("setting ifs_in_curr_cfg = 0\n");
  ifs_in_curr_cfg = 0;
}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 957 "src/cfgparser/oparse.y"
    {
  struct olsr_if *in, *last;
  in = olsr_cnf->interfaces;
  last = NULL;
  while (in != NULL) {
    if (strcmp(in->name, (yyvsp[(1) - (1)])->string) == 0) {
      free ((yyvsp[(1) - (1)])->string);
      break;
    }
    last = in;
    in = in->next;
  }

  if (in != NULL) {
    /* remove old interface from list to add it later at the beginning */
    if (last) {
      last->next = in->next;
    }
    else {
      olsr_cnf->interfaces = in->next;
    }
  }
  else {
    in = malloc(sizeof(*in));
    if (in == NULL) {
      fprintf(stderr, "Out of memory(ADD IF)\n");
      YYABORT;
    }
    memset(in, 0, sizeof(*in));

    in->cnf = malloc(sizeof(*in->cnf));
    if (in->cnf == NULL) {
      fprintf(stderr, "Out of memory(ADD IFRULE)\n");
      free(in);
      YYABORT;
    }
    memset(in->cnf, 0x00, sizeof(*in->cnf));

    in->cnfi = malloc(sizeof(*in->cnfi));
    if (in->cnfi == NULL) {
      fprintf(stderr, "Out of memory(ADD IFRULE)\n");
      free (in->cnf);
      free(in);
      YYABORT;
    }
    memset(in->cnfi, 0xFF, sizeof(*in->cnfi));
    in->cnfi->orig_lq_mult_cnt=0;

    in->name = (yyvsp[(1) - (1)])->string;
  }
  /* Queue */
  in->next = olsr_cnf->interfaces;
  olsr_cnf->interfaces = in;
  ifs_in_curr_cfg++;
  free((yyvsp[(1) - (1)]));
}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 1016 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Noint set to %d\n", (yyvsp[(2) - (2)])->boolean);
  olsr_cnf->allow_no_interfaces = (yyvsp[(2) - (2)])->boolean;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 1024 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("TOS: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->tos = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));

}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 1033 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("OlsrPort: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->olsrport = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 1041 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtProto: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_proto = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1049 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTable: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 1055 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTable: auto\n");
  olsr_cnf->rt_table = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 1063 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefault: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_default = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 1069 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefault: auto\n");
  olsr_cnf->rt_table_default = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 1077 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableTunnel: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_tunnel = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 1083 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableTunnel: auto\n");
  olsr_cnf->rt_table_tunnel = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 1091 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTablePriority: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_pri = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 1097 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTablePriority: auto\n");
  olsr_cnf->rt_table_pri = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 1103 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTablePriority: none\n");
  olsr_cnf->rt_table_pri = DEF_RT_NONE;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 1111 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultPriority: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_default_pri = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 1117 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultPriority: auto\n");
  olsr_cnf->rt_table_default_pri = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 1123 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultPriority: none\n");
  olsr_cnf->rt_table_default_pri = DEF_RT_NONE;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 1131 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableTunnelPriority: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_tunnel_pri = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 1137 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableTunnelPriority: auto\n");
  olsr_cnf->rt_table_tunnel_pri = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 1143 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableTunnelPriority: none\n");
  olsr_cnf->rt_table_tunnel_pri = DEF_RT_NONE;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 1151 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultOlsrPriority: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->rt_table_defaultolsr_pri = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 1157 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultOlsrPriority: auto\n");
  olsr_cnf->rt_table_defaultolsr_pri = DEF_RT_AUTO;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 1163 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("RtTableDefaultOlsrPriority: none\n");
  olsr_cnf->rt_table_defaultolsr_pri = DEF_RT_NONE;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 1171 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Willingness: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->willingness_auto = false;
  olsr_cnf->willingness = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 1180 "src/cfgparser/oparse.y"
    {
  olsr_cnf->use_hysteresis = (yyvsp[(2) - (2)])->boolean;
  PARSER_DEBUG_PRINTF("Hysteresis %s\n", olsr_cnf->use_hysteresis ? "enabled" : "disabled");
  free((yyvsp[(2) - (2)]));
}
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 1188 "src/cfgparser/oparse.y"
    {
  olsr_cnf->hysteresis_param.scaling = (yyvsp[(2) - (2)])->floating;
  PARSER_DEBUG_PRINTF("Hysteresis Scaling: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 1196 "src/cfgparser/oparse.y"
    {
  olsr_cnf->hysteresis_param.thr_high = (yyvsp[(2) - (2)])->floating;
  PARSER_DEBUG_PRINTF("Hysteresis UpperThr: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 1204 "src/cfgparser/oparse.y"
    {
  olsr_cnf->hysteresis_param.thr_low = (yyvsp[(2) - (2)])->floating;
  PARSER_DEBUG_PRINTF("Hysteresis LowerThr: %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 1212 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Pollrate %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  olsr_cnf->pollrate = (yyvsp[(2) - (2)])->floating;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 1220 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("NIC Changes Pollrate %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  olsr_cnf->nic_chgs_pollrate = (yyvsp[(2) - (2)])->floating;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 1228 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("TC redundancy %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->tc_redundancy = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1236 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("MPR coverage %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->mpr_coverage = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1244 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Link quality level %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->lq_level = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 1252 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Link quality fish eye %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->lq_fish = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1260 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Link quality aging factor %f\n", (double)(yyvsp[(2) - (2)])->floating);
  olsr_cnf->lq_aging = (yyvsp[(2) - (2)])->floating;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1268 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Minimum TC validity time %f\n", (double)(yyvsp[(2) - (2)])->floating);
  olsr_cnf->min_tc_vtime = (yyvsp[(2) - (2)])->floating;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1276 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Lock file %s\n", (yyvsp[(2) - (2)])->string);
  olsr_cnf->lock_file = (yyvsp[(2) - (2)])->string;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1283 "src/cfgparser/oparse.y"
    {
  olsr_cnf->lq_algorithm = (yyvsp[(2) - (2)])->string;
  PARSER_DEBUG_PRINTF("LQ Algorithm: %s\n", (yyvsp[(2) - (2)])->string);
  free((yyvsp[(2) - (2)]));
}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1291 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("NAT threshold %0.2f\n", (double)(yyvsp[(2) - (2)])->floating);
  olsr_cnf->lq_nat_thresh = (yyvsp[(2) - (2)])->floating;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1299 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Clear screen %s\n", (yyvsp[(2) - (2)])->boolean ? "enabled" : "disabled");
  olsr_cnf->clear_screen = (yyvsp[(2) - (2)])->boolean;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1307 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Use NIIT ip translation: %s\n", (yyvsp[(2) - (2)])->boolean ? "enabled" : "disabled");
  olsr_cnf->use_niit = (yyvsp[(2) - (2)])->boolean;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1315 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway system: %s\n", (yyvsp[(2) - (2)])->boolean ? "enabled" : "disabled");
	olsr_cnf->smart_gw_active = (yyvsp[(2) - (2)])->boolean;
	free((yyvsp[(2) - (2)]));
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1323 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway always remove server tunnel: %s\n", (yyvsp[(2) - (2)])->boolean ? "enabled" : "disabled");
	olsr_cnf->smart_gw_always_remove_server_tunnel = (yyvsp[(2) - (2)])->boolean;
	free((yyvsp[(2) - (2)]));
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1331 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway use count: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_use_count = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1339 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway takedown percentage: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_takedown_percentage = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1347 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway policy routing script: %s\n", (yyvsp[(2) - (2)])->string);
  olsr_cnf->smart_gw_policyrouting_script = (yyvsp[(2) - (2)])->string;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1361 "src/cfgparser/oparse.y"
    {
  struct sgw_egress_if *in, *previous, *last;
  char * str = (yyvsp[(1) - (1)])->string;
  char *end;

  /* Trim leading space */
  while(isspace(*str)) {
    str++;
  }

  /* Trim trailing space */
  end = str + strlen(str) - 1;
  while((end > str) && isspace(*end)) {
    end--;
  }

  /* Write new null terminator */
  *(end + 1) = '\0';

  if(*str == 0) {
    PARSER_DEBUG_PRINTF("Smart gateway egress interface: <empty> (skipped)\n");
  } else {
    PARSER_DEBUG_PRINTF("Smart gateway egress interface: %s\n", str);

    in = olsr_cnf->smart_gw_egress_interfaces;
    previous = NULL;
    while (in != NULL) {
      if (strcmp(in->name, str) == 0) {
        free ((yyvsp[(1) - (1)])->string);
        break;
      }
      previous = in;
      in = in->next;
    }

    if (in != NULL) {
      /* remove old interface from list to add it later at the end */
      if (previous) {
        previous->next = in->next;
      }
      else {
        olsr_cnf->smart_gw_egress_interfaces = in->next;
      }
      in->next = NULL;
    }
    else {
      /* interface in not in the list: create a new entry to add it later at the end */
      in = malloc(sizeof(*in));
      if (in == NULL) {
        fprintf(stderr, "Out of memory(ADD IF)\n");
        YYABORT;
      }
      memset(in, 0, sizeof(*in));

      in->name = str;
    }

    last = olsr_cnf->smart_gw_egress_interfaces;
    while (last && last->next) {
      last = last->next;
    }

    /* Add to the end of the list */
    if (!last) {
      olsr_cnf->smart_gw_egress_interfaces = in;
    } else {
      last->next = in;
    }
    free((yyvsp[(1) - (1)]));
  }
}
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1435 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway tables offset: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_offset_tables = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1443 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway rules offset: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_offset_rules = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1451 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway allow client nat: %s\n", (yyvsp[(2) - (2)])->boolean ? "yes" : "no");
	olsr_cnf->smart_gw_allow_nat = (yyvsp[(2) - (2)])->boolean;
	free((yyvsp[(2) - (2)]));
}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1459 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway period: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_period = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1467 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway stablecount: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_stablecount = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1475 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway threshold: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_thresh = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1483 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway exitlink uplink weight: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_weight_exitlink_up = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1491 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway exitlink downlink weight: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_weight_exitlink_down = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1499 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway ETX weight: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_weight_etx = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1507 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway ETX divider: %d\n", (yyvsp[(2) - (2)])->integer);
  olsr_cnf->smart_gw_divider_etx = (yyvsp[(2) - (2)])->integer;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1515 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway uplink: %s\n", (yyvsp[(2) - (2)])->string);
	if (strcasecmp((yyvsp[(2) - (2)])->string, GW_UPLINK_TXT[GW_UPLINK_NONE]) == 0) {
		olsr_cnf->smart_gw_type = GW_UPLINK_NONE;
	}
	else if (strcasecmp((yyvsp[(2) - (2)])->string, GW_UPLINK_TXT[GW_UPLINK_IPV4]) == 0) {
		olsr_cnf->smart_gw_type = GW_UPLINK_IPV4;
	}
	else if (strcasecmp((yyvsp[(2) - (2)])->string, GW_UPLINK_TXT[GW_UPLINK_IPV6]) == 0) {
		olsr_cnf->smart_gw_type = GW_UPLINK_IPV6;
	}
	else if (strcasecmp((yyvsp[(2) - (2)])->string, GW_UPLINK_TXT[GW_UPLINK_IPV46]) == 0) {
		olsr_cnf->smart_gw_type = GW_UPLINK_IPV46;
	}
	else {
		fprintf(stderr, "Bad gateway uplink type: %s\n", (yyvsp[(2) - (2)])->string);
		YYABORT;
	}
	free((yyvsp[(2) - (2)]));
}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1538 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway speed: %u uplink/%u downlink kbit/s\n", (yyvsp[(2) - (3)])->integer, (yyvsp[(3) - (3)])->integer);
	olsr_cnf->smart_gw_uplink = (yyvsp[(2) - (3)])->integer;
	olsr_cnf->smart_gw_downlink = (yyvsp[(3) - (3)])->integer;
	free((yyvsp[(2) - (3)]));
	free((yyvsp[(3) - (3)]));
}
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1548 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway uplink nat: %s\n", (yyvsp[(2) - (2)])->boolean ? "yes" : "no");
	olsr_cnf->smart_gw_uplink_nat = (yyvsp[(2) - (2)])->boolean;
	free((yyvsp[(2) - (2)]));
}
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1556 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Smart gateway prefix: %s %u\n", (yyvsp[(2) - (3)])->string, (yyvsp[(3) - (3)])->integer);
	if (inet_pton(olsr_cnf->ip_version, (yyvsp[(2) - (3)])->string, &olsr_cnf->smart_gw_prefix.prefix) == 0) {
	  fprintf(stderr, "Bad IP part of gateway prefix: %s\n", (yyvsp[(2) - (3)])->string);
    YYABORT;
  }
	olsr_cnf->smart_gw_prefix.prefix_len = (uint8_t)(yyvsp[(3) - (3)])->integer;
	
	free((yyvsp[(2) - (3)]));
	free((yyvsp[(3) - (3)]));
}
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1568 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Smart gateway prefix: %s %u\n", (yyvsp[(2) - (4)])->string, (yyvsp[(4) - (4)])->integer);
	if (inet_pton(olsr_cnf->ip_version, (yyvsp[(2) - (4)])->string, &olsr_cnf->smart_gw_prefix.prefix) == 0) {
	  fprintf(stderr, "Bad IP part of gateway prefix: %s\n", (yyvsp[(2) - (4)])->string);
    YYABORT;
  }
	olsr_cnf->smart_gw_prefix.prefix_len = (uint8_t)(yyvsp[(4) - (4)])->integer;
	
	free((yyvsp[(2) - (4)]));
	free((yyvsp[(4) - (4)]));
}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1582 "src/cfgparser/oparse.y"
    {
	PARSER_DEBUG_PRINTF("Use originator for routes src-ip: %s\n", (yyvsp[(2) - (2)])->boolean ? "yes" : "no");
	if (olsr_cnf->ip_version != AF_INET) {
          fprintf(stderr, "Source ip routes not possible with IPV6\n");
          YYABORT;
	}
	else olsr_cnf->use_src_ip_routes = (yyvsp[(2) - (2)])->boolean;
	free((yyvsp[(2) - (2)]));
}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1594 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Fixed Main IP: %s\n", (yyvsp[(2) - (2)])->string);
  
  if (olsr_cnf->ip_version != AF_INET
      || inet_pton(olsr_cnf->ip_version, (yyvsp[(2) - (2)])->string, &olsr_cnf->main_addr) != 1) {
    fprintf(stderr, "Bad main IP: %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }
  else olsr_cnf->unicast_src_ip = olsr_cnf->main_addr;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1606 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Fixed Main IP: %s\n", (yyvsp[(2) - (2)])->string);
  
  if (olsr_cnf->ip_version != AF_INET6
      || inet_pton(olsr_cnf->ip_version, (yyvsp[(2) - (2)])->string, &olsr_cnf->main_addr) != 1) {
    fprintf(stderr, "Bad main IP: %s\n", (yyvsp[(2) - (2)])->string);
    YYABORT;
  }
  free((yyvsp[(2) - (2)]));
}
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1619 "src/cfgparser/oparse.y"
    {
  PARSER_DEBUG_PRINTF("Set IP-Forward procfile variable: %s\n", (yyvsp[(2) - (2)])->boolean ? "yes" : "no");
  olsr_cnf->set_ip_forward = (yyvsp[(2) - (2)])->boolean;
  free((yyvsp[(2) - (2)]));
}
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1628 "src/cfgparser/oparse.y"
    {
  struct plugin_entry *pe, *last;
  
  pe = olsr_cnf->plugins;
  last = NULL;
  while (pe != NULL) {
    if (strcmp(pe->name, (yyvsp[(2) - (2)])->string) == 0) {
      free ((yyvsp[(2) - (2)])->string);
      break;
    }
    last = pe;
    pe = pe->next;
  }

  if (pe != NULL) {
    /* remove old plugin from list to add it later at the beginning */
    if (last) {
      last->next = pe->next;
    }
    else {
      olsr_cnf->plugins = pe->next;
    }
  }
  else {
    pe = malloc(sizeof(*pe));

    if (pe == NULL) {
      fprintf(stderr, "Out of memory(ADD PL)\n");
      YYABORT;
    }

    pe->name = (yyvsp[(2) - (2)])->string;
    pe->params = NULL;

    PARSER_DEBUG_PRINTF("Plugin: %s\n", (yyvsp[(2) - (2)])->string);
  }
  
  /* Queue */
  pe->next = olsr_cnf->plugins;
  olsr_cnf->plugins = pe;

  free((yyvsp[(2) - (2)]));
}
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1674 "src/cfgparser/oparse.y"
    {
  struct plugin_param *pp = malloc(sizeof(*pp));
  
  if (pp == NULL) {
    fprintf(stderr, "Out of memory(ADD PP)\n");
    YYABORT;
  }
  
  PARSER_DEBUG_PRINTF("Plugin param key:\"%s\" val: \"%s\"\n", (yyvsp[(2) - (3)])->string, (yyvsp[(3) - (3)])->string);
  
  pp->key = (yyvsp[(2) - (3)])->string;
  pp->value = (yyvsp[(3) - (3)])->string;

  /* Queue */
  pp->next = olsr_cnf->plugins->params;
  olsr_cnf->plugins->params = pp;

  free((yyvsp[(2) - (3)]));
  free((yyvsp[(3) - (3)]));
}
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1697 "src/cfgparser/oparse.y"
    {
    //PARSER_DEBUG_PRINTF("Comment\n");
}
    break;



/* Line 1806 of yacc.c  */
#line 3590 "src/cfgparser/oparse.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1704 "src/cfgparser/oparse.y"


void yyerror (const char *string)
{
  fprintf(stderr, "Config line %d: %s\n", current_line, string);
}

