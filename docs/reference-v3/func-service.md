Functions of class **afb_service**
==============================

All the functions of the class **afb_service** use the default api.

All these function are deprecated, try to use functions of class **afb_api** instead.

## afb_service_call

```c
/**
 * @deprecated try to use @ref afb_api_call instead
 *
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * The result of the call is delivered to the 'callback' function with the 'callback_closure'.
 *
 * For convenience, the function calls 'json_object_put' for 'args'.
 * Thus, in the case where 'args' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * The 'callback' receives 5 arguments:
 *  1. 'closure' the user defined closure pointer 'closure',
 *  2. 'object'  a JSON object returned (can be NULL)
 *  3. 'error'   a string not NULL in case of error but NULL on success
 *  4. 'info'    a string handling some info (can be NULL)
 *  5. 'api'     the api
 *
 * @param api      The api name of the method to call
 * @param verb     The verb name of the method to call
 * @param args     The arguments to pass to the method
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call_sync
 */
void afb_service_call(
			const char *api,
			const char *verb,
			struct json_object *args,
			void (*callback)(
					void *closure,
					struct json_object *object,
					const char *error,
					const char * info,
					afb_api_t api),
			void *closure);
```

## afb_service_call_sync

```c
/**
 * @deprecated try to use @ref afb_api_call_sync instead
 *
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * 'result' will receive the response.
 *
 * For convenience, the function calls 'json_object_put' for 'args'.
 * Thus, in the case where 'args' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * @param api      The api name of the method to call
 * @param verb     The verb name of the method to call
 * @param args     The arguments to pass to the method
 * @param object   Where to store the returned object - should call json_object_put on it - can be NULL
 * @param error    Where to store the copied returned error - should call free on it - can be NULL
 * @param info     Where to store the copied returned info - should call free on it - can be NULL
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call
 */
int afb_service_call_sync(
			const char *api,
			const char *verb,
			struct json_object *args,
			struct json_object **object,
			char **error,
			char **info);
```

## afb_service_call_legacy

```c
/**
 * @deprecated try to use @ref afb_api_call instead
 *
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb'
 * in the name of the binding.
 * The result of the call is delivered to the 'callback' function with
 * the 'callback_closure'.
 *
 * For convenience, the function calls 'json_object_put' for 'args'.
 * Thus, in the case where 'args' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * The 'callback' receives 3 arguments:
 *  1. 'closure' the user defined closure pointer 'closure',
 *  2. 'status' a status being 0 on success or negative when an error occurred,
 *  2. 'result' the resulting data as a JSON object.
 *
 * @param api      The api name of the method to call
 * @param verb     The verb name of the method to call
 * @param args     The arguments to pass to the method
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 * @see also 'afb_api_call'
 * @see also 'afb_api_call_sync'
 * @see also 'afb_api_call_sync_legacy'
 * @see also 'afb_req_subcall'
 * @see also 'afb_req_subcall_sync'
 */
void afb_service_call_legacy(
			const char *api,
			const char *verb,
			struct json_object *args,
			void (*callback)(
					void *closure,
					int status,
					struct json_object *result,
					afb_api_t api),
			void *closure);
```

## afb_service_call_sync_legacy

```c
/**
 * @deprecated try to use @ref afb_api_call_sync instead
 *
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the
 * name of the binding. 'result' will receive the response.
 *
 * For convenience, the function calls 'json_object_put' for 'args'.
 * Thus, in the case where 'args' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * @param api      The api name of the method to call
 * @param verb     The verb name of the method to call
 * @param args     The arguments to pass to the method
 * @param result   Where to store the result - should call json_object_put on it -
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see also 'afb_api_call'
 * @see also 'afb_api_call_sync'
 * @see also 'afb_api_call_legacy'
 * @see also 'afb_req_subcall'
 * @see also 'afb_req_subcall_sync'
 */
int afb_service_call_sync_legacy(
			const char *api,
			const char *verb,
			struct json_object *args,
			struct json_object **result);
```
