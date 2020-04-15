/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

typedef enum   afb_auth_type            afb_auth_type_t;
typedef struct afb_auth                 afb_auth_t;
typedef struct afb_arg                  afb_arg_t;

#if AFB_BINDING_VERSION == 3

typedef struct afb_verb_v3              afb_verb_t;
typedef struct afb_binding_v3           afb_binding_t;

typedef struct afb_event_x2            *afb_event_t;
typedef struct afb_req_x2              *afb_req_t;
typedef struct afb_api_x3              *afb_api_t;
typedef enum afb_req_subcall_flags	afb_req_subcall_flags_t;

#define afbBindingExport		afbBindingV3
#define afbBindingRoot			afbBindingV3root
#define afbBindingEntry			afbBindingV3entry

/* compatibility with previous versions */

typedef struct afb_api_x3              *afb_daemon_t;
typedef struct afb_api_x3              *afb_service_t;

#endif
