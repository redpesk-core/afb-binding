/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "afb-ctlid.h"

/******************************************************************************/
/* declaration of types */

struct afb_binding_x4_itf;
struct afb_verb_v4;

struct afb_api_x4;
struct afb_req_x4;
struct afb_event_x4;
struct afb_type_x4;
struct afb_data_x4;

typedef const struct afb_api_x4        *afb_api_x4_t;
typedef const struct afb_req_x4        *afb_req_x4_t;
typedef const struct afb_event_x4      *afb_event_x4_t;
typedef const struct afb_data_x4       *afb_data_x4_t;
typedef const struct afb_type_x4       *afb_type_x4_t;

/******************************************************************************/

typedef
	enum afb_data_action_x4
{
	/**
	 * Notify that the value of the data changed
	 */
	Afb_Data_Action_x4_Notify_Changed = 0,

	/**
	 * Test if the data is constant.
	 * If the data is constant, it can not be modified
	 * by querying a not const pointer to it.
	 */
	Afb_Data_Action_x4_Is_Constant = 1,

	/**
	 * Tell that the data is constant.
	 * That is the default state at creation.
	 */
	Afb_Data_Action_x4_Set_Constant = 2,

	/**
	 * Tell that the data is not constant.
	 * At creation, data is told to be constant.
	 */
	Afb_Data_Action_x4_Set_Not_Constant = 3,

	/**
	 * Test if the data is volatile.
	 * A data is volatile if its value can change at
	 * any time. If a data is volatile, it implies
	 * conversions of that have a limited validity.
	 * Clock is the good example of a volatile data.
	 */
	Afb_Data_Action_x4_Is_Volatile = 4,

	/**
	 * Tell that the data is volatile.
	 * At creation, data are not told volatile.
	 */
	Afb_Data_Action_x4_Set_Volatile = 5,

	/**
	 * Tell that the data is not volatile.
	 * That is the default state at creation.
	 */
	Afb_Data_Action_x4_Set_Not_Volatile = 6,

	/**
	 * Requires to lock the data for reading.
	 */
	Afb_Data_Action_x4_Lock_Read = 7,

	/**
	 * Requires to try to lock the data for reading.
	 */
	Afb_Data_Action_x4_Try_Lock_Read = 8,

	/**
	 * Requires to lock the data for writing.
	 */
	Afb_Data_Action_x4_Lock_Write = 9,

	/**
	 * Requires to try to lock the data for writing.
	 */
	Afb_Data_Action_x4_Try_Lock_Write = 10,

	/**
	 * Requires to unlock the locked data.
	 */
	Afb_Data_Action_x4_Unlock = 11
}
	afb_data_action_x4_t;

/******************************************************************************/

typedef
	enum afb_type_flags_x4
{
	/** Data of that type can be shared through memory */
	Afb_Type_Flags_x4_Shareable = 1,

	/** Data of that type can be streamed */
	Afb_Type_Flags_x4_Streamable = 2,

	/** Data of that type can be opacified. */
	Afb_Type_Flags_x4_Opaque = 4
}
	afb_type_flags_x4_t;

/******************************************************************************/

/**
 */
typedef int (*afb_api_callback_x4_t)(
		afb_api_x4_t api,
		afb_ctlid_t ctlid,
		afb_ctlarg_t ctlarg);

/**
 */
typedef void (*afb_call_callback_x4_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_x4_t const replies[],
		afb_api_x4_t api);

/**
 */
typedef void (*afb_req_callback_x4_t)(
		afb_req_x4_t req,
		unsigned nparams,
		afb_data_x4_t const params[]);

/**
 */
typedef void (*afb_subcall_callback_x4_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_x4_t const replies[],
		afb_req_x4_t req);

/**
 */
typedef void (*afb_check_callback_x4_t)(
		void *closure,
		int status,
		afb_req_x4_t req);

/**
 */
typedef void (*afb_event_handler_x4_t)(
		void *closure,
		const char *event_name,
		unsigned nparams,
		afb_data_x4_t const params[],
		afb_api_x4_t api);

/**
 * The type afb_type_converter_x4_t denote a conversion callback.
 *
 * A conversion callback receives 4 parameters:
 *
 *  - void *          closure   A closure defined when converter is declared
 *  - afb_data_x4_t   from      The data to convert
 *  - afb_type_x4_t   type      The type to convert to
 *  - afb_data_x4_t * to        Where to store the result of the conversion
 *
 * It should return an integer status of 0 in case of success or a negative value
 * indicating the error.
 */
typedef int (*afb_type_converter_x4_t)(
			void *closure,
			afb_data_x4_t from,
			afb_type_x4_t type,
			afb_data_x4_t *to);

/**
 * The type afb_type_updater_x4_t denote a conversion callback that is able
 * to update the target instead of creating it.
 *
 * A conversion callback receives 4 parameters:
 *
 *  - void *          closure   A closure defined when converter is declared
 *  - afb_data_x4_t   from      The data of reference
 *  - afb_type_x4_t   type      The type of the data to update
 *  - afb_data_x4_t   to        the existing data to update from the given reference
 *
 * It should return an integer status of 0 in case of success or a negative value
 * indicating the error.
 */
typedef int (*afb_type_updater_x4_t)(
			void *closure,
			afb_data_x4_t from,
			afb_type_x4_t type,
			afb_data_x4_t to);

/******************************************************************************/

/**
 * Definition of the function's interface for the APIv3
 */
struct afb_binding_x4_itf
{
	/* CAUTION: respect the order, add at the end, see AFTERWARD... */

/*-- DATA ------------------------------------------*/

	/** create a new data from raw values */
	int (*create_data_raw)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		const void *buffer,
		size_t size,
		void (*dispose)(void*),
		void *closure);

	/** create a new data allocated in shareable memory */
	int (*create_data_alloc)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		void **pointer,
		size_t size,
		int zeroes);

	/** create a new data by copying in shareable memory */
	int (*create_data_copy)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		const void *pointer,
		size_t size);

	/** increment of reference count of data */
	afb_data_x4_t(*data_addref)(
		afb_data_x4_t data);

	/** decrement of reference count of data */
	void (*data_unref)(
		afb_data_x4_t data);

	/** get mutable pointer and size optionally */
	int (*data_get_mutable)(
		afb_data_x4_t data,
		void **pointer,
		size_t *size);

	/** get constant pointer and size optionally */
	int (*data_get_constant)(
		afb_data_x4_t data,
		const void **pointer,
		size_t *size);

	/** updfate the data from the given value */
	int (*data_update)(
		afb_data_x4_t data,
		afb_data_x4_t value);

	/** get the data converted/formated to the given type (return a new reference) */
	int (*data_convert)(
		afb_data_x4_t data,
		afb_type_x4_t type,
		afb_data_x4_t *result);

	/** set flags of the data  */
	int (*data_control)(
		afb_data_x4_t data,
		afb_data_action_x4_t action);

	/** get the type */
	afb_type_x4_t (*data_type)(
		afb_data_x4_t data);

/*-- REQ ------------------------------------------*/

	/** logmask of req */
	int (*req_logmask)(
		afb_req_x4_t req);

	/** increase reference count of the request */
	afb_req_x4_t (*req_addref)(
		afb_req_x4_t req);

	/** decrease reference count of the request */
	void (*req_unref)(
		afb_req_x4_t req);

	/** get api */
	afb_api_x4_t (*req_api)(
		afb_req_x4_t req);

	/** get vcbdata */
	void *(*req_vcbdata)(
		afb_req_x4_t req);

	/** get called api */
	const char *(*req_called_api)(
		afb_req_x4_t req);

	/** get called verb */
	const char *(*req_called_verb)(
		afb_req_x4_t req);

	/** log a message for the request */
	void (*req_vverbose)(
		afb_req_x4_t req,
		int level,
		const char *file,
		int line,
		const char * func,
		const char *fmt,
		va_list args);

	/** close the client session related to the api of the request */
	void (*req_session_close)(
		afb_req_x4_t req);

	/** set the levele of assurancy related to the api of the request */
	int (*req_session_set_LOA)(
		afb_req_x4_t req,
		unsigned level);

	/** handle client cookie of the api getting the request */
	void *(*req_cookie)(
		afb_req_x4_t req,
		int replace,
		void *(*create_value)(void *creation_closure),
		void (*free_value)(void*),
		void *creation_closure);

	/** make subscription of the client of the request to the event */
	int (*req_subscribe)(
		afb_req_x4_t req,
		afb_event_x4_t event);

	/** remove subscription of the client of the request to the event */
	int (*req_unsubscribe)(
		afb_req_x4_t req,
		afb_event_x4_t event);

	/** get description of the client of the request */
	struct json_object *(*req_get_client_info)(
		afb_req_x4_t req);

	/** check the permission */
	void (*req_check_permission)(
		afb_req_x4_t req,
		const char *permission,
		afb_check_callback_x4_t callback,
		void *closure);

	/** get the parameters of the request */
	unsigned (*req_parameters)(
		afb_req_x4_t req,
		afb_data_x4_t const **params);

	/** reply to the request */
	void (*req_reply)(
		afb_req_x4_t req,
		int status,
		unsigned nreplies,
		afb_data_x4_t const *replies);

	/** asynchronous subcall */
	void (*req_subcall)(
		afb_req_x4_t req,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int flags,
		afb_subcall_callback_x4_t callback,
		void *closure);

	/** synchronous subcall */
	int (*req_subcall_sync)(
		afb_req_x4_t req,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int flags,
		int *status,
		unsigned *nreplies,
		afb_data_x4_t replies[]);

/*-- EVENT ------------------------------------------*/

	/** rereference the event */
	afb_event_x4_t (*event_addref)(
		afb_event_x4_t event);

	/** unreference the event */
	void (*event_unref)(
		afb_event_x4_t event);

	/** get the event name */
	const char *(*event_name)(
		afb_event_x4_t event);

	/** push the event to its subscribers */
	int (*event_push)(
		afb_event_x4_t event,
		unsigned nparams,
		afb_data_x4_t const params[]);

	/** broadcast the event */
	int (*event_broadcast)(
		afb_event_x4_t event,
		unsigned nparams,
		afb_data_x4_t const params[]);

/*-- TYPE ------------------------------------------*/

	/** lookup for a type */
	int (*type_lookup)(
		afb_type_x4_t *type,
		const char *name);

	/** register a type */
	int (*type_register)(
		afb_type_x4_t *type,
		const char *name,
		afb_type_flags_x4_t flags);

	/** name of the type */
	const char *(*type_name)(
		afb_type_x4_t type
		);

	/** set the family of a type */
	int (*type_set_family)(
		afb_type_x4_t type,
		afb_type_x4_t family
		);

	/** add converter */
	int (*type_add_converter)(
		afb_type_x4_t fromtype,
		afb_type_x4_t totype,
		afb_type_converter_x4_t converter,
		void *closure
		);

	/** add updater */
	int (*type_add_updater)(
		afb_type_x4_t fromtype,
		afb_type_x4_t totype,
		afb_type_updater_x4_t updater,
		void *closure
		);

/*-- API ------------------------------------------*/

	/** logmask of api */
	int (*api_logmask)(
		afb_api_x4_t api);

	/** sending log messages */
	void (*api_vverbose)(
		afb_api_x4_t api,
		int level,
		const char *file,
		int line,
		const char * func,
		const char *fmt,
		va_list args);

	/** Get name of the api */
	const char *(*api_name)(
		afb_api_x4_t api);

	/** Getting user data */
	void *(*api_get_userdata)(
		afb_api_x4_t api);

	/** Setting user data */
	void *(*api_set_userdata)(
		afb_api_x4_t api,
		void *userdata);

	/** settings of the api */
	struct json_object *(*api_settings)(
		afb_api_x4_t api);

	/** broadcasts event 'name' with 'object' */
	int (*api_event_broadcast)(
		afb_api_x4_t api,
		const char *name,
		unsigned nparams,
		afb_data_x4_t const params[]);

	/** creates an event of 'name' */
	int (*api_new_event)(
		afb_api_x4_t api,
		const char *name,
		afb_event_x4_t *event);

	/** add an event handler for the api */
	int (*api_event_handler_add)(
		afb_api_x4_t api,
		const char *pattern,
		afb_event_handler_x4_t callback,
		void *closure);

	/** delete an event handler of the api */
	int (*api_event_handler_del)(
		afb_api_x4_t api,
		const char *pattern,
		void **closure);

	/** asynchronous call for the api */
	void (*api_call)(
		afb_api_x4_t api,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		afb_call_callback_x4_t callback,
		void *closure);

	/** synchronous call for the api */
	int (*api_call_sync)(
		afb_api_x4_t api,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int *status,
		unsigned *nreplies,
		afb_data_x4_t replies[]);

	/** add one verb to the api */
	int (*api_add_verb)(
		afb_api_x4_t api,
		const char *verb,
		const char *info,
		afb_req_callback_x4_t callback,
		void *vcbdata,
		const struct afb_auth *auth,
		uint32_t session,
		int glob);

	/** delete one verb of the api */
	int (*api_del_verb)(
		afb_api_x4_t api,
		const char *verb,
		void **vcbdata);

	/** seal the api */
	void (*api_seal)(
		afb_api_x4_t api);

	/** set verbs of the api using v3 description */
	int (*api_set_verbs)(
		afb_api_x4_t api,
		const struct afb_verb_v4 *verbs);

	/** requires an api initialized or not */
	int (*api_require_api)(
		afb_api_x4_t api,
		const char *name,
		int initialized);

	/** indicate provided classes of the api */
	int (*api_class_provide)(
		afb_api_x4_t api,
		const char *name);

	/** indicate required classes of the api */
	int (*api_class_require)(
		afb_api_x4_t api,
		const char *name);

	/** delete the api */
	int (*api_delete)(
		afb_api_x4_t api);

/*-- MISC ------------------------------------------*/

	/** creation of a new api*/
	int (*create_api)(
		afb_api_x4_t root,
		afb_api_x4_t *newapi,
		const char *apiname,
		const char *info,
		int noconcurrency,
		afb_api_callback_x4_t mainctl,
		void *closure);

	/** queue a job */
	int (*job_queue)(
		afb_api_x4_t root,
		void (*callback)(int signum, void *arg),
		void *argument,
		void *group,
		int timeout);

	/** add an alias */
	int (*alias_api)(
		afb_api_x4_t root,
		const char *name,
		const char *as_name);

/*-- PREDEFINED TYPES -----------------------------------*/

	/** predefined type opaque */
	afb_type_x4_t type_opaque;

	/** predefined type stringz */
	afb_type_x4_t type_stringz;

	/** predefined type json */
	afb_type_x4_t type_json;

	/** predefined type json-c */
	afb_type_x4_t type_json_c;

/*-- AFTERWARD ------------------------------------------*/

};