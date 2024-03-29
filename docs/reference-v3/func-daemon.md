Functions of class **afb_daemon**
============================

All the functions of the class **afb_daemon** use the default api.
This are internally aliased to the corresponding function of class afb_api_t.

```c
/**
 * Retrieves the common systemd's event loop of AFB
 */
struct sd_event *afb_daemon_get_event_loop();

/**
 * Retrieves the common systemd's user/session d-bus of AFB
 */
struct sd_bus *afb_daemon_get_user_bus();

/**
 * Retrieves the common systemd's system d-bus of AFB
 */
struct sd_bus *afb_daemon_get_system_bus();

/**
 * Broadcasts widely the event of 'name' with the data 'object'.
 * 'object' can be NULL.
 *
 * For convenience, the function calls 'json_object_put' for 'object'.
 * Thus, in the case where 'object' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * Calling this function is only forbidden during preinit.
 *
 * Returns the count of clients that received the event.
 */
int afb_daemon_broadcast_event(
			const char *name,
			struct json_object *object);

/**
 * Creates an event of 'name' and returns it.
 *
 * Calling this function is only forbidden during preinit.
 *
 * See afb_event_is_valid to check if there is an error.
 */
afb_event_t afb_daemon_make_event(
			const char *name);

/**
 * @deprecated use bindings version 3
 *
 * Send a message described by 'fmt' and following parameters
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
 */
void afb_daemon_verbose(
			int level,
			const char *file,
			int line,
			const char * func,
			const char *fmt,
			...);

/**
 * @deprecated use bindings version 3
 *
 * Get the root directory file descriptor. This file descriptor can
 * be used with functions 'openat', 'fstatat', ...
 *
 * Returns the file descriptor or -1 in case of error.
 */
int afb_daemon_rootdir_get_fd();

/**
 * Opens 'filename' within the root directory with 'flags' (see function openat)
 * using the 'locale' definition (example: "jp,en-US") that can be NULL.
 *
 * Returns the file descriptor or -1 in case of error.
 */
int afb_daemon_rootdir_open_locale(
			const char *filename,
			int flags,
			const char *locale);

/**
 * Queue the job defined by 'callback' and 'argument' for being executed asynchronously
 * in this thread (later) or in an other thread.
 * If 'group' is not NUL, the jobs queued with a same value (as the pointer value 'group')
 * are executed in sequence in the order of there submission.
 * If 'timeout' is not 0, it represent the maximum execution time for the job in seconds.
 * At first, the job is called with 0 as signum and the given argument.
 * The job is executed with the monitoring of its time and some signals like SIGSEGV and
 * SIGFPE. When a such signal is catched, the job is terminated and reexecuted but with
 * signum being the signal number (SIGALRM when timeout expired).
 *
 * Returns 0 in case of success or -1 in case of error.
 */
int afb_daemon_queue_job(
			void (*callback)(int signum, void *arg),
			void *argument,
			void *group,
			int timeout);

/**
 * Tells that it requires the API of "name" to exist
 * and if 'initialized' is not null to be initialized.
 * Calling this function is only allowed within init.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
int afb_daemon_require_api(
			const char *name,
			int initialized);

/**
 * Create an aliased name 'as_name' for the api 'name'.
 * Calling this function is only allowed within preinit.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
int afb_daemon_add_alias(const char *name, const char *as_name);

/**
 * Creates a new api of name 'api' with brief 'info'.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
int afb_daemon_new_api(
			const char *api,
			const char *info,
			int noconcurrency,
			int (*preinit)(void*, struct afb_api_x3 *),
			void *closure);
```
