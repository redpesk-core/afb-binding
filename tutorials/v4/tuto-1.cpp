/*
 * Copyright (C) 2015-2023 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding>

void hello(afb::req req, afb::received_data params)
{
	try {
	}
	catch (std::exception &e) {
		req.error("catched exception %s", e.what());
		req.reply(
	}
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

{
	session &usr = req.context<session>();
	afb::dataset<1> a;

	AFB_REQ_NOTICE(req, "login user %s out", (const char*)usr);
	a[0] = json_to_req_data(req, json_object_new_string((const char*)usr));
	event_logout.push(a);
	req.session_set_LOA(0);
	req.context<session>().clear();
	req.reply();
}




int mainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	afb::api a(api);

	if (ctlid == afb_ctlid_Init) {
		AFB_NOTICE("init");
		event_login = a.new_event("login");
		event_logout = a.new_event("logout");
		if (!event_login || !event_login) {
			AFB_ERROR("Can't create events");
			return -1;
		}
	}
	return 0;
}

const afb_verb_t verbs[] = {
	afb::verb<hello>("hello", "simple hello reply"),
	afb::verbend()
};

const afb_binding_t afbBindingExport = afb::binding("tuto-1", verbs, "first C++ tutorial", mainctl);

