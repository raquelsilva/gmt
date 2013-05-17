/*--------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 1991-2013 by P. Wessel, W. H. F. Smith, R. Scharroo, J. Luis and F. Wobbe
 *	See LICENSE.TXT file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation; version 3 or any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	Contact info: gmt.soest.hawaii.edu
 *--------------------------------------------------------------------*/
/*
 * Launcher for any GMT5 program via the corresponding function.
 *
 * Version:	5
 * Created:	17-Feb-2010
 *
 */

#include "gmt_dev.h"

int main (int argc, char *argv[]) {
	int status = EXIT_SUCCESS;              /* Default status code */
	unsigned int modulename_arg_n = 0;      /* argument number that contains maodule name */
	enum GMT_MODULE_ID module_id = 0;       /* Module ID */
	struct GMTAPI_CTRL *api_ctrl = NULL;    /* GMT API control structure */
	char *progname = GMT_basename(argv[0]); /* Last component from the pathname */

	/* test if argv[0] contains module name: */
	if ((module_id = gmt_module_lookup (progname)) == GMT_ID_NONE && argc > 1) {
		/* argv[0] does not contain a valid module name
		 * argv[1] either holds the name of the module or an option: */
		modulename_arg_n = 1;
		module_id = gmt_module_lookup (argv[1]); /* either valid id or GMT_ID_NONE */
	}

	/* Initializing new GMT session */
	if ((api_ctrl = GMT_Create_Session (NULL, 2U, 0U, NULL)) == NULL)
		return EXIT_FAILURE;

	if (module_id == GMT_ID_NONE) {
		/* neither argv[0] nor argv[1] contain a valid module name */

		int arg_n;
		for (arg_n = 1; arg_n < argc; ++arg_n) {
			/* Try all remaining arguments: */

			/* Print module list */
			if (!strcmp (argv[arg_n], "--help")) {
				gmt_module_show_all(api_ctrl);
				goto exit;
			}

			/* Print version and exit */
			if (!strcmp (argv[arg_n], "--version")) {
				fprintf (stdout, "%s\n", GMT_PACKAGE_VERSION_WITH_SVN_REVISION);
				goto exit;
			}

			/* Show share directory */
			if (!strcmp (argv[arg_n], "--show-sharedir")) {
				fprintf (stdout, "%s\n", api_ctrl->GMT->session.SHAREDIR);
				goto exit;
			}

			/* Show the directory that contains the 'gmt' executable */
			if (!strcmp (argv[arg_n], "--show-bindir")) {
				fprintf (stdout, "%s\n", api_ctrl->GMT->init.runtime_bindir);
				goto exit;
			}
		} /* for (arg_n = 1; arg_n < argc; ++arg_n) */

		/* gmt.c is not a module and hence can use fprintf (stderr, ...). Any API needing a
		 * gmt-like application will write one separately [see mex API] */
		fprintf (stderr, "GMT - The Generic Mapping Tools, Version %s\n", GMT_VERSION);
		fprintf (stderr, "Copyright 1991-%d Paul Wessel, Walter H. F. Smith, R. Scharroo, J. Luis, and F. Wobbe\n\n", GMT_VERSION_YEAR);

		fprintf (stderr, "This program comes with NO WARRANTY, to the extent permitted by law.\n");
		fprintf (stderr, "You may redistribute copies of this program under the terms of the\n");
		fprintf (stderr, "GNU Lesser General Public License.\n");
		fprintf (stderr, "For more information about these matters, see the file named LICENSE.TXT.\n\n");
		fprintf (stderr, "usage: %s [options] <module name> [module options]\n\n", progname);
		fprintf (stderr, "  --help               List and description of GMT modules\n");
		fprintf (stderr, "  --version            Print version and exit\n");
		fprintf (stderr, "  --show-sharedir      Show share directory and exit\n");
		fprintf (stderr, "  --show-bindir        Show directory of executables and exit\n");
		status = EXIT_FAILURE;
		goto exit;
	} /* module_id == GMT_ID_NONE */

	/* OK, here we found a recognized GMT module and the API has been initialized; do the job */

	/* Run selected GMT cmd function, or give usage message if errors arise during parsing */
	status = g_module[module_id].p_func (api_ctrl, argc-1-modulename_arg_n, argv+1+modulename_arg_n);

exit:
	/* Destroy GMT session */
	if (GMT_Destroy_Session (api_ctrl))
		return EXIT_FAILURE;

	return status; /* Return the status from the module */
}
