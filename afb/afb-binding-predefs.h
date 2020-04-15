/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/******************************************************************************/
#if AFB_BINDING_VERSION == 3

#define afb_req_x2   			afb_req

#define afb_req_x2_is_valid		afb_req_is_valid
#define afb_req_x2_get_api		afb_req_get_api
#define afb_req_x2_get_vcbdata		afb_req_get_vcbdata
#define afb_req_x2_get_called_api	afb_req_get_called_api
#define afb_req_x2_get_called_verb	afb_req_get_called_verb
#define afb_req_x2_wants_log_level	afb_req_wants_log_level

#define afb_req_x2_get			afb_req_get
#define afb_req_x2_value		afb_req_value
#define afb_req_x2_path			afb_req_path
#define afb_req_x2_json			afb_req_json
#define afb_req_x2_reply		afb_req_reply
#define afb_req_x2_reply_f		afb_req_reply_f
#define afb_req_x2_reply_v		afb_req_reply_v
#define afb_req_success(r,o,i)		afb_req_reply(r,o,0,i)
#define afb_req_success_f(r,o,...)	afb_req_reply_f(r,o,0,__VA_ARGS__)
#define afb_req_success_v(r,o,f,v)	afb_req_reply_v(r,o,0,f,v)
#define afb_req_fail(r,e,i)		afb_req_reply(r,0,e,i)
#define afb_req_fail_f(r,e,...)		afb_req_reply_f(r,0,e,__VA_ARGS__)
#define afb_req_fail_v(r,e,f,v)		afb_req_reply_v(r,0,e,f,v)
#define afb_req_x2_context_get		afb_req_context_get
#define afb_req_x2_context_set		afb_req_context_set
#define afb_req_x2_context		afb_req_context
#define afb_req_x2_context_make		afb_req_context_make
#define afb_req_x2_context_clear	afb_req_context_clear
#define afb_req_x2_addref		afb_req_addref
#define afb_req_x2_unref		afb_req_unref
#define afb_req_x2_session_close	afb_req_session_close
#define afb_req_x2_session_set_LOA	afb_req_session_set_LOA
#define afb_req_x2_subscribe		afb_req_subscribe
#define afb_req_x2_unsubscribe		afb_req_unsubscribe
#define afb_req_x2_subcall		afb_req_subcall
#define afb_req_x2_subcall_legacy	afb_req_subcall_legacy
#define afb_req_x2_subcall_req		afb_req_subcall_req
#define afb_req_x2_subcall_sync_legacy	afb_req_subcall_sync_legacy
#define afb_req_x2_subcall_sync		afb_req_subcall_sync
#define afb_req_x2_verbose		afb_req_verbose
#define afb_req_x2_has_permission	afb_req_has_permission
#define afb_req_x2_check_permission	afb_req_check_permission
#define afb_req_x2_get_application_id	afb_req_get_application_id
#define afb_req_x2_get_uid		afb_req_get_uid
#define afb_req_x2_get_client_info	afb_req_get_client_info

#define afb_req_x2_subcall_flags	afb_req_subcall_flags
#define afb_req_x2_subcall_catch_events	afb_req_subcall_catch_events
#define afb_req_x2_subcall_pass_events	afb_req_subcall_pass_events
#define afb_req_x2_subcall_on_behalf	afb_req_subcall_on_behalf
#define afb_req_x2_subcall_api_session	afb_req_subcall_api_session
	
#define afb_event_x2			afb_event
#define afb_event_x2_is_valid		afb_event_is_valid
#define afb_event_x2_broadcast		afb_event_broadcast
#define afb_event_x2_push		afb_event_push
#define afb_event_x2_name		afb_event_name
#define afb_event_x2_unref		afb_event_unref
#define afb_event_x2_addref		afb_event_addref

#define afb_api_x3			afb_api

#define afb_api_x3_name			afb_api_name
#define afb_api_x3_get_userdata		afb_api_get_userdata
#define afb_api_x3_set_userdata		afb_api_set_userdata
#define afb_api_x3_wants_log_level	afb_api_wants_log_level

#define afb_api_x3_vverbose		afb_api_vverbose
#define afb_api_x3_verbose		afb_api_verbose
#define afb_api_x3_get_event_loop	afb_api_get_event_loop
#define afb_api_x3_get_user_bus		afb_api_get_user_bus
#define afb_api_x3_get_system_bus	afb_api_get_system_bus
#define afb_api_x3_rootdir_get_fd	afb_api_rootdir_get_fd
#define afb_api_x3_rootdir_open_locale	afb_api_rootdir_open_locale
#define afb_api_x3_queue_job		afb_api_queue_job
#define afb_api_x3_require_api		afb_api_require_api
#define afb_api_x3_broadcast_event	afb_api_broadcast_event
#define afb_api_x3_make_event_x2	afb_api_make_event
#define afb_api_x3_call			afb_api_call
#define afb_api_x3_call_sync		afb_api_call_sync
#define afb_api_x3_call_legacy		afb_api_call_legacy
#define afb_api_x3_call_sync_legacy	afb_api_call_sync_legacy
#define afb_api_x3_new_api		afb_api_new_api
#define afb_api_x3_delete_api		afb_api_delete_api
#define afb_api_x3_set_verbs_v2		afb_api_set_verbs_v2
#define afb_api_x3_set_verbs_v3		afb_api_set_verbs_v3
#define afb_api_x3_add_verb		afb_api_add_verb
#define afb_api_x3_del_verb		afb_api_del_verb
#define afb_api_x3_on_event		afb_api_on_event
#define afb_api_x3_on_init		afb_api_on_init
#define afb_api_x3_seal			afb_api_seal
#define afb_api_x3_add_alias		afb_api_add_alias
#define afb_api_x3_event_handler_add	afb_api_event_handler_add
#define afb_api_x3_event_handler_del	afb_api_event_handler_del
#define afb_api_x3_require_class	afb_api_require_class
#define afb_api_x3_provide_class	afb_api_provide_class
#define afb_api_x3_settings		afb_api_settings

#define AFB_API_ERROR			AFB_API_ERROR_V3
#define AFB_API_WARNING			AFB_API_WARNING_V3
#define AFB_API_NOTICE			AFB_API_NOTICE_V3
#define AFB_API_INFO			AFB_API_INFO_V3
#define AFB_API_DEBUG			AFB_API_DEBUG_V3

#define AFB_REQ_ERROR			AFB_REQ_ERROR_V3
#define AFB_REQ_WARNING			AFB_REQ_WARNING_V3
#define AFB_REQ_NOTICE			AFB_REQ_NOTICE_V3
#define AFB_REQ_INFO			AFB_REQ_INFO_V3
#define AFB_REQ_DEBUG			AFB_REQ_DEBUG_V3

#define AFB_REQ_VERBOSE			AFB_REQ_VERBOSE_V3

#define afb_stored_req 			afb_req_x2
#define afb_req_store(x) 		afb_req_x2_addref(x)
#define afb_req_unstore(x) 		(x)

#define afb_get_verbosity		afb_get_verbosity_v3
#define afb_get_logmask			afb_get_logmask_v3
#define afb_get_daemon			afb_get_root_api_v3
#define afb_get_service			afb_get_root_api_v3

#define afb_daemon_get_event_loop	afb_daemon_get_event_loop_v3
#define afb_daemon_get_user_bus		afb_daemon_get_user_bus_v3
#define afb_daemon_get_system_bus	afb_daemon_get_system_bus_v3
#define afb_daemon_broadcast_event	afb_daemon_broadcast_event_v3
#define afb_daemon_make_event		afb_daemon_make_event_v3
#define afb_daemon_verbose		afb_daemon_verbose_v3
#define afb_daemon_rootdir_get_fd	afb_daemon_rootdir_get_fd_v3
#define afb_daemon_rootdir_open_locale	afb_daemon_rootdir_open_locale_v3
#define afb_daemon_queue_job		afb_daemon_queue_job_v3
#define afb_daemon_require_api		afb_daemon_require_api_v3
#define afb_daemon_add_alias		afb_daemon_add_alias_v3

#define afb_service_call		afb_service_call_v3
#define afb_service_call_sync		afb_service_call_sync_v3
#define afb_service_call_legacy         afb_service_call_legacy_v3
#define afb_service_call_sync_legacy	afb_service_call_sync_legacy_v3

#define AFB_ERROR			AFB_ERROR_V3
#define AFB_WARNING			AFB_WARNING_V3
#define AFB_NOTICE			AFB_NOTICE_V3
#define AFB_INFO			AFB_INFO_V3
#define AFB_DEBUG			AFB_DEBUG_V3

#define AFB_SESSION_NONE		AFB_SESSION_NONE_X2
#define AFB_SESSION_CLOSE		AFB_SESSION_CLOSE_X2
#define AFB_SESSION_RENEW		AFB_SESSION_REFRESH_X2
#define AFB_SESSION_REFRESH		AFB_SESSION_REFRESH_X2
#define AFB_SESSION_CHECK		AFB_SESSION_CHECK_X2

#define AFB_SESSION_LOA_MASK		AFB_SESSION_LOA_MASK_X2

#define AFB_SESSION_LOA_0		AFB_SESSION_LOA_0_X2
#define AFB_SESSION_LOA_1		AFB_SESSION_LOA_1_X2
#define AFB_SESSION_LOA_2		AFB_SESSION_LOA_2_X2
#define AFB_SESSION_LOA_3		AFB_SESSION_LOA_3_X2

/******************************************************************************/

#endif

