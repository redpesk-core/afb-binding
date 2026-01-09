/*
 * Copyright (C) 2015-2026 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE

#include <stdio.h>

#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#define AFB_BINDING_X4R1_ITF_REVISION 7

#include <afb/afb-binding.h>

int afbBindingV4entry(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	switch (ctlid) {
	case afb_ctlid_Root_Entry:
		AFB_API_NOTICE(api, "ROOT ENTRY");
		AFB_API_NOTICE(api, "    path: %s", ctlarg->root_entry.path);
		AFB_API_NOTICE(api, "     uid: %s", ctlarg->root_entry.uid);
		AFB_API_NOTICE(api, "  config: %s", json_object_to_json_string(ctlarg->root_entry.config));
		break;

	case afb_ctlid_Pre_Init:
		AFB_API_NOTICE(api, "unexpected pre-init");
		break;

	case afb_ctlid_Init:
		AFB_API_NOTICE(api, "unexpected init");
		break;

	case afb_ctlid_Class_Ready:
		AFB_API_NOTICE(api, "unexpected class-ready");
		break;

	case afb_ctlid_Orphan_Event:
		AFB_API_NOTICE(api, "unexpected orphan-event");
		break;

	case afb_ctlid_Exiting:
		AFB_API_NOTICE(api, "exiting code %d", ctlarg->exiting.code);
		break;

	default:
		AFB_API_NOTICE(api, "unexpected ctlid %d", ctlid);
		break;
	}
	return 0;
}

