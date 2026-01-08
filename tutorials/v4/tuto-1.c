/*
 * Copyright (C) 2015-2026 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */
#include <stdio.h>

#define AFB_BINDING_VERSION 4
#define AFB_BINDING_X4R1_ITF_REVISION 2
#include <afb/afb-binding.h>

void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	int retcod;
	const char *who = "World"; /* default value */
	char hello[100];
	afb_data_t reply, name;

	/* some output for tracking the process */
	AFB_REQ_DEBUG(req, "Hi debugger!");
	AFB_REQ_NOTICE(req, "Hi observer!");

	/* extract the name of the caller */
	retcod = afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_STRINGZ, &name);
	who =  retcod == 0 ? afb_data_ro_pointer(name) : "World" /* default value */;

	/* create the reply string */
	retcod = snprintf(hello, sizeof hello, "Hello %s!", who);
	if (retcod >= (int)sizeof hello) {
		AFB_REQ_WARNING(req, "name too long, truncated!");
		retcod = (int)sizeof hello - 1;
		hello[retcod] = 0;
	}

	/* make the reply string */
	afb_create_data_copy(&reply, AFB_PREDEFINED_TYPE_STRINGZ, hello, (size_t)(retcod + 1 /*with last zero*/));
	afb_req_reply(req, 0, 1, &reply);
}

const afb_verb_t verbs[] = {
	{ .verb="hello", .callback=hello },
	{ .verb=NULL }
};

const afb_binding_t afbBindingExport = {
	.api = "tuto-1",
	.verbs = verbs
};

