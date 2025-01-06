// Copyright (C) 2015-2025 IoT.bzh Company
// Author: José Bollo <jose.bollo@iot.bzh>
//
// SPDX-License-Identifier: LGPL-3.0-only

#include <string>
#include <cstring>
#include <utility>

#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding>

void hello(afb::req req, afb::received_data params)
{
	/* some output for tracking the process */
	AFB_REQ_DEBUG(req, "Hi debugger!");
	AFB_REQ_NOTICE(req, "Hi observer!");

	/* extract the name of the caller */
	const char *who = "World"; /* default value */
	if (req.try_convert(0, afb::STRINGZ()))
		who = (const char*)*(params[0]);

	/* create the reply string */
	std::string hello = (std::string("Hello ") + who) + '!';

	/* make the reply string */
	afb::data reply(afb::STRINGZ(), hello.c_str(), 1 + hello.size());
	req.reply(0, reply);
}

int mainctl(afb::api api, afb::ctlid ctlid, const afb::ctlarg ctlarg, void *userdata) noexcept
{
	if (ctlid == afb_ctlid_Init) {
		AFB_NOTICE("init");
	}
	return 0;
}

const afb_verb_t verbs[] = {
	afb::verb<hello>("hello", "hello the system"),
	afb::verbend()
};

const afb_binding_t afbBindingExport =
	afb::binding<mainctl>("tuto-1", verbs, "first C++ tutorial");

