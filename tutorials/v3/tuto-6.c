/*
 * Copyright (C) 2015-2024 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */
#define AFB_BINDING_VERSION 3
#include <afb/afb-binding.h>

void hello(afb_req_t req)
{
	AFB_REQ_DEBUG(req, "hello world");
	afb_req_reply(req, NULL, NULL, "hello world");
}

static struct afb_auth auths[] = {
        {
                .type = afb_auth_Permission,
                .text = "permission"
        }
};

const afb_verb_t verbs[] = {
	{ .verb="hello", .callback=hello, .auth = &auths[0] },
	{ .verb=NULL }
};

const afb_binding_t afbBindingExport = {
	.api = "tuto-1",
	.verbs = verbs
};

