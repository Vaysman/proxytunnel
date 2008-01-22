/* Proxytunnel - (C) 2001-2008 Jos Visser / Mark Janssen    */
/* Contact:                  josv@osp.nl / maniac@maniac.nl */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "proxytunnel.h"

#ifndef HAVE_GETOPT_LONG
	extern char * optarg;
#else
#include <getopt.h>
#endif

#include "cmdline.h"
static char *getCredentialsFromFile( const char* filename, char **user, char **pwd);

void
cmdline_parser_print_version (void)
{
	printf ("%s %s (rev %d)\nCopyright 2001-2008 Proxytunnel Project\n%s\n", PACKAGE, VERSION, REV, AUTHORS);
}

void
cmdline_parser_print_help (void)
{
	//  cmdline_parser_print_version ();
	printf(
"Usage: %s [OPTIONS]...\n"
"Build generic tunnels trough HTTPS proxy's, supports HTTP authorization\n"
"\n"
"Standard options:\n"
// FIXME: "   -c, --config=FILE       Read config options from file\n"
"  -i, --inetd             Run from inetd (default=off)\n"
"  -a, --standalone=INT    Run as standalone daemon on specified port\n"
// FIXME: "   -f, --nobackground      Don't for to background in standalone mode\n"
#ifdef USE_SSL
"  -e, --encrypt           SSL encrypt data between local proxy and destination\n"
"  -E, --encrypt-proxy     SSL encrypt data between client and local proxy\n"
#endif
"  -p, --proxy=STRING      Local proxy host:port combination\n"
"  -d, --dest=STRING       Destination host:port combination\n"
"\n"
"Additional options for specific features:\n"
"  -F, --passfile=STRING   File with credentials for proxy authentication\n"
"  -u, --user=STRING       Username for proxy authentication\n"
"  -s, --pass=STRING       Password for proxy authentication\n"
"  -U, --uservar=STRING    Environment variable that holds username\n"
"  -S, --passvar=STRING    Environment variable that holds password\n"
"  -N, --ntlm              Use NTLM based authentication\n"
"  -t, --domain=STRING     NTLM domain (default: autodetect)\n"
"  -r, --remproxy=STRING   Remote proxy host:port combination (using 2 proxies)\n"
"  -H, --header=STRING     Add additional HTTP headers to send to proxy\n"
#ifdef SETPROCTITLE
"  -x, --proctitle=STRING  Use a different process title\n"
#endif
"\n"
"Miscellaneous options:\n"
"  -v, --verbose           Turn on verbosity\n"
"  -q, --quiet             Suppress messages\n"
"  -h, --help              Print help and exit\n"
"  -V, --version           Print version and exit\n", PACKAGE);

#ifndef HAVE_GETOPT_LONG
	printf( "\n"
"Notice: This version is compiled without support for long options.\n"
"This means you can only use the short (1 letter) options on the commandline.\n" );
#endif
}


static char *
gengetopt_strdup (char * s)
{
	char * n, * pn, * ps = s;
	while (*ps) ps++;
	n = (char *) malloc (1 + ps - s);
	if (n != NULL)
	{
		for (ps=s,pn=n; *ps; ps++,pn++)
			*pn = *ps;
		*pn = 0;
	}
	return n;
}

int
cmdline_parser( int argc, char * const *argv, struct gengetopt_args_info *args_info )
{
	int c;	/* Character of the parsed option.  */
	int r;	/* Tmd retval */
	int missing_required_options = 0;	
	char * tmp_env_var;

	args_info->help_given = 0;
	args_info->version_given = 0;
	args_info->user_given = 0;
	args_info->pass_given = 0;
	args_info->proxy_given = 0;
	args_info->proxyhost_given = 0;
	args_info->proxyport_given = 0;
	args_info->dest_given = 0;
	args_info->remproxy_given = 0;
	args_info->verbose_given = 0;
	args_info->ntlm_given = 0;
	args_info->inetd_given = 0;
	args_info->quiet_given = 0;
	args_info->header_given = 0;
	args_info->domain_given = 0;
	args_info->encrypt_given = 0;
	args_info->encryptproxy_given = 0;
	args_info->proctitle_given = 0;

/* No... we can't make this a function... -- Maniac */
#define clear_args() \
{ \
	args_info->user_arg = NULL; \
	args_info->pass_arg = NULL; \
	args_info->domain_arg = NULL; \
	args_info->proxy_arg = NULL; \
	args_info->proxyhost_arg = NULL; \
	args_info->dest_arg = NULL; \
	args_info->remproxy_arg = NULL; \
	args_info->header_arg[0] = '\0'; \
	args_info->verbose_flag = 0; \
	args_info->ntlm_flag = 0; \
	args_info->inetd_flag = 0; \
	args_info->quiet_flag = 0; \
	args_info->standalone_arg = 0; \
	args_info->encrypt_flag = 0; \
	args_info->encryptproxy_flag = 0; \
	args_info->proctitle_arg = NULL; \
} 

	clear_args();

	optarg = 0;

#ifdef HAVE_GETOPT_LONG
	optind = 1;
	opterr = 1;
	optopt = '?';
#endif

	while (1) {
#ifdef HAVE_GETOPT_LONG
		int option_index = 0;

		/* Struct option: Name, Has_arg, Flag, Value */
		static struct option long_options[] = {
			{ "help",		0, NULL, 'h' },
			{ "version",	0, NULL, 'V' },
			{ "user",		1, NULL, 'u' },
			{ "pass",		1, NULL, 's' },
			{ "domain",		1, NULL, 't' },
			{ "uservar",	1, NULL, 'U' },
			{ "passvar",	1, NULL, 'S' },
			{ "passfile",	1, NULL, 'F' },
			{ "proxy",		1, NULL, 'p' },
			{ "dest",		1, NULL, 'd' },
			{ "remproxy",	1, NULL, 'r' },
			{ "proctitle",	1, NULL, 'x' },
			{ "header",		1, NULL, 'H' },
			{ "verbose",	0, NULL, 'v' },
			{ "ntlm",		0, NULL, 'N' },
			{ "inetd",		0, NULL, 'i' },
			{ "standalone", 1, NULL, 'a' },
			{ "quiet",		0, NULL, 'q' },
			{ "encrypt",	0, NULL, 'e' },
			{ "encrypt-proxy",	0, NULL, 'E' },
			{ NULL,	0, NULL, 0 }
		};

		c = getopt_long (argc, argv, "hVia:u:s:t:U:S:F:p:r:d:H:x:nvNeEq", long_options, &option_index);
#else
		c = getopt( argc, argv, "hVia:u:s:t:U:S:F:p:r:d:H:x:nvNeEq" );
#endif

		if (c == -1)
			break;	/* Exit from `while (1)' loop.  */

		switch (c) {
			case 'h':	/* Print help and exit.  */
				clear_args ();
				cmdline_parser_print_help ();
				exit(0);

#ifdef USE_SSL
			case 'e':       /* Turn on SSL encryption */
				args_info->encrypt_flag = !(args_info->encrypt_flag);
				if( args_info->verbose_flag )
					message("SSL enabled\n");
				break;

			case 'E':	/* Turn on client to proxy SSL encryption */
				args_info->encryptproxy_flag = !(args_info->encryptproxy_flag);
				if( args_info->verbose_flag )
					message("SSL client to proxy enabled\n");
				break;
#endif

			case 'i':	/* Run from inetd. */
				if ( args_info->standalone_arg > 0 ) {
					fprintf( stderr, "%s: '--inetd' ('-i') conflicts with '--standalone' ('-a')\n", PACKAGE );
					clear_args();
					exit( 1 );
				}
				args_info->inetd_flag = !(args_info->inetd_flag);
				break;

			case 'a':       /* Run as standalone daemon */
				if ( args_info->inetd_flag ) {
					fprintf( stderr, "%s: `--standalone' (`-a') conflicts with `--inetd' (`-i')\n", PACKAGE );
					clear_args();
					exit(1);
				}
				if ( ( args_info->standalone_arg = atoi( optarg ) ) < 1 ) {
					fprintf( stderr, "%s: Illegal port value for `--standalone' (`-a')\n", PACKAGE);
					clear_args();
					exit(1);
				}
				break;

			case 'V':	/* Print version and exit.  */
				clear_args ();
				cmdline_parser_print_version ();
				exit(0);

			case 'x':
				args_info->proctitle_given = 1;
				message( "Proctitle override enabled\n" );
				args_info->proctitle_arg = gengetopt_strdup (optarg);
				break;

			case 'u':	/* Username to send to HTTPS proxy for authentication.  */
				if (args_info->user_given) {
					fprintf (stderr, "%s: `--user' (`-u'), `--uservar' (`-U') or `--passfile' (`-F') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->user_given = 1;
				args_info->user_arg = gengetopt_strdup (optarg);
				break;

			case 'U':	/* Env Var with Username to send to HTTPS proxy for authentication.  */
				if (args_info->user_given) {
					fprintf (stderr, "%s: `--user' (`-u'), `--uservar' (`-U') or `--passfile' (`-F') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				tmp_env_var = getenv(optarg) ;
				if (!tmp_env_var) {
					fprintf (stderr, "%s Invalid environment variable\n", optarg) ;
					clear_args ();
					exit (1);
				}
				args_info->user_given = 1;
				args_info->user_arg = gengetopt_strdup (tmp_env_var);
				break;

			case 's':	/* Password to send to HTTPS proxy for authentication.  */
				if (args_info->pass_given) {
					fprintf (stderr, "%s: `--pass' (`-s'), `--passvar' (`-S') or `--passfile' (`-F') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->pass_given = 1;
				args_info->pass_arg = gengetopt_strdup (optarg);
				break;

			case 't':	/* Env Var with NTLM DOMAIN (when overriding).  */
				if (args_info->domain_given) {
					fprintf (stderr, "%s: `--domain' (`-t') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->domain_given = 1;
				args_info->domain_arg = gengetopt_strdup (optarg);
				break;

			case 'S':	/* Env Var with Password to send to HTTPS proxy for authentication.  */
				if (args_info->pass_given) {
					fprintf (stderr, "%s: `--pass' (`-s') or `--passvar' (`-S') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				tmp_env_var = getenv(optarg) ;
				if (!tmp_env_var) {
					fprintf (stderr, "%s Invalid environment variable\n", optarg) ;
					clear_args ();
					exit (1);
				}
				args_info->pass_given = 1;
				args_info->pass_arg = gengetopt_strdup (tmp_env_var);
				break;

			case 'F':	/* File containing Username & Password to send to
							HTTPS proxy for authentication.  */
				if (args_info->user_given) {
					fprintf (stderr, "%s: `--user' (`-u'), `--uservar' (`-U') or  `--passfile' (`-F') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				if (args_info->pass_given) {
					fprintf (stderr, "%s: `--pass' (`-s'), `--passvar' (`-S') or `--passfile' (`-F') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->user_given = 1;
				args_info->pass_given = 1;
				char *result = getCredentialsFromFile(optarg, &(args_info->user_arg), &(args_info->pass_arg) );

				if( result != NULL ) {
					fprintf( stderr, "%s: Bad password file for `--passfile' (`-F')\n%s\n", PACKAGE, result);
					clear_args();
					exit (1);
				}
				break;

			case 'p':       /* HTTPS Proxy host:port to connect to.  */
				if (args_info->proxy_given) {
					fprintf (stderr, "%s: `--proxy' (`-p') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->proxy_given = 1;
				args_info->proxy_arg = gengetopt_strdup (optarg);
				break;

			case 'r':       /* Use a remote proxy */
				if (args_info->remproxy_given) {
					fprintf (stderr, "%s: `--remproxy' (`-r') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->remproxy_given = 1;
				args_info->remproxy_arg = gengetopt_strdup (optarg);
				break;

			case 'd':	/* Destination host to built the tunnel to.  */
				if (args_info->dest_given) {
					fprintf (stderr, "%s: `--dest' (`-d') option given more than once\n", PACKAGE);
					clear_args ();
					exit (1);
				}
				args_info->dest_given = 1;
				args_info->dest_arg = gengetopt_strdup (optarg);
				break;

			case 'H':	/* Extra headers to send to HTTPS proxy. */
				args_info->header_given++;		/* Amount of extra headers */
				strlcat( args_info->header_arg, optarg, MAX_HEADER_SIZE );
				strlcat( args_info->header_arg, "\r\n", MAX_HEADER_SIZE );
				break;

			case 'v':	/* Turn on verbosity.  */
				if (args_info->quiet_flag) {        /* -q also on cmd line */
					fprintf (stderr, "-v and -q are mutually exclusive\n");
					clear_args();
					exit(1);
				}
				args_info->verbose_flag = !(args_info->verbose_flag);
				break;

			case 'N':	/* Turn on NTLM.  */
				args_info->ntlm_flag = !(args_info->ntlm_flag);
				break;

			case 'q':	/* Suppress messages -- Quiet mode */
				args_info->quiet_flag = !(args_info->quiet_flag);
				break;

			case 0:	/* Long option with no short option */

			case '?':	/* Invalid option.  */
				/* `getopt_long' already printed an error message.  */
				clear_args();
				exit (1);

			default:	/* bug: option not considered.  */
				fprintf (stderr, "%s: option unknown: %c\n", PACKAGE, c);
				clear_args();
				abort();
		} /* switch */
	} /* while */

	if (! args_info->proxy_given && ! args_info->dest_given ) {
		clear_args ();
		cmdline_parser_print_help ();
		exit(0);
	}

/* For Windows quiet is the default output. -- Dag */
#ifdef CYGWIN
	if (! args_info->verbose_flag ) {
		args_info->quiet_flag = 1;
	}
#endif

	if (args_info->proxy_given ) {
		char * phost;
		int pport;

		phost = malloc( 50+1 );

		r = sscanf( args_info->proxy_arg, "%50[^:]:%5u", phost, &pport );
		if ( r == 2 ) {
			args_info->proxyhost_arg = phost;
			args_info->proxyport_arg = pport;
			args_info->proxyhost_given = 1;
			args_info->proxyport_given = 1;
		} else {
			message( "parse_cmdline: couln't find your proxy hostname/ip\n" );
			missing_required_options++;
		}
	}

	if ( missing_required_options )
		exit (1);

	return 0;
}

static char *
getCredentialsFromFile( const char* filename, char **user, char **pwd ) {
	/* Check file permissions, must have '0' for group and other */
	struct stat statbuf;
	if ( stat( filename, &statbuf ) == 0 ) {
		if ( statbuf.st_mode & (S_IRWXG|S_IRWXO) ) {
			return strdup( "Stricter permissions required for password file" );
		}
	} else {
		return strdup( strerror(errno) );
	}

	FILE* pwfile = fopen( filename, "r" );
	char line[80], buf[80];

	*user = NULL;
	*pwd = NULL;

	if( pwfile ) {
		/* Read a line */
		while (fgets( line, 80, pwfile ) != NULL ) {
			if ( sscanf( line, "proxy_user = %s", buf ) == 1 ) {
				*user = strdup( buf );
			} else if ( sscanf( line, "proxy_passwd = %s", buf ) == 1 ) {
				*pwd = strdup( buf );
			}
			if ( *user != NULL && *pwd != NULL ) {
				fclose( pwfile );
				return NULL;
			}
		}
		fclose( pwfile );
		return strdup( "proxy_user & proxy_passwd not found in password file" );
	}

	return strdup( "Error opening password file" );
}

// vim:noet
