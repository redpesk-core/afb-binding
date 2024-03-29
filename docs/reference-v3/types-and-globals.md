# Types and globals

## The global afbBindingRoot

The global **afbBindingRoot** of type **afb_api_t** is always implicitly
defined for bindings of version 3 or upper. It records the root api of
the binding.

When the binding has a defined **afbBindingExport**,  the root api
**afbBindingRoot** is the **afb_pi_t** relative to the api created for
this static description.

When the binding has no defined **afbBindingExport**, the root api is
a virtual api representing the shared object of the binding. In that case
the name of the api is the path of the shared object. Its use is restricted
but allows log messages.

## The global afbBindingExport

The global **afbBindingExport** is not mandatory.

If **afbBindingExport** is defined and exported, it must be of the type
**const afb_binding_t** and must describe the *root* api of the binding.

## The type afb_api_t

Bindings now can declare more than one api. The counter part is that
a new handle is needed to manage apis. These handles are of the type
**afb_api_t**.

It is defined as below.

```c
typedef struct afb_api_x3 afb_api_t;
```

## The type afb_binding_t

The main structure, of type **afb_binding_t**, for describing the binding
must be exported under the name **afbBindingExport**.

This structure is defined as below.

```c
typedef struct afb_binding_v3 afb_binding_t;
```

Where:

```c
/**
 * Description of the bindings of type version 3
 */
struct afb_binding_v3
{
	/** api name for the binding, can't be NULL */
	const char *api;

	/** textual specification of the binding, can be NULL */
	const char *specification;

	/** some info about the api, can be NULL */
	const char *info;

	/** array of descriptions of verbs terminated by a NULL name, can be NULL */
	const struct afb_verb_v3 *verbs;

	/** callback at load of the binding */
	int (*preinit)(struct afb_api_x3 *api);

	/** callback for starting the service */
	int (*init)(struct afb_api_x3 *api);

	/** callback for handling events */
	void (*onevent)(struct afb_api_x3 *api, const char *event, struct json_object *object);

	/** userdata for afb_api_x3 */
	void *userdata;

	/** space separated list of provided class(es) */
	const char *provide_class;

	/** space separated list of required class(es) */
	const char *require_class;

	/** space separated list of required API(es) */
	const char *require_api;

	/** avoids concurrent requests to verbs */
	unsigned noconcurrency: 1;
};
```

## The type afb_verb_t

Each verb is described with a structure of type **afb_verb_t**
defined below:

```c
typedef struct afb_verb_v3 afb_verb_t;
```

```c
/**
 * Description of one verb as provided for binding API version 3
 */
struct afb_verb_v3
{
	/** name of the verb, NULL only at end of the array */
	const char *verb;

	/** callback function implementing the verb */
	void (*callback)(afb_req_t_x2 *req);

	/** required authorization, can be NULL */
	const struct afb_auth *auth;

	/** some info about the verb, can be NULL */
	const char *info;

	/** data for the verb callback */
	void *vcbdata;

	/** authorization and session requirements of the verb */
	uint16_t session;

	/** is the verb glob name */
	uint16_t glob: 1;
};
```

The **session** flags is one of the constant defined below:

| Name                   | Description
|:----------------------:|------------------------------------------------------
| AFB_SESSION_NONE       | no flag, synonym to 0
| AFB_SESSION_LOA_0      | Requires the LOA to be 0 or more, synonym to 0 or AFB_SESSION_NONE
| AFB_SESSION_LOA_1      | Requires the LOA to be 1 or more
| AFB_SESSION_LOA_2      | Requires the LOA to be 2 or more
| AFB_SESSION_LOA_3      | Requires the LOA to be 3 or more
| AFB_SESSION_CHECK      | Requires the token to be set and valid
| AFB_SESSION_REFRESH    | Implies a token refresh
| AFB_SESSION_CLOSE      | Implies closing the session after request processed

The LOA (Level Of Assurance) is set, by binding api, using the function **afb_req_session_set_LOA**.

The session can be closed, by binding api, using the function **afb_req_session_close**.

## The types afb_auth_t and afb_auth_type_t

The structure **afb_auth_t** is used within verb description to
set security requirements.
The interpretation of the structure depends on the value of the field **type**.

```c
typedef struct afb_auth afb_auth_t;

/**
 * Definition of an authorization entry
 */
struct afb_auth
{
	/** type of entry @see afb_auth_type */
	enum afb_auth_type type;

	union {
		/** text when @ref type == @ref afb_auth_Permission */
		const char *text;

		/** level of assurancy when @ref type ==  @ref afb_auth_LOA */
		unsigned loa;

		/** first child when @ref type in { @ref afb_auth_Or, @ref afb_auth_And, @ref afb_auth_Not } */
		const struct afb_auth *first;
	};

	/** second child when @ref type in { @ref afb_auth_Or, @ref afb_auth_And } */
	const struct afb_auth *next;
};

```

The possible values for **type** is defined here:

```c
typedef enum afb_auth_type afb_auth_type_t;

/**
 * Enumeration  for authority (Session/Token/Assurance) definitions.
 *
 * @see afb_auth
 */
enum afb_auth_type
{
	/** never authorized, no data */
	afb_auth_No = 0,

	/** authorized if token valid, no data */
	afb_auth_Token,

	/** authorized if LOA greater than or equal to data 'loa' */
	afb_auth_LOA,

	/** authorized if permission 'text' is granted */
	afb_auth_Permission,

	/** authorized if 'first' or 'next' is authorized */
	afb_auth_Or,

	/** authorized if 'first' and 'next' are authorized */
	afb_auth_And,

	/** authorized if 'first' is not authorized */
	afb_auth_Not,

	/** always authorized, no data */
	afb_auth_Yes
};
```

Example:

```c
static const afb_auth_t myauth[] = {
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:me:public:set" },
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:me:public:get" },
    { .type = afb_auth_Or, .first = &myauth[1], .next = &myauth[0] }
};
```


## The type afb_req_subcall_flags_t

This is an enumeration that defines bit's positions for setting behaviour
of subcalls.

| flag                       | value | description
|----------------------------|-------|--------------
| afb_req_subcall_catch_events | 1 | the calling API wants to receive the events from subscription
| afb_req_subcall_pass_events  | 2 | the original request will receive the events from subscription
| afb_req_subcall_on_behalf    | 4 | the calling API wants to use the credentials of the original request
| afb_req_subcall_api_session  | 8 | the calling API wants to use its session instead of the one of the original request

