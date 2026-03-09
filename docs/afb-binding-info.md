# Description of verb info

Since afb-libafb version 5.7.0, every API provides implicitely or explicitely the verb `info`.

This verb takes no argument and returns a JSON string describing the API.

The returned JSON follows one of the formats described below.

The reason why the format takes more than one form is historical.
We will first show the latest format and then the legacy one.

But let first discuss how info verb is processed.

## Processing of info verb

When the verb info is queried by a client, libafb follows the below search:

1. If the API declares a info verb, let the implementation of that verb answer.
   This mode is explicit and it is the responsability of the implemeter to
   return a valid description.

2. If the API provided a value for its specification (structure `afb\_binding\_t`,
   field `specification`, this specification is supposed to be the JSON description
   and is returned. It is the responsability of the implementer to provide a valid
   description in that field (see below for tooling).

   The specification can also be set using function `afb\_api\_set\_specification`
   since version 4.3.0 (need to set AFB\_BINDING\_X4R1\_ITF\_REVISION to 9 or upper).

3. If none of the previous steps has produced an answer, a description is automatically
   generated for the existing verbs of the API.

The second case, providing a descritpion using `specification` is the preferred one for
API that are fully defined at compile time.

### example of integration of afb-json2c

The tool `afb-json2c` provided by the package `afb-idl` translates a JSON file
to a C file containing that JSON in a compressed and escaped format.

Suppose that the JSON specification of the API is in the file `spec.json`
and the corresponding generated C code in `spec.h`.

In your C code, you should write:

```C
static const char spec[] =
#include "spec.h"
;
```

Note that afb-json2c only produce a string constant, not a variable definition.
For this reason, the variable is declared before the include and a semi colon
is needed after. This feature allows to write instead of the previous code the
below code:

```C
const afb_binding_t afbBindingExport = {
	.api = "API",
	.info = "Some info",
	.verbs = my_array_of_verbs,
	.specification =
#include "spec.h"
};
```

Then for building, the integration in make file looks as below:

```make
spec.h: spec.json
	afb-json2c < $< > $@
```

and in cmake as below:

```cmake
add_custom_command(
	OUTPUT spec.h
	COMMAND afb-json2c spec.json > spec.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating specification source from JSON"
)
```

## Description format "afb-api-info/v2"

The description is a JSON object looking like:

```JSON
{
   "$schema": "afb-api-info/v2",
   "verbs": [ ... ]
}
```

The root fields are:

- **`$schema`**, MANDATORY, string, the string `afb-api-info/v2` that identify
  the current format.

- **`verbs`**, MANDATORY, array of VerbDesc, see below.

- **`name`**, OPTIONAL, string, the API name, this field is not useful because it
  is redundant with the api that returned the specification. However, if set,
  it should be the same name.

- **`info`**, OPTIONAL, string, a short informative description.

- **`description`**, OPTIONAL, any, a long description of the API.

- **`version`**, OPTIONAL, string, the version of the API.

- **`author`**, OPTIONAL, string, declaration of the author.

- **`site`**, OPTIONAL, string, URL of the site for the API.

- **`help`**, OPTIONAL, string, URL of the help site for the API.

- **`types`**, OPTIONAL, dictionary of any, description of types,
  the key of the dictionary is the type name.

- **`groups`**, OPTIONAL, dictionary of any, description of groups,
  the key of the dictionary is the group name.

Note that adding optional fields does not change the version of the format.

### VerbDesc, the description of verbs

Decription of verbs are objects looking like:

```JSON
{
   "name": "get-counts",
   "info": "return the counts",
   "loa": 1,
   "auth": "urn:redpesk:permission::partner:counts",
   "signature": { "
   "group": [ "getter", "protected" ]
}
```

The fields are:

- **`name`**, MANDATORY, string, the verb name.

- **`glob`**, OPTIONAL, boolean, if true, verb name is a global
  pattern, false by default.

- **`info`**, OPTIONAL, string, a short description of the verb.

- **`description`**, OPTIONAL, any, a long description of the verb.

- **`session-close`**, OPTIONAL, boolean, if true, the session
  is closed when the verb call ends.

- **`auth`**, OPTIONAL, string, a string describing the required
  permissions, loa and token, default is no permission required.

- **`signature`**, OPTIONAL, any or array of any, signature(s) of
  the verb, documenting the count and types of arguments and the
  count and types of responses.

- **`groups`**, OPTIONAL, string or array of strings, list of groups
  where the verb belongs.

- **`samples`**, OPTIONAL, any or array of any, list of samples of usage.


## Description legacy format

Here is in few lines the only doc available for that format.

```json
 "metadata": {
      "uid": "helloworld", // string => api name
      "info": "A simple Helloworld binding", // string => api description/info
      "status": {"online": true}, // object where to store additional infos such as uri, online, ...
      "version": "1.0"
    },
    "groups": [ // array of objects => list of groups
      {
        "uid":  "general", // string => group name
        "info": "verbs related to general tests on this binding",// string => group description/info
        "status": {"slaveid": 1}, // object where to store additional infos such as uri, online, ...
        "verbs": [ // array of objects => list of verbs belonging to this group
          {
            "uid": "ping", // string => verb name
            "info": "sends ping", // string => verb description
            "verb": "ping", // string => raw verb to be sent to the API
            "usage": { // object => query format will appear as a placeholder in the textarea to enter query
                "action": ["get","subscribe","unsubscribe"], // array of strings => list of actions such as Subscribe, Get, ...
                "data":{"data":"parameter"}, // object => hints on other parameter to add to the query can also be verbose, arg...
            },
            "sample":  [ // array of objects => list of samples
                {"foo":"bar"},
                {"cezam":"tada"},
                {"cezam":"open"}
            ]
          },
        ]
      },
    ]
  },
}
```


