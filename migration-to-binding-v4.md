Migration to bindings v4
========================

* Version 1, Monday 4 May 2020

  + Initial document

* Version 2, Tuesday 5 May 2020

  + Add buffer composition/scanning to list of needed evolutions
  + Add reference to mime types

At this state of the document, anything here is
PROPOSAL and subject to discussions

Introduction
------------

The main reason for shifting is the poor performances
measured in bindings v3 when using the JSON encoding
format and the library json-c.

These poor performances can be fight by using an other
library for JSON, a library that would enforce the serialisation
instead of the internal data manipulation. A quick study
showed that the main cause would remain and that JSON encoding
is somtime a flaw.

A change of serialisation format, for a more efficient one, like CBOR,
could also lead to the same situation.

Even more, switching from format A to format B would let aside the
case of formats C, D, E, ...

**** REFORMULER CIDESSOUS
Solving any of the above issues with the API version 3 of bindings
is not wanted because after that the programming model would not be
clear.

During 18th week of 2020, we took the decision to create a new
version of API for bindings, the version 4.

The constraints are:

* No more link to JSON serialisation or internal representation
  but be open to any internal representation or serialisation.

* The migration from existing bindings V3 to V4 should be as easy
  as possible. The coexisting and interoperability of V3 and V4
  must be possible with automatic conversion.

* It is accepted that existing V3/JSON client would not be fully
  able to interact with new bindings. It is also accepted that clients
  compatible with V4 might not communicate with bindings V4 that it
  don't know because it has no way to exploit an unknown formatting
  data.

* JSON data currently used are kept for compatibility and for its
  good conveniency but are seen as a JSON string by default, not as
  objects of libjson-c. A library of helper functions would help
  keep the existing programming model.

Overview of operations
----------------------

Clients and servers manipulates 3 kinds of items: events, requests
and replies. The table below summarizes operations performed.

|         | server  | client  |
|:--------|:--------|:--------|
| event   | compose | scan    |
| request | scan    | compose |
| reply   | compose | scan    |

The request is adressed to a destination designated by the pair API
and VERB.

The reply is adressed to the caller and includes from the beginning
a status, indicating whether it is a success or an error and a
companion string named info. Having a synthetic OK versus KO status
companion of the reply is probably needed but because the new version
of API changes how data are handled, it is a valid question to ask
if the string error and info are to be kept as is, as aside of the
answer, or if it should be managed by the implementers as part of the
transmited data.

At the end, the binder that is in charge of transmitting data could
do it using only 2 operations easy to abstract: serialization and
deserialization of the data.

Naive example of operation's flow:

1. the client composes a request
2. the client serializes the request to a flow
3. the client transmits the flow to the binder
4. the binder receives the flow
5. the binder transmits the flow to the binding
6. the binding deserializes the flow to a request
7. the binding treats the request
8. the binding composes the reply
9. the binding serializes the reply to a flow
10. the binding transmits the flow to the binderhttp://git.ovh.iot/afb/afb-binding/blob/sb/jobol/wip/migration-to-binding-v4.md
13. the client deserializes the flow to a reply

That flow is already mostly in place except that the
binder serializes and deserializes the flow, not the binding.

Data model for bindings V4
--------------------------

### Type of data

With previous versions, the type of the data was implicitly JSON
plus some nearby items. Now the type of data must be given explicitly.

How? Strings? UUID? Integers? Mime types?

### Composition

In all cases, the composition must be started by indicating the type
of the data composed.

To achieve performance by avoiding unneeded memory copy and to
avoid issues resulting of blocking I/Os, a mix of technics has
to be used:

* for fresh buffer to create, the binder should be able to allocate
  it for the client and return its address

* for static buffers (like constant strings), or already allocated
  buffers, it should handle the address, the length and a release
  callback

When pushed to the binder as reply or as event, the data is released
by the binder.

### Scanning

The received data is always read only. It has a type.

The scanner request access to read only buffer. The binder
can deliver the buffer by pieces.

### Question about streaming

In some cases the data to transmit is big (mega is already big, but what
about giga?) or is never terminating (audio/video streams).

Should we handle it? If no, what is the limit?

Evolutions of the API
---------------------

The shift to binding V4 will bring new things. Some are enforced
by the need. Some could be discussed.

Needed:

* registration / management of types of payload
  + versionning?

* buffer composition and scanning

Optionnal:

* new interface for managing sessions

Comparisons between V3 and V4
-----------------------------

### Comparison of afb_binding_t

| v3            | V4            | comment   |
|:-------------:|:-------------:|:----------|
| api           | api           | unchanged |
| specification | specification | unchanged |
| info          | info          | unchanged |
| verbs         | verbs         | unchanged |
| preinit       | preinit       | unchanged |
| init          | init          | unchanged |
| -             | ready         | called when required classes are initialized |
| onevent       | onevent       | signature changed |
| userdata      | userdata      | unchanged |
| provide_class | provide_class | unchanged |
| require_class | require_class | unchanged |
| require_api   | require_api   | unchanged |
| noconcurrency | noconcurrency | unchanged |


### Comparison fo afb_verb_t

| v3       | V4       | comment   |
|:--------:|:--------:|:----------|
| verb     | verb     | unchanged |
| callback | callback | unchanged |
| auth     | auth     | unchanged |
| info     | info     | unchanged |
| vcbdata  | vcbdata  | unchanged |
| session  | session  | unchanged |
| glob     | glob     | unchanged |

### Comparison of afb_auth_t

Are the same, that's all.

### Comparison of SESSION

| v3                  | V4                | comment   |
|:-------------------:|:-----------------:|:----------|
| AFB_SESSION_NONE    | AFB_SESSION_NONE  | unchanged |
| AFB_SESSION_CLOSE   | AFB_SESSION_CLOSE | unchanged |
| AFB_SESSION_CHECK   | AFB_SESSION_CHECK | unchanged |
| AFB_SESSION_LOA_0   | AFB_SESSION_LOA_0 | unchanged |
| AFB_SESSION_LOA_1   | AFB_SESSION_LOA_1 | unchanged |
| AFB_SESSION_LOA_2   | AFB_SESSION_LOA_2 | unchanged |
| AFB_SESSION_LOA_3   | AFB_SESSION_LOA_3 | unchanged |
| AFB_SESSION_RENEW   | -                 | removed, ignored, act fact that tokens are generated outside |
| AFB_SESSION_REFRESH | -                 | removed, ignored, act fact that tokens are generated outside |

### Comparison of afb_api_t

| v3 | V4 | comment |
|:---------------------------:|:-------------------------:|:----------|
| afb_api_name                | afb_api_name              | unchanged |
| afb_api_get_userdata        | afb_api_get_userdata      | unchanged |
| afb_api_set_userdata        | afb_api_set_userdata      | kept but not more a macro |
| afb_api_wants_log_level     | afb_api_wants_log_level   | unchanged |
| afb_api_vverbose            | afb_api_vverbose          | unchanged |
| afb_api_verbose             | afb_api_verbose           | unchanged |
| afb_api_get_event_loop      | -                         | removed   |
| afb_api_get_user_bus        | -                         | removed   |
| afb_api_get_system_bus      | -                         | removed   |
| afb_api_rootdir_get_fd      | -                         | removed   |
| afb_api_rootdir_open_locale | -                         | removed   |
| afb_api_queue_job           | afb_api_queue_job         | unchanged |
| afb_api_require_api         | afb_api_require_api       | unchanged |
| afb_api_broadcast_event     | afb_api_broadcast_event   | signature changed |
| afb_api_make_event          | afb_api_make_event        | unchanged |
| afb_api_call                | afb_api_call              | signature changed |
| afb_api_call_sync           | afb_api_call_sync         | signature changed |
| afb_api_call_legacy         | -                         | removed   |
| afb_api_call_sync_legacy    | -                         | removed   |
| afb_api_new_api             | afb_api_new_api           | unchanged |
| afb_api_delete_api          | afb_api_delete_api        | unchanged |
| afb_api_set_verbs_v2        | -                         | removed   |
| afb_api_set_verbs_v3        | -                         | removed   |
| -                           | afb_api_set_verbs         | only current version |
| afb_api_add_verb            | afb_api_add_verb          | signature changed |
| afb_api_del_verb            | afb_api_del_verb          | unchanged |
| afb_api_on_event            | afb_api_on_event          | signature changed |
| afb_api_on_init             | afb_api_on_init           | unchanged |
| afb_api_seal                | afb_api_seal              | unchanged |
| afb_api_add_alias           | afb_api_add_alias         | unchanged |
| afb_api_event_handler_add   | afb_api_event_handler_add | unchanged |
| afb_api_event_handler_del   | afb_api_event_handler_del | unchanged |
| afb_api_require_class       | afb_api_require_class     | unchanged |
| afb_api_provide_class       | afb_api_provide_class     | unchanged |
| afb_api_settings            | afb_api_settings          | signature changed |

### Comparison of afb_event_t

| v3                  | V4                  | comment   |
|:-------------------:|:-------------------:|:----------|
| afb_event_is_valid  | afb_event_is_valid  | unchanged |
| afb_event_unref     | afb_event_unref     | unchanged |
| afb_event_addref    | afb_event_addref    | unchanged |
| afb_event_name      | afb_event_name      | unchanged |
| afb_event_broadcast | afb_event_broadcast | signature changed |
| afb_event_push      | afb_event_push      | signature changed |

### Comparison of afb_req_t

| v3                          | V4                       | comment   |
|:---------------------------:|:------------------------:|:----------|
| afb_req_is_valid            | afb_req_is_valid         | unchanged |
| afb_req_get_api             | afb_req_get_api          | unchanged |
| afb_req_get_vcbdata         | afb_req_get_vcbdata      | unchanged |
| afb_req_get_called_api      | afb_req_get_called_api   | unchanged |
| afb_req_get_called_verb     | afb_req_get_called_verb  | unchanged |
| afb_req_wants_lo_level      | afb_req_wants_log_level  | unchanged |
| afb_req_get                 | -                        | removed   |
| afb_req_value               | -                        | removed   |
| afb_req_path                | -                        | removed   |
| afb_req_json                | -                        | removed but migration facility |
| afb_req_reply               | afb_req_reply            | signature changed |
| afb_req_reply_f             | -                        | removed   |
| afb_req_reply_v             | -                        | removed   |
| afb_req_context_get         | afb_req_context_get      | unchanged |
| afb_req_context_set         | afb_req_context_set      | unchanged |
| afb_req_context             | afb_req_context          | unchanged |
| afb_req_context_make        | afb_req_context_make     | unchanged |
| afb_req_context_clear       | afb_req_context_clear    | unchanged |
| afb_req_addref              | afb_req_addref           | unchanged |
| afb_req_unref               | afb_req_unref            | unchanged |
| afb_req_session_close       | afb_req_session_close    | unchanged |
| afb_req_session_set_LOA     | afb_req_session_set_LOA  | unchanged |
| afb_req_subscribe           | afb_req_subscribe        | unchanged |
| afb_req_unsubscribe         | afb_req_unsubscribe      | unchanged |
| afb_req_subcall             | afb_req_subcall          | unchanged |
| afb_req_subcall_legacy      | -                        | removed   |
| afb_req_subcall_req         | -                        | removed   |
| afb_req_subcall_sync_legacy | -                        | removed   |
| afb_req_subcall_sync        | afb_req_subcall_sync     | unchanged |
| afb_req_verbose             | afb_req_verbose          | unchanged |
| afb_req_has_permission      | -                        | removed   |
| afb_req_check_permission    | afb_req_check_permission | unchanged |
| afb_req_get_application_id  | -                        | removed   |
| afb_req_get_uid             | -                        | removed   |
| afb_req_get_client_info     | afb_req_get_client_info  | signature changed |
| afb_req_success             | -                        | removed   |
| afb_req_success_f           | -                        | removed   |
| afb_req_success_v           | -                        | removed   |
| afb_req_fail                | -                        | removed   |
| afb_req_fail_f              | -                        | removed   |
| afb_req_fail_v              | -                        | removed   |
| afb_stored_req              | -                        | removed   |
| afb_req_store               | -                        | removed   |
| afb_req_unstore             | -                        | removed   |

