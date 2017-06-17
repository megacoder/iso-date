/*
 *------------------------------------------------------------------------
 * vim: ts=8 sw=8
 *------------------------------------------------------------------------
 * Author:   tf135c (James Reynolds)
 * Filename: iso-date.c
 * Created:  2007-02-24 11:52:12
 *------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <gcc-compat.h>

static	char *		me = "iso-date";
static	unsigned	nonfatal;
static	unsigned	utc;
static	char *		tz;
static	char const	fmt_strict[]	= "%Y-%m-%dT%H:%M:%S";
static	char const	fmt_human[]	= "%Y-%m-%d %H:%M:%S %Z";
static	char const	fmt_token[]	= "%Y%m%d%H%M%S";
static	char const *	fmt = fmt_human;

int
main(
	int		argc,
	char * *	argv
)
{
	int		c;
	char *		bp;
	time_t		now;
	struct tm *	tm;
	size_t		needed;
	char		buf[ 32 ];

	/* Figure out our process name					*/
	me = argv[ 0 ];
	if( (bp = strrchr( me, '/' )) != NULL )	{
		me = bp + 1;
	}
	/* Process command line						*/
	opterr = 0;
	while( (c = getopt( argc, argv, "stuz:" )) != EOF )	{
		switch( c )	{
		default:
			fprintf(
				stderr,
				"%s: switch '-%c' not implemented yet.\n",
				me,
				c
			);
			++nonfatal;
			break;
		case '?':
			fprintf(
				stderr,
				"%s: switch '-%c' unknown.\n",
				me,
				optopt
			);
			++nonfatal;
			break;
		case 's':
			fmt = fmt_strict;
			break;
		case 't':
			fmt = fmt_token;
			break;
		case 'u':
			utc = 1;
			break;
		case 'z':
			tz = optarg;
			break;
		}
	}
	if( optind < argc )	{
		fprintf(
			stderr,
			"%s: too many arguments!\n",
			me
		);
		++nonfatal;
	}
	if( nonfatal )	{
		exit( 1 );
	}
	/* Choose another timezone if we were asked nicely		 */
	if( utc )	{
		/* Force strict time to be UTC				 */
		tz = "UTC";
	}
	if( tz && (setenv( "TZ", tz, 1 )) )	{
		fprintf(
			stderr,
			"%s: failed to set timezone to '%s'.\n",
			me,
			tz
		);
		exit( 1 );
	}
	/*								*/
	time( &now );
	tm = localtime( &now );
	needed = strftime(
		buf,
		sizeof( buf ),
		fmt,
		tm
	);
	puts( needed ? buf : "DUNNO" );
	return( 0 );
}
