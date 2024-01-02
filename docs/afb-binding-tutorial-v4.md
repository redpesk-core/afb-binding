# Tutorial of AFB bindings version 4

## Forewords

AFB bindings are dynamically loaded libraries. It may be strange
for application developpers that usually create programs running
standalone but it has advantages.

One of the most important advantage is that the composition of
the system is decided from an external design. It allows to run
several bindings in the same process for efficiency. It also
allows to run bindings in the context of different programs.

## Sample binding: tuto-1

Tuto-1 simply defines a service of name "tuto-1" with one
callable endpoint named "hello" that reply "Hello ME!" when
queried with argument string "ME".

This is the code of the binding **tutorials/v4/tuto-1.c**:

```c
  1 /*
  2  * Copyright (C) 2015-2024 IoT.bzh Company
  3  * Author: José Bollo <jose.bollo@iot.bzh>
  4  *
  5  * SPDX-License-Identifier: LGPL-3.0-only
  6  */
  7 #include <stdio.h>
  8
  9 #define AFB_BINDING_VERSION 4
 10 #include <afb/afb-binding.h>
 11
 12 void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
 13 {
 14     int retcod;
 15     const char *who = "World"; /* default value */
 16     char hello[100];
 17     afb_data_t reply, name;
 18
 19     /* some output for tracking the process */
 20     AFB_REQ_DEBUG(req, "Hi debugger!");
 21     AFB_REQ_NOTICE(req, "Hi observer!");
 22
 23     /* extract the name of the caller */
 24     retcod = afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_STRINGZ, &name);
 25     who =  retcod == 0 ? afb_data_ro_pointer(name) : "World" /* default value */;
 26
 27     /* create the reply string */
 28     retcod = snprintf(hello, sizeof hello, "Hello %s!", who);
 29     if (retcod >= (int)sizeof hello) {
 30             AFB_REQ_WARNING(req, "name too long, truncated!");
 31             retcod = (int)sizeof hello - 1;
 32             hello[retcod] = 0;
 33     }
 34
 35     /* make the reply string */
 36     afb_create_data_copy(&reply, AFB_PREDEFINED_TYPE_STRINGZ, hello, (size_t)(retcod + 1 /*with last zero*/));
 37     afb_req_reply(req, 0, 1, &reply);
 38 }
 39
 40 const afb_verb_t verbs[] = {
 41     { .verb="hello", .callback=hello },
 42     { .verb=NULL }
 43 };
 44
 45 const afb_binding_t afbBindingExport = {
 46     .api = "tuto-1",
 47     .verbs = verbs
 48 };
```

Compiling:

```bash
gcc -fPIC -shared tuto-1.c -o tuto-1.so $(pkg-config --cflags-only-I afb-binding)
```

> Note: the variable environment variable PKG_CONFIG_PATH might be
> tuned to get **pkg-config** working properly

Running it (with the binder afb-binder):

```bash
afb-binder --binding ./tuto-1.so --port 3333
```

At this point, afb-binder has started, it loaded the binding tuto-1.so and is now
listening at localhost on the port 3333.

Testing using **curl**:

```bash
$ curl -i http://localhost:3333/api/tuto-1/hello?ME
HTTP/1.1 200 OK
Connection: close
Content-Length: 77
Set-Cookie: x-afb-uuid-3333=0f6345d9-9d5b-4ff5-8bb2-cbd147ed45da; Path=/api; Max-Age=32000000; HttpOnly
Date: Tue, 24 Nov 2020 15:28:49 GMT

{"jtype":"afb-reply","request":{"status":"success","code":0},"response":"Hello {\"ME\":null}!"}
```

When requests are done using GET of HTTP protocol, the query part
of the URL is translated to an object of structure `{"key":"value",...}`.
This explains why the retrieved string for the name is JSON
representation `{\"ME\":null}`, the single key being `ME` and having no value.

This can be improved using **afb-client** (with option -H for
getting a human readable output):

```bash
$ afb-client -H ws://localhost:3333/api tuto-1 hello Me
ON-REPLY 1:tuto-1/hello: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success",
    "code":0
  },
  "response":"Hello Me!"
}
```

This shows basic things:

- The include to get for creating a binding
- How to declare the API offered by the binding
- How to handle requests made to the binding

### Getting declarations for the binding

The lines 9 and 10 show how to get the include file **afb-binding.h**.

```c
  9 #define AFB_BINDING_VERSION 4
 10 #include <afb/afb-binding.h>
```

You must define the version of ***binding*** that you are using.
This is done line 9 where we define that this is the version 4.

If you don't define it, an error is reported and the compilation aborts.
Note that this define is sometime done outside of the source file,
either in a common header or using the option -D of C compilers/preprocessors.

To include **afb-binding.h** successfully, the include search path
should be set correctly if needed (not needed only if installed in
/usr/include/afb directory that is the default).

Setting the include path is easy using **pkg-config**:

```bash
pkg-config --cflags-only-I afb-binding
```

> Note for **C++** developers:
>
> The ***binder*** currently expose a **DRAFT** version of **C++** api.
> To get it include the file <**afb/afb-binding**> (without **.h**).


### Declaring the API of the binding

Lines 40 to 48 show the declaration of the ***binding***.

The ***binder*** knows that this is a ***binding*** because
it finds the exported symbol **afbBindingExport** that is expected to be
a structure of type **afb_binding_t**.

```c
 40 const afb_verb_t verbs[] = {
 41     { .verb="hello", .callback=hello },
 42     { .verb=NULL }
 43 };
 44
 45 const afb_binding_t afbBindingExport = {
 46     .api = "tuto-1",
 47     .verbs = verbs
 48 };
```

The structure **afbBindingExport** actually tells that:

- the exported **API** name is **tuto-1** (line 46)
- the array of verbs is the above defined one

The exported list of verb is specified by an array of structures of
type **afb_verb_t**, each describing a verb, ended with a verb NULL (line 42).

The only defined verb here (line 41) is named **hello** (field **.verb**)
and the function that handle the related request is **hello**
(field **.callback**).

### Handling binder's requests

When the ***binder*** receives a request for the verb **hello** of
of the api **tuto-1**, it invoke the callback **hello** of the **binding**
with the argument **req** that handles the client request.

```c
 12 void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
 13 {
 14     int retcod;
 15     const char *who = "World"; /* default value */
 16     char hello[100];
 17     afb_data_t reply, name;
 18
 19     /* some output for tracking the process */
 20     AFB_REQ_DEBUG(req, "Hi debugger!");
 21     AFB_REQ_NOTICE(req, "Hi observer!");
 22
 23     /* extract the name of the caller */
 24     retcod = afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_STRINGZ, &name);
 25     who =  retcod == 0 ? afb_data_ro_pointer(name) : "World" /* default value */;
 26
 27     /* create the reply string */
 28     retcod = snprintf(hello, sizeof hello, "Hello %s!", who);
 29     if (retcod >= (int)sizeof hello) {
 30             AFB_REQ_WARNING(req, "name too long, truncated!");
 31             retcod = (int)sizeof hello - 1;
 32             hello[retcod] = 0;
 33     }
 34
 35     /* make the reply string */
 36     afb_create_data_copy(&reply, AFB_PREDEFINED_TYPE_STRINGZ, hello, (size_t)(retcod + 1 /*with last zero*/));
 37     afb_req_reply(req, 0, 1, &reply);
 38 }
```

The callback method for the verb receives the request and must
reply to it, either synchronously or asynchronously.

At the line 37, the callback for **tuto-1/hello** replies to the request **req**.
Parameters of the reply are:

 1. The first parameter is the request that is replied
 2. The second parameter is a status code (here 0)
 3. The third parameter is the count of replied data (here 1)
 4. The fourth parameter is an array of the replied data (here a single value)
