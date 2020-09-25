/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

/**< @file afb/afb-binding-x4.h */

#pragma once

#if AFB_BINDING_VERSION == 4

#include "afb-binding-x4-itf.h"

/** types ******************************************************************/

typedef afb_api_x4_t     afb_api_t;
typedef afb_req_x4_t     afb_req_t;
typedef afb_event_x4_t   afb_event_t;
typedef afb_data_x4_t    afb_data_t;
typedef afb_type_x4_t    afb_type_t;

typedef afb_data_action_x4_t   afb_data_action_t;
typedef afb_type_flags_x4_t    afb_type_flags_t;
typedef afb_type_converter_x4_t afb_type_converter_t;
typedef afb_type_updater_x4_t afb_type_updater_t;

/** constants ***********************************************************/

#define Afb_Type_Flags_X4_Shareable	Afb_Type_Flags_Shareable
#define Afb_Type_Flags_X4_Streamable	Afb_Type_Flags_Streamable
#define Afb_Type_Flags_X4_Opaque	Afb_Type_Flags_Opaque

/******************************************************************************/

#if !defined(AFB_BINDING_NO_ROOT) /* use with caution, see @ref validity-v4 */

/******************************************************************************/
/**
 * Default root binding's api that can be used when no explicit context is
 * available.
 *
 * When @ref afbBindingV4 is defined, this variable records the corresponding
 * api handler. Otherwise, it points to an fake handles that allows logging
 * and api creation.
 *
 * @see afbBindingV4entry
 * @see afbBindingV4
 * @see @ref validity-v4
 */
struct afb_api_x4 *afbBindingV4root __attribute__((weak));

/******************************************************************************/
/**
 * Instanciation of the interface to the binder
 */
#if defined(AFB_BINDING_INDIRECT_CALLS)
const struct afb_binding_x4_itf *afbBindingV4itfptr __attribute__((weak));
#define afbBindingV4itf (*afbBindingV4itfptr)
#else
struct afb_binding_x4_itf afbBindingV4itf __attribute__((weak)); /* can not be const */
#endif

/** DATA ***********************************************************/
/** @defgroup AFB_DATA
 *  @{ */

/**
 * Checks whether the 'data' is valid or not.
 *
 * @param data the data to check
 *
 * @return 0 if not valid or 1 if valid.
 */
static inline
int
afb_data_is_valid(
	afb_data_t data
) {
	return data != 0;
}

/**
 * Create a new data instance of the given type with the given values of
 * pointer, size, dispose and closure.
 *
 * A data is defined by the 5 given values defined here:
 *
 *   - type: is the public type of the data (it can be NULL -not recommanded-)
 *   - pointer: a pointer that can be NULL
 *   - size: a size that can be zero but that must be less than 4294967296 (2**32)
 *   - dispose: a function called with closure when data is no more referenced
 *              it can be NULL if nothing has to be done to release resources
 *   - closure: the closure for the dispose callback
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 * @param dispose  a callback for releasing resources
 * @param closure  a closure for re the callback dispose
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
static inline
int afb_create_data_raw(
	afb_data_t *data,
	afb_type_x4_t type,
	const void *buffer,
	size_t size,
	void (*dispose)(void*),
	void *closure
) {
	return afbBindingV4itf.create_data_raw(data, type, buffer, size, dispose, closure);
}

/**
 * Create a new data instance of the given type by allocating memory
 * of the given size. The allocated memory can eventually
 * be shared.
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 * @param zeroes   if not null, the allocated memory is filled with zeroes
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
static inline
int afb_create_data_copy(
	afb_data_t *data,
	afb_type_x4_t type,
	const void *buffer,
	size_t size
) {
	return afbBindingV4itf.create_data_copy(data, type, buffer, size);
}

/**
 * Create a new data instance of the given type by copying the memory
 * given by pointer and size. The memory where data is copied can eventually
 * be shared.
 *
 * This function is equivalent to:
 *
 *    if (afb_create_data_alloc(req, data, type, &ptr, size, 0) >= 0)
 *        memcpy(ptr, pointer, size);
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
static inline
int afb_create_data_alloc(
	afb_data_t *data,
	afb_type_x4_t type,
	void **pointer,
	size_t size,
	int zeroes
) {
	return afbBindingV4itf.create_data_alloc(data, type, pointer, size, zeroes);
}

/**
 * Increase the count of references of 'data'
 *
 * @param data the data
 *
 * @return the data
 */
static inline
afb_data_t
afb_data_addref(
	afb_data_t data
) {
	return afbBindingV4itf.data_addref(data);
}

/**
 * Decrease the count of references of 'data'.
 * Call this function when the data is no more used.
 * It destroys the data when the reference count falls to zero.
 *
 * @param data the data
 */
static inline
void
afb_data_unref(
	afb_data_t data
) {
	afbBindingV4itf.data_unref(data);
}

/**
 * Get a new instance of the 'data' converted to the 'type'
 *
 * If a data is returned (no error case), it MUST be released
 * using afb_data_unref.
 *
 * When type is NULL, the function returns the current data
 * with a reference count incremented by one.
 *
 * @param data   the original data
 * @param type   the expected type
 * @param result location for storing the result of the conversion
 *
 * @return 0 in case of success or a negative value indication the error.
 */
static inline
int
afb_data_convert(
	afb_data_t data,
	afb_type_x4_t type,
	afb_data_t *result
) {
	return afbBindingV4itf.data_convert(data, type, result);
}

/**
 * Get the type of the data.
 *
 * @param data the data
 */
static inline
afb_type_x4_t
afb_data_type(
	afb_data_t data
) {
	return afbBindingV4itf.data_type(data);
}

/**
 * Get a mutable pointer to the data and also its size
 *
 * @param data the data
 * @param pointer if not NULL address where to store the pointer
 * @param size if not NULL address where to store the size
 *
 * @return 0 in case of success or -1 in case of error
 */
static inline
int
afb_data_get_mutable(
	afb_data_t data,
	void **pointer,
	size_t *size
) {
	return afbBindingV4itf.data_get_mutable(data, pointer, size);
}

/**
 * Get a mutable pointer to the data.
 * Getting a mutable pointer has the effect of
 * notifying that the data changed.
 *
 * @param data the data
 *
 * @return the pointer (can be NULL)
 */
static inline
int
afb_data_get_constant(
	afb_data_t data,
	const void **pointer,
	size_t *size
) {
	return afbBindingV4itf.data_get_constant(data, pointer, size);
}

/**
 * Get the size of the data
 *
 * @param data the data
 *
 * @return the size of the data, can be zero
 */
static inline
size_t
afb_data_size(
	afb_data_t data
) {
	size_t sz;
	afb_data_get_constant(data, 0, &sz);
	return sz;
}

/**
 * Get a read only pointer to the data
 *
 * @param data the data
 *
 * @return the buffer (can be NULL)
 */
static inline
const void *
afb_data_ro_pointer(
	afb_data_t data
) {
	const void *ptr;
	afb_data_get_constant(data, &ptr, 0);
	return ptr;
}

/**
 * Get a read/write pointer to the data
 *
 * @param data the data
 *
 * @return the buffer (can be NULL)
 */
static inline
void *
afb_data_rw_pointer(
	afb_data_t data
) {
	void *ptr;
	afb_data_get_mutable(data, &ptr, 0);
	return ptr;
}

/**
 */
static inline
void
afb_data_notify_changed(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Notify_Changed);
}

/**
 */
static inline
int
afb_data_is_volatile(
	afb_data_t data
) {
	return afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Is_Volatile);
}

/**
 */
static inline
void
afb_data_set_volatile(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Set_Volatile);
}

/**
 */
static inline
void
afb_data_set_not_volatile(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Set_Not_Volatile);
}

/**
 */
static inline
int
afb_data_is_constant(
	afb_data_t data
) {
	return afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Is_Constant);
}

/**
 */
static inline
void
afb_data_set_constant(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Set_Constant);
}

/**
 */
static inline
void
afb_data_set_not_constant(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Set_Not_Constant);
}

/**
 */
static inline
void
afb_data_lock_read(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Lock_Read);
}

/**
 */
static inline
int
afb_data_try_lock_read(
	afb_data_t data
) {
	return afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Try_Lock_Read);
}

/**
 */
static inline
void
afb_data_lock_write(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Lock_Write);
}

/**
 */
static inline
int
afb_data_try_lock_write(
	afb_data_t data
) {
	return afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Try_Lock_Write);
}

/**
 */
static inline
void
afb_data_unlock(
	afb_data_t data
) {
	afbBindingV4itf.data_control(data, Afb_Data_Action_x4_Unlock);
}

/**
 */
static inline
int
afb_data_update(
	afb_data_t data,
	afb_data_t value
) {
	return afbBindingV4itf.data_update(data, value);
}

/**
 */
static inline
void
afb_data_assign(
	afb_data_t *data,
	afb_data_t value
) {
	afb_data_t old = *data;
	*data = value;
	afb_data_unref(old);
}

/*  @} */
/** DATA ARRAY ***********************************************************/
/** @defgroup AFB_DATA_ARRAY
 *  @{ */


/**
 * Increase the count of references of the array 'array'
 * that has 'count' data.
 *
 * @param count the count of data in the array
 * @param array the array of data
 */
static inline
void
afb_data_array_addref(
	unsigned count,
	afb_data_t const *array
) {
	while (count--)
		afb_data_addref(*array++);
}

/**
 * Decrease the count of references of the array 'array'
 * that has 'count' data.
 *
 * Call this function when the data is no more used.
 * It destroys the data when the reference count falls to zero.
 *
 * @param count the count of data in the array
 * @param array the array of data
 */
static inline
void
afb_data_array_unref(
	unsigned count,
	afb_data_t const *array
) {
	while (count--)
		afb_data_unref(*array++);
}

/**
 * Get a new instances of items of 'array_data' converted to the
 * corresponding types of 'array_type'
 *
 * If a data are returned (no error case), they MUST be released
 * using afb_data_unref.
 *
 * @param count        count of items in the arrays
 * @param array_data   array of original data
 * @param array_type   array of expected types
 * @param array_result array for storing result of conversions
 *
 * @return 0 in case of success or a negative value indication the error.
 */
static inline
int
afb_data_array_convert(
	unsigned count,
	afb_data_t const * array_data,
	afb_type_x4_t const * array_type,
	afb_data_t *array_result
) {
	int rc = 0;
	unsigned index = 0;

	while (rc >= 0 && index < count) {
		rc = afb_data_convert(array_data[index], array_type[index], &array_result[index]);
		if (rc >= 0)
			index++;
		else {
			while (index)
				afb_data_unref(array_result[--index]);
			while (index < count)
				array_result[index++] = 0;
		}
	}
	return rc;
}

/** @} */
/** REQ ***********************************************************/
/** @defgroup AFB_REQ
 *  @{ */

/**
 * Get the logmask of the 'req'.
 *
 * @param req the req whose logmask is to be returned
 *
 * @return logmask of the req.
 */
static inline
int afb_req_logmask(
	afb_req_t req)
{
	return afbBindingV4itf.req_logmask(req);
}

/**
 * Is the log message of 'level (as defined for syslog) required for the
 * request 'req'?
 *
 * @param req the request
 * @param level the level to check as defined for syslog:
 *
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @return 0 if not required or a value not null if required
 *
 * @see syslog
 */
static inline
int
afb_req_wants_log_level(
	afb_req_t req,
	int level
) {
	return AFB_SYSLOG_MASK_WANT(afb_req_logmask(req), level);
}

/**
 * Checks whether the request 'req' is valid or not.
 *
 * @param req the request to check
 *
 * @return 0 if not valid or 1 if valid.
 */
static inline
int
afb_req_is_valid(
	afb_req_t req
) {
	return !!req;
}

/**
 * Retrieves the api that serves the request
 *
 * @param req the request whose serving api is queried
 *
 * @return the api serving the request
 */
static inline
afb_api_t
afb_req_get_api(
	afb_req_t req
) {
	return afbBindingV4itf.req_api(req);
}

/**
 * Retrieves the callback data of the verb. This callback data is set
 * when the verb is created.
 *
 * @param req whose verb vcbdata is queried
 *
 * @return the callback data attached to the verb description
 */
static inline
void *
afb_req_get_vcbdata(
	afb_req_t req
) {
	return afbBindingV4itf.req_vcbdata(req);
}

/**
 * Retrieve the name of the called api.
 *
 * @param req the request
 *
 * @return the name of the called api
 *
 * @see afb_api_x3_add_alias
 */
static inline
const char *
afb_req_get_called_api(
	afb_req_t req
) {
	return afbBindingV4itf.req_called_api(req);
}

/**
 * Retrieve the name of the called verb
 *
 * @param req the request
 *
 * @return the name of the called verb
 */
static inline
const char *
afb_req_get_called_verb(
	afb_req_t req
) {
	return afbBindingV4itf.req_called_verb(req);
}

/**
 * Increments the count of references of 'req'.
 *
 * @param req the request
 *
 * @return returns the request req
 */
static inline
afb_req_t
afb_req_addref(
	afb_req_t req
) {
	return afbBindingV4itf.req_addref(req);
}

/**
 * Decrement the count of references of 'req'.
 *
 * @param req the request
 */
static inline
void
afb_req_unref(
	afb_req_t req
) {
	afbBindingV4itf.req_unref(req);
}

/**
 * Send associated to 'req' a message described by 'fmt' and its 'args'
 * to the journal for the verbosity 'level'.
 *
 * 'file', 'line' and 'func' are indicators of position of the code in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @param req the request
 * @param level the level of the message
 * @param file the source filename that emits the message or NULL
 * @param line the line number in the source filename that emits the message
 * @param func the name of the function that emits the message or NULL
 * @param fmt the message format as for printf
 * @param args the arguments to the format 'fmt'
 *
 * @see printf
 * @see afb_req_verbose
 */
static inline
void
afb_req_vverbose(
	afb_req_t req,
	int level, const char *file,
	int line,
	const char * func,
	const char *fmt,
	va_list args
) {
	afbBindingV4itf.req_vverbose(req, level, file, line, func, fmt, args);
}

/**
 * Send associated to 'req' a message described by 'fmt' and following parameters
 * to the journal for the verbosity 'level'.
 *
 * 'file', 'line' and 'func' are indicators of position of the code in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @param req the request
 * @param level the level of the message
 * @param file the source filename that emits the message or NULL
 * @param line the line number in the source filename that emits the message
 * @param func the name of the function that emits the message or NULL
 * @param fmt the message format as for printf
 * @param ... the arguments of the format 'fmt'
 *
 * @see printf
 * @see afb_req_vverbose
 */
__attribute__((format(printf, 6, 7)))
static inline
void
afb_req_verbose(
	afb_req_t req,
	int level, const char *file,
	int line,
	const char * func,
	const char *fmt,
	...
) {
	va_list args;
	va_start(args, fmt);
	afb_req_vverbose(req, level, file, line, func, fmt, args);
	va_end(args);
}

/**
 * Manage the pointer stored by the binding for the client session of 'req'.
 *
 * If no previous pointer is stored or if 'replace' is not zero, a new value
 * is generated using the function 'create_context' called with the 'closure'.
 * If 'create_context' is NULL the generated value is 'closure'.
 *
 * When a value is created, the function 'free_context' is recorded and will
 * be called (with the created value as argument) to free the created value when
 * it is not more used.
 *
 * This function is atomic: it ensures that 2 threads will not race together.
 *
 * @param req the request
 * @param replace if not zero an existing value is replaced
 * @param create_context the creation function or NULL
 * @param free_context the destroying function or NULL
 * @param closure the closure to the creation function
 *
 * @return the stored value
 */
static inline
void *
afb_req_context(
	afb_req_t req,
	int replace,
	void *(*create_context)(void *closure),
	void (*free_context)(void*),
	void *closure
) {
	return afbBindingV4itf.req_cookie(req, replace, create_context, free_context, closure);
}

/**
 * Gets the pointer stored by the binding for the session of 'req'.
 * When the binding has not yet recorded a pointer, NULL is returned.
 *
 * Shortcut for: afb_req_context(req, 0, NULL, NULL, NULL)
 *
 * @param req the request
 *
 * @return the previously stored value
 */
static inline
void *
afb_req_context_get(
	afb_req_t req
) {
	return afb_req_context(req, 0, 0, 0, 0);
}

/**
 * Stores for the binding the pointer 'context' to the session of 'req'.
 * The function 'free_context' will be called when the session is closed
 * or if binding stores an other pointer.
 *
 * Shortcut for: afb_req_context(req, 1, NULL, free_context, context)
 *
 *
 * @param req the request
 * @param context the context value to store
 * @param free_context the cleaning function for the stored context (can be NULL)
 */
static inline
void
afb_req_context_set(
	afb_req_t req,
	void *context,
	void (*free_context)(void*)
) {
	afb_req_context(req, 1, 0, free_context, context);
}

/**
 * Frees the pointer stored by the binding for the session of 'req'
 * and sets it to NULL.
 *
 * Shortcut for: afb_req_context_set(req, NULL, NULL)
 *
 * @param req the request
 */
static inline
void
afb_req_context_clear(
	afb_req_t req
) {
	afb_req_context(req, 1, 0, 0, 0);
}

/**
 * Closes the session associated with 'req'
 * and delete all associated contexts.
 *
 * @param req the request
 */
static inline
void
afb_req_session_close(
	afb_req_t req
) {
	afbBindingV4itf.req_session_close(req);
}

/**
 * Sets the level of assurance of the session of 'req'
 * to 'level'. The effect of this function is subject of
 * security policies.
 *
 * @param req the request
 * @param level of assurance from 0 to 7
 *
 * @return 0 on success or -1 if failed.
 */
static inline
int
afb_req_session_set_LOA(
	afb_req_t req,
	unsigned level
) {
	return afbBindingV4itf.req_session_set_LOA(req, level);
}

/**
 * Establishes for the client link identified by 'req' a subscription
 * to the 'event'.
 *
 * Establishing subscription MUST be called BEFORE replying to the request.
 *
 * @param req the request
 * @param event the event to subscribe
 *
 * @return 0 in case of successful subscription or -1 in case of error.
 */
static inline
int
afb_req_subscribe(
	afb_req_t req,
	afb_event_t event
) {
	return afbBindingV4itf.req_subscribe(req, event);
}

/**
 * Revokes the subscription established to the 'event' for the client
 * link identified by 'req'.
 * Returns 0 in case of successful subscription or -1 in case of error.
 *
 * Revoking subscription MUST be called BEFORE replying to the request.
 *
 * @param req the request
 * @param event the event to revoke
 *
 * @return 0 in case of successful subscription or -1 in case of error.
 */
static inline
int
afb_req_unsubscribe(
	afb_req_t req,
	afb_event_t event
) {
	return afbBindingV4itf.req_unsubscribe(req, event);
}

/**
 * Check whether the 'permission' is granted or not to the client
 * identified by 'req'.
 *
 * @param req the request
 * @param permission string to check
 * @param callback that receive the result of the check
 * @param closure closure for the callback
 */
static inline
void
afb_req_check_permission(
	afb_req_t req,
	const char *permission,
	void (*callback)(void *closure, int status, afb_req_t req),
	void *closure
) {
	afbBindingV4itf.req_check_permission(req, permission, callback, closure);
}

/**
 * Get informations about the client of the
 * request 'req'.
 *
 * Returns an object with client informations:
 *  {
 *    "pid": int, "uid": int, "gid": int,
 *    "label": string, "id": string, "user": string,
 *    "uuid": string, "LOA": int
 *  }
 *
 * If some of this information can't be computed, the field of the return
 * object will not be set at all.
 *
 * @param req the request
 *
 * @return a JSON object that must be freed using @ref json_object_put
 */
static inline
struct json_object *
afb_req_get_client_info(
	afb_req_t req
) {
	return afbBindingV4itf.req_get_client_info(req);
}

/**
 * Get the parameters of the request
 *
 * @param req      the request
 * @param params   pointer where to store the address to the array of data parameters
 *                 (can be NULL)
 *
 * @return the count of parameters
 */
static inline
unsigned
afb_req_parameters(
	afb_req_t req,
	afb_data_t const **params
) {
	return afbBindingV4itf.req_parameters(req, params);
}

/**
 * Reply to the request
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'replies'. Thus, in the case where some data of 'replies' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * @param req      the request
 * @param status   an integer status
 * @param nreplies count of replied data
 * @param replies  array of the replied data if any (can be NULL when nparams is 0)
 */
static inline
void
afb_req_reply(
	afb_req_t req,
	int status,
	unsigned nreplies,
	afb_data_t const *replies
) {
	afbBindingV4itf.req_reply(req, status, nreplies, replies);
}

/**
 * Calls the 'verb' of the 'api' with the parameters 'params' and 'verb' in the name of the binding.
 * The result of the call is delivered to the 'callback' function with the 'callback_closure'.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * The 'callback' receives 5 arguments:
 *  1. 'closure'  the user defined closure pointer 'closure',
 *  2. 'status'   the replied status
 *  3. 'nreplies' the count of replied data
 *  4. 'replies'  the array of replied data
 *  5. 'req'      the req
 *
 * NOTE: For convenience, the data received by the callback function in the
 * array 'replies' are released automatically after the return of the
 * callback. Thus, in the case where some data of 'replies' should remain
 * available after the end of the callback, the function 'afb_data_addref'
 * shall be used on such items to keep them alive after end of callback.
 *
 * @param req      The request
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param flags    The bit field of flags for the subcall as defined by @ref afb_req_subcall_flags
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 * @see also 'afb_req_subcall_sync'
 */
static inline
void
afb_req_subcall(
	afb_req_t req,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int flags,
	void (*callback)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_t const replies[],
		afb_req_t req),
	void *closure
) {
	afbBindingV4itf.req_subcall(req, apiname, verbname, nparams, params, flags, callback, closure);
}

/**
 * Makes a call to the method of name 'api' / 'verb' with the parameters 'params'.
 * This call is made in the context of the request 'req'.
 * This call is synchronous, it waits untill completion of the request.
 * It returns 0 on success or a negative value on error answer.
 *
 * At call, the value pointed by nreplies must be the count of data
 * of the array replies. The values in replies are not used at call and
 * can be anything of anyvalue, the caller does not care or use it.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * At return, status receives the reply status, nreplies receives the count
 * of value replied and stored in the array replies.
 *
 * NOTE: The data returned by the call in the array replies MUST be released
 * by the caller using 'afb_data_unref' or 'afb_data_array_unref'.
 *
 * If the array 'replies' is too small to receive all the replied data, the
 * reply is truncated to the count of 'replies' (indicated by 'nreplies'),
 * the remaining data replied are ignored.
 *
 * @param req      The request
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param flags    The bit field of flags for the subcall as defined by @ref afb_req_subcall_flags
 * @param status   the replied status of the call
 * @param nreplies at call, size of params, at return, count of data in replies
 * @param replies  where to store the replied data
 *
 * @return 0 in case of success or -1 in case of error
 */
static inline
int
afb_req_subcall_sync(
	afb_req_t req,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int flags,
	int *status,
	unsigned *nreplies,
	afb_data_t replies[]
) {
	return afbBindingV4itf.req_subcall_sync(req, apiname, verbname, nparams, params, flags, status, nreplies, replies);
}

/** @} */

/** EVENT ***********************************************************/
/** @defgroup AFB_EVENT
 *  @{ */

/**
 * Checks whether the 'event' is valid or not.
 *
 * @param event the event to check
 *
 * @return 0 if not valid or 1 if valid.
 */
static inline
int
afb_event_is_valid(
	afb_event_t event
) {
	return !!event;
}

/**
 * Increases the count of references to 'event'
 *
 * @param event the event
 *
 * @return the event
 */
static inline
afb_event_t
afb_event_addref(
	afb_event_t event
) {
	return afbBindingV4itf.event_addref(event);
}

/**
 * Decrease the count of references to 'event'.
 * Call this function when the evenid is no more used.
 * It destroys the event when the reference count falls to zero.
 *
 * @param event the event
 */
static inline
void
afb_event_unref(
	afb_event_t event
) {
	afbBindingV4itf.event_unref(event);
}

/**
 * Gets the name associated to 'event'.
 *
 * @param event the event whose name is requested
 *
 * @return the name of the event
 *
 * The returned name can be used until call to 'afb_event_unref'.
 * It shouldn't be freed.
 */
static inline
const char *
afb_event_name(
	afb_event_t event
) {
	return afbBindingV4itf.event_name(event);
}

/**
 * Pushes an event of 'event' with the 'data' to its observers.
 * 'data' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * @param event    the event to push
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @Return
 *   *  1 if at least one client listen for the event
 *   *  0 if no more client listen for the event
 *   * -1 in case of error (the event can't be delivered)
 */
static inline
int
afb_event_push(
	afb_event_t event,
	unsigned nparams,
	afb_data_t const params[]
) {
	return afbBindingV4itf.event_push(event, nparams, params);
}

/**
 * Broadcasts widely an event of 'event' with the 'data.
 * 'data' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * @param event    the event to broadcast
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @return 0 in case of success or -1 in case of error
 */
static inline
int
afb_event_broadcast(
	afb_event_t event,
	unsigned nparams,
	afb_data_t const params[]
) {
	return afbBindingV4itf.event_broadcast(event, nparams, params);
}

/** @} */

/** TYPE ***********************************************************/
/** @defgroup AFB_TYPE
 *  @{ */

/**
 * Lookup for a type
 *
 *
 * @return 0 in case of success or a negative error code
 */
static inline
int afb_type_lookup(
	afb_type_x4_t *type,
	const char *name
) {
	return afbBindingV4itf.type_lookup(type, name);
}

/**
 * Register a type
 *
 *
 * @return 0 in case of success or a negative error code
 */
static inline
int afb_type_register(
	afb_type_t *type,
	const char *name,
	afb_type_flags_t flags
) {
	return afbBindingV4itf.type_register(type, name, flags);
}

/**  */
static inline
const char *
afb_type_name(
	afb_type_x4_t type
) {
	return afbBindingV4itf.type_name(type);
}

/**  */
static inline
int
afb_type_set_family(
	afb_type_t type,
	afb_type_x4_t family
) {
	return afbBindingV4itf.type_set_family(type, family);
}

/** add convert to */
static inline
int
afb_type_add_convert_to(
	afb_type_t type,
	afb_type_x4_t to_type,
	afb_type_converter_t converter,
	void *closure
) {
	return afbBindingV4itf.type_add_converter(type, to_type, converter, closure);
}

/** add convert from */
static inline
int
afb_type_add_convert_from(
	afb_type_t type,
	afb_type_x4_t from_type,
	afb_type_converter_t converter,
	void *closure
) {
	return afbBindingV4itf.type_add_converter(from_type, type, converter, closure);
}

/** add update to */
static inline
int
afb_type_add_update_to(
	afb_type_t type,
	afb_type_x4_t to_type,
	afb_type_updater_t updater,
	void *closure
) {
	return afbBindingV4itf.type_add_updater(type, to_type, updater, closure);
}

/** add update from */
static inline
int
afb_type_add_update_from(
	afb_type_t type,
	afb_type_x4_t from_type,
	afb_type_updater_t updater,
	void *closure
) {
	return afbBindingV4itf.type_add_updater(from_type, type, updater, closure);
}

/** @} */
/** API ***********************************************************/
/** @defgroup AFB_API
 *  @{ */

/**
 * Get the logmask of the 'api'.
 *
 * @param api the api whose logmask is to be returned
 *
 * @return logmask of the api.
 */
static inline
int afb_api_logmask(
	afb_api_t api)
{
	return afbBindingV4itf.api_logmask(api);
}

/**
 * Get the name of the 'api'.
 *
 * @param api the api whose name is to be returned
 *
 * @return the name of the api.
 *
 * The returned value must not be changed nor freed.
 */
static inline
const char *afb_api_name(
	afb_api_t api)
{
	return afbBindingV4itf.api_name(api);
}

/**
 * Get the "userdata" pointer of the 'api'
 *
 * @param api the api whose user's data is to be returned
 *
 * @return the user's data  pointer of the api.
 *
 * @see afb_api_set_userdata
 */
static inline
void *afb_api_get_userdata(
	afb_api_t api)
{
	return afbBindingV4itf.api_get_userdata(api);
}

/**
 * Set the "userdata" pointer of the 'api' to 'value'
 *
 * @param api   the api whose user's data is to be set
 * @param value the data to set
 *
 * @return the previous value
 *
 * @see afb_api_get_userdata
 */
static inline
void *afb_api_set_userdata(
	afb_api_t api,
	void *value)
{
	return afbBindingV4itf.api_set_userdata(api, value);
}

/**
 * Is the log message of 'level (as defined for syslog) required for the api?
 *
 * @param api   the api to check
 * @param level the level to check as defined for syslog:
 *
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @return 0 if not required or a value not null if required
 *
 * @see syslog
 */
static inline
int afb_api_wants_log_level(
	afb_api_t api,
	int level)
{
	return AFB_SYSLOG_MASK_WANT(afb_api_logmask(api), level);
}

/**
 * Send to the journal with the logging 'level' a message described
 * by 'fmt' applied to the va-list 'args'.
 *
 * 'file', 'line' and 'func' are indicators of code position in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
 *
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @param api the api that collects the logging message
 * @param level the level of the message
 * @param file the source file that logs the messages or NULL
 * @param line the line in the source file that logs the message
 * @param func the name of the function in the source file that logs (or NULL)
 * @param fmt the format of the message as in printf
 * @param args the arguments to the format string of the message as a va_list
 *
 * @see syslog
 * @see printf
 */
static inline
void afb_api_vverbose(
	afb_api_t api,
	int level,
	const char *file,
	int line,
	const char *func,
	const char *fmt,
	va_list args)
{
	afbBindingV4itf.api_vverbose(api, level, file, line, func, fmt, args);
}

/**
 * Send to the journal with the log 'level' a message described
 * by 'fmt' and following parameters.
 *
 * 'file', 'line' and 'func' are indicators of position of the code in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @param api the api that collects the logging message
 * @param level the level of the message
 * @param file the source file that logs the messages or NULL
 * @param line the line in the source file that logs the message
 * @param func the name of the function in the source file that logs (or NULL)
 * @param fmt the format of the message as in printf
 * @param ... the arguments to the format string of the message
 *
 * @see syslog
 * @see printf
 */
__attribute__((format(printf, 6, 7)))
static inline
void afb_api_verbose(
	afb_api_t api,
	int level,
	const char *file,
	int line,
	const char *func,
	const char *fmt,
	...)
{
	va_list args;
	va_start(args, fmt);
	afbBindingV4itf.api_vverbose(api, level, file, line, func, fmt, args);
	va_end(args);
}

/**
 * Broadcasts widely the event of 'name' with the data 'object'.
 * 'object' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * Calling this function is forbidden during preinit.
 *
 * The event sent as the name API/name where API is the name of the
 * api.
 *
 * @param api      the api that broadcast the event
 * @param name     the event name suffix
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @return the count of clients that received the event.
 */
static inline
int afb_api_broadcast_event(
	afb_api_t api,
	const char *name,
	unsigned nparams,
	afb_data_t const params[])
{
	return afbBindingV4itf.api_event_broadcast(api, name, nparams, params);
}

/**
 * Check that it requires the API of 'name'.
 * If 'initialized' is not zero it request the API to be
 * initialized, implying its initialization if needed.
 *
 * Calling this function is only allowed within init.
 *
 * A single request allows to require multiple apis.
 *
 * @param api the api that requires the other api by its name
 * @param name a space separated list of required api names
 * @param initialized if zero, the api is just required to exist. If not zero,
 * the api is required to exist and to be initialized at return of the call
 * (initializing it if needed and possible as a side effect of the call).
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
static inline
int afb_api_require_api(
	afb_api_t api,
	const char *name,
	int initialized)
{
	return afbBindingV4itf.api_require_api(api, name, initialized);
}

/**
 * Creates an event of 'name' and returns it.
 *
 * Calling this function is only forbidden during preinit.
 *
 * See afb_event_is_valid to check if there is an error.
 *
 * The event created as the name API/name where API is the name of the
 * api.
 *
 * @param api the api that creates the event
 * @param name the event name suffix
 *
 * @return the created event. Use the function afb_event_is_valid to check
 * whether the event is valid (created) or not (error as reported by errno).
 *
 * @see afb_event_is_valid
 */
static inline
int afb_api_new_event(
	afb_api_t api,
	const char *name,
	afb_event_t *event)
{
	return afbBindingV4itf.api_new_event(api, name, event);
}

/**
 * Add one verb to the dynamic set of the api
 *
 * @param api the api to change
 * @param verb name of the verb
 * @param info brief description of the verb, can be NULL
 * @param callback callback function implementing the verb
 * @param vcbdata data for the verb callback, available through req
 * @param auth required authorization, can be NULL
 * @param session authorization and session requirements of the verb
 * @param glob is the verb glob name
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_verb_v3
 * @see afb_api_del_verb
 * @see afb_api_set_verbs_v3
 * @see fnmatch for matching names using glob
 */
static inline
int afb_api_add_verb(
	afb_api_t api,
	const char *verb,
	const char *info,
	void (*callback)(afb_req_t req, unsigned nparams, afb_data_t const params[]),
	void *vcbdata,
	const struct afb_auth *auth,
	uint32_t session,
	int glob)
{
	return afbBindingV4itf.api_add_verb(api, verb, info, callback, vcbdata, auth, session, glob);
}

/**
 * Delete one verb from the dynamic set of the api
 *
 * @param api the api to change
 * @param verb name of the verb to delete
 * @param vcbdata if not NULL will receive the vcbdata of the deleted verb
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_add_verb
 */
static inline
int afb_api_del_verb(
	afb_api_t api,
	const char *verb,
	void **vcbdata)
{
	return afbBindingV4itf.api_del_verb(api, verb, vcbdata);
}

/**
 * Seal the api. After a call to this function the api can not be modified
 * anymore.
 *
 * @param api the api to be sealed
 */
static inline
void afb_api_seal(
	afb_api_t api)
{
	afbBindingV4itf.api_seal(api);
}

/**
 * Set the verbs of the 'api' using description of verbs of the api v2
 *
 * @param api the api that will get the verbs
 * @param verbs the array of verbs to add terminated with an item with name=NULL
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_verb_v3
 * @see afb_api_add_verb
 * @see afb_api_del_verb
 */
static inline
int afb_api_set_verbs(
	afb_api_t api,
	const struct afb_verb_v4 *verbs)
{
	return afbBindingV4itf.api_set_verbs(api, verbs);
}

/**
 * Add a specific event handler for the api
 *
 * The handler callback is called when an event matching the given pattern
 * is received (it is received if broadcasted or after subscription through
 * a call or a subcall).
 *
 * The handler callback receive 4 arguments:
 *
 *  - the closure given here
 *  - the event full name
 *  - the companion JSON object of the event
 *  - the api that subscribed the event
 *
 * @param api the api that creates the handler
 * @param pattern the global pattern of the event to handle
 * @param callback the handler callback function
 * @param closure the closure of the handler
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_on_event
 * @see afb_api_event_handler_del
 */
static inline
int afb_api_event_handler_add(
	afb_api_t api,
	const char *pattern,
	void (*callback)(
		void *,
		const char*,
		unsigned,
		afb_data_t const[],
		afb_api_t),
	void *closure)
{
	return afbBindingV4itf.api_event_handler_add(api, pattern, callback, closure);
}

/**
 * Delete a specific event handler for the api
 *
 * @param api the api that the handler belongs to
 * @param pattern the global pattern of the handler to remove
 * @param closure if not NULL it will receive the closure set to the handler
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_on_event
 * @see afb_api_event_handler_add
 */
static inline
int afb_api_event_handler_del(
	afb_api_t api,
	const char *pattern,
	void **closure)
{
	return afbBindingV4itf.api_event_handler_del(api, pattern, closure);
}

/**
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * The result of the call is delivered to the 'callback' function with the 'callback_closure'.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * The 'callback' receives 5 arguments:
 *  1. 'closure' the user defined closure pointer 'closure',
 *  2. 'status'   the replied status
 *  3. 'nreplies' the count of replied data
 *  4. 'replies'  the array of replied data
 *  5. 'api'      the api
 *
 * NOTE: For convenience, the data received by the callback function in the
 * array 'replies' are released automatically after the return of the
 * callback. Thus, in the case where some data of 'replies' should remain
 * available after the end of the callback, the function 'afb_data_addref'
 * shall be used on such items to keep them alive after end of callback.
 *
 * @param api      The api that makes the call
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call_sync
 */
static inline
void afb_api_call(
	afb_api_t api,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	void (*callback)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_t const replies[],
		afb_api_t api),
	void *closure)
{
	afbBindingV4itf.api_call(api, apiname, verbname, nparams, params, callback, closure);
}

/**
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * 'result' will receive the response.
 *
 * At call, the value pointed by nreplies must be the count of data
 * of the array replies. The values in replies are not used at call and
 * can be anything of anyvalue, the caller does not care or use it.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * At return, status receives the reply status, nreplies receives the count
 * of value replied and stored in the array replies.
 *
 * NOTE: The data returned by the call in replies MUST be released by the caller
 * using 'afb_data_unref'.
 *
 * If the array 'replies' is too small to receive all the replied data, the
 * reply is truncated to the count of 'replies' (indicated by 'nreplies'),
 * the remaining data replied are ignored.
 *
 * @param api      The api that makes the call
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param status   the replied status of the call
 * @param nreplies at call, size of params, at return, count of data in replies
 * @param replies  where to store the replied data
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call
 */
static inline
int afb_api_call_sync(
	afb_api_t api,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int *status,
	unsigned *nreplies,
	afb_data_t replies[])
{
	return afbBindingV4itf.api_call_sync(api,
			apiname, verbname, nparams, params,
			status, nreplies, replies);
}

/**
 * Tells that the api provides a class of features. Classes are intended to
 * allow ordering of initializations: apis that provides a given class are
 * initialized before apis requiring it.
 *
 * This function is only valid during the pre-initialization stage.
 *
 * @param api  the api that provides the classes
 * @param name a space separated list of the names of the provided classes
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_require_class
 */
static inline
int afb_api_provide_class(
	afb_api_t api,
	const char *name)
{
	return afbBindingV4itf.api_class_provide(api, name);
}

/**
 * Tells that the api requires a set of class features. Classes are intended to
 * allow ordering of initializations: apis that provides a given class are
 * initialized before apis requiring it.
 *
 * This function is only valid during the pre-initialization stage.
 *
 * @param api  the api that requires the classes
 * @param name a space separated list of the names of the required classes
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_provide_class
 */
static inline
int afb_api_require_class(
	afb_api_t api,
	const char *name)
{
	return afbBindingV4itf.api_class_require(api, name);
}

/**
 * Delete the given api.
 *
 * It is of the responsibility of the caller to don't used the deleted api
 * anymore after this function returned.
 *
 * @param api the api to delete
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_new_api
 */
static inline
int afb_api_delete(
	afb_api_t api)
{
	return afbBindingV4itf.api_delete(api);
}

/**
 * Settings of the api.
 *
 * Get the settings of the API. The settings are recorded
 * as a JSON object. The returned object should not be modified.
 * It MUST NOT be released using json_object_put.
 *
 * @param api the api whose settings are required
 *
 * @returns The setting object.
 */
static inline
struct json_object *afb_api_settings(
	afb_api_t api)
{
	return afbBindingV4itf.api_settings(api);
}


/** MISC ***********************************************************/
/** @defgroup AFB_MISC
 *  @{ */

/**
 * Creates a new api of name 'apiname' briefly described by 'info' (that can
 * be NULL).
 *
 * When the pre-initialization function is given, it is a function that
 * receives 2 parameters:
 *
 *  - the closure as given in the call
 *  - the created api that can be initialised
 *
 * This pre-initialization function must return a negative value to abort
 * the creation of the api. Otherwise, it returns a non-negative value to
 * continue.
 *
 * @param api the api that creates the other one
 * @param apiname the name of the new api
 * @param info the brief description of the new api (can be NULL)
 * @param noconcurrency zero or not zero whether the new api is reentrant or not
 * @param preinit the pre-initialization function if any (can be NULL)
 * @param closure the closure for the pre-initialization preinit
 *
 * @return the created api in case of success or NULL on error
 *
 * @see afb_api_delete_api
 */
static inline
int afb_create_api(
	afb_api_t *newapi,
	const char *apiname,
	const char *info,
	int noconcurrency,
	int (*mainctl)(afb_api_t, afb_ctlid_t, afb_ctlarg_t),
	void *closure)
{
	return afbBindingV4itf.create_api(afbBindingV4root, newapi, apiname, info, noconcurrency, mainctl, closure);
}

/**
 * Queue the job defined by 'callback' and 'argument' for being executed asynchronously
 * in this thread (later) or in an other thread.
 *
 * If 'group' is not NULL, the jobs queued with a same value (as the pointer value 'group')
 * are executed in sequence in the order of there submission.
 *
 * If 'timeout' is not 0, it represent the maximum execution time for the job in seconds.
 * At first, the job is called with 0 as signum and the given argument.
 *
 * The job is executed with the monitoring of its time and some signals like SIGSEGV and
 * SIGFPE. When a such signal is catched, the job is terminated and reexecuted but with
 * signum being the signal number (SIGALRM when timeout expired).
 *
 * When executed, the callback function receives 2 arguments:
 *
 *  - int signum: the signal catched if any or zero at the beginning
 *  - void *arg: the parameter 'argument'
 *
 * A typical implementation of the job callback is:
 *
 * ```C
 * void my_job_cb(int signum, void *arg)
 * {
 *	struct myarg_t *myarg = arg;
 *	if (signum)
 *		AFB_API_ERROR(myarg->api, "job interrupted with signal %s", strsignal(signum));
 *	else
 *		really_do_my_job(myarg);
 * }
 * ```
 *
 * @param api the api that queue the job
 * @param callback the job as a callback function
 * @param argument the argument to pass to the queued job
 * @param group the group of the job, NULL if no group
 * @param timeout the timeout of execution of the job
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
static inline
int afb_job_queue(
	void (*callback)(int signum, void *arg),
	void *argument,
	void *group,
	int timeout)
{
	return afbBindingV4itf.job_queue(afbBindingV4root, callback, argument, group, timeout);
}

/**
 * Create an aliased name 'as_name' for the api 'name'.
 * Calling this function is only allowed within preinit.
 *
 * @param api the api that requires the aliasing
 * @param name the api to alias
 * @param as_name the aliased name of the aliased api
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
static inline
int afb_alias_api(
	const char *name,
	const char *as_name)
{
	return afbBindingV4itf.alias_api(afbBindingV4root, name, as_name);
}

/** @} */
/** PREDEFINED TYPE ***********************************************************/
/** @defgroup AFB_PREDEFINED_TYPE
 *  @{ */

#define AFB_PREDEFINED_TYPE_OPAQUE  (afbBindingV4itf.type_opaque)
#define AFB_PREDEFINED_TYPE_STRINGZ (afbBindingV4itf.type_stringz)
#define AFB_PREDEFINED_TYPE_JSON    (afbBindingV4itf.type_json)
#define AFB_PREDEFINED_TYPE_JSON_C  (afbBindingV4itf.type_json_c)

/** @} */
/** MISC ***********************************************************/
/** AFTERWARD ***********************************************************/

/******************************************************************************/

#endif
#endif