Migration to bindings v4
========================

* Version 1, Monday 4 May 2020

  + Initial document

* Version 2, Tuesday 5 May 2020

  + Add buffer creation/scanning to list of needed evolutions
  + Add reference to mime types

* Version 3, Thursday 20 April 2023

  + Improved migration guide

At this state of the document, anything here is
PROPOSAL and subject to discussions

Introduction
------------

The already existing micro-service framework afb-binding was
improved during year 2020 in order to solve some existing problems
reveled by usage. This work introduced the fourth version of the
afb-binding API also called binding v4.

The main reason for shifting from v3 to v4 is the poor performances
measured when using JSON encoding that is mandatory in v3.

These poor performances can be fighten by using an other
library for JSON, a library that would enforce the serialisation
instead of the internal data manipulation. A quick study
showed that the main cause would remain and that JSON encoding
is a flaw, not the library used.

A change of serialisation format, for a more efficient one, like CBOR,
could also lead to the same situation.

Solving any of the above issues with the API version 3 of bindings
is not wanted because after that the programming model would not be
clear.

During 18th week of 2020, we took the decision to create a new
version of API for bindings, the version 4.

The constraints were:

* No more link to JSON serialisation or internal representation
  but be open to any internal representation or serialisation.

* It is accepted that existing V3/JSON client would not be fully
  able to interact with new bindings. It is also accepted that clients
  compatible with V4 might not communicate with bindings V4 that it
  don't know because it has no way to exploit an unknown formatting
  data.

* JSON data currently used are kept for compatibility and for its
  good convenience but are seen as a JSON string by default, not as
  objects of libjson-c. The transformation to object of json-c library
  would be still possible but on request.

Overview of operations
----------------------

Clients and servers manipulates 3 kinds of items: events, requests
and replies. The table below summarizes operations performed.

|         | server | client |
|:--------|:-------|:-------|
| event   | create | scan   |
| request | scan   | create |
| reply   | create | scan   |

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

1. the client creates a request
2. the client serializes the request to a flow
3. the client transmits the flow to the binder
4. the binder receives the flow
5. the binder transmits the flow to the binding
6. the binding deserializes the flow to a request
7. the binding treats the request
8. the binding creates the reply
9. the binding serializes the reply to a flow
10. the binding transmits the flow to the binder
13. the client deserializes the flow to a reply

That flow is already mostly in place except that the
binder, not the binding, serializes and deserializes the flow for the
most common type of data, the predefined data types.

For the application specific data types, the binder invoque
application code to serialize and deserialize the data flow on need.


Data model for bindings V4
--------------------------

### Type of data

With previous versions, the type of the data was implicitly JSON
plus some nearby items linked to the first binding interface HTML.
So key/values were still existing.

Now the type of data must be given explicitly.

How? Strings? UUID? Integers? Mime types?

### Creation of data

In all cases, the creation must be started by indicating the type
of the data created.

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

### Scanning the data

The received data is always read only. It has a type.

The scanner request access to read only buffer. The binder
can deliver the buffer by pieces.

### Question about streaming

In some cases the data to transmit is big (mega is already big, but what
about giga?) or is never terminating (audio/video streams).

Should the framework handle it? If no, what is the limit?

The current specification expects that when data is really big,
the application must slice it. There is no enforced limit, but
developpers and architects in charge of defining working system
must handle the question because the version 4 of afb-binding is not
providing a streaming API.


Evolutions of the API
---------------------

The shift to binding V4 will bring new things. Some are enforced
by the need. Some could be discussed.

Needed:

* registration / management of types of payload
  + versionning?

* buffer creation and scanning

Optional:

* new interface for managing sessions

Comparisons between V3 and V4
-----------------------------

### Comparison of afb\_binding\_t

| v3             | V4             | comment   |
|:--------------:|:--------------:|:----------|
| api            | api            | unchanged |
| specification  | specification  | unchanged |
| info           | info           | unchanged |
| verbs          | verbs          | signature changed |
| preinit        | -              | removed   |
| init           | -              | removed   |
| -              | mainctl        | main callback |
| onevent        | onevent        | removed   |
| userdata       | userdata       | unchanged |
| provide\_class | provide\_class | unchanged |
| require\_class | require\_class | unchanged |
| require\_api   | require\_api   | unchanged |
| noconcurrency  | noconcurrency  | unchanged |


### Comparison fo afb\_verb\_t

| v3       | V4       | comment   |
|:--------:|:--------:|:----------|
| verb     | verb     | unchanged |
| callback | callback | signature changed |
| auth     | auth     | unchanged |
| info     | info     | unchanged |
| vcbdata  | vcbdata  | unchanged |
| session  | session  | unchanged |
| glob     | glob     | unchanged |

### Comparison of afb\_auth\_t

Are the same, that's all.

### Comparison of SESSION

| v3                     | V4                   | comment   |
|:----------------------:|:--------------------:|:----------|
| AFB\_SESSION\_NONE     | AFB\_SESSION\_NONE   | unchanged |
| AFB\_SESSION\_CLOSE    | AFB\_SESSION\_CLOSE  | unchanged |
| AFB\_SESSION\_CHECK    | AFB\_SESSION\_CHECK  | unchanged |
| AFB\_SESSION\_LOA\_0   | AFB\_SESSION\_LOA\_0 | unchanged |
| AFB\_SESSION\_LOA\_1   | AFB\_SESSION\_LOA\_1 | unchanged |
| AFB\_SESSION\_LOA\_2   | AFB\_SESSION\_LOA\_2 | unchanged |
| AFB\_SESSION\_LOA\_3   | AFB\_SESSION\_LOA\_3 | unchanged |
| AFB\_SESSION\_RENEW    | -                    | removed, ignored, act fact that tokens are generated outside |
| AFB\_SESSION\_REFRESH  | -                    | removed, ignored, act fact that tokens are generated outside |

### Comparison of afb\_api\_t

| v3                              | V4                            | comment   |
|:-------------------------------:|:-----------------------------:|:----------|
| afb\_api\_name                  | afb\_api\_name                | kept, but not more a macro |
| afb\_api\_get\_userdata         | afb\_api\_get\_userdata       | kept, but not more a macro |
| afb\_api\_set\_userdata         | afb\_api\_set\_userdata       | kept, but not more a macro |
| afb\_api\_logmask               | afb\_api\_logmask             | kept, but not more a macro |
| afb\_api\_wants\_log\_level     | afb\_api\_wants\_log\_level   | unchanged |
| afb\_api\_vverbose              | afb\_api\_vverbose            | unchanged |
| afb\_api\_verbose               | afb\_api\_verbose             | unchanged |
| afb\_api\_get\_event\_loop      | -                             | removed   |
| afb\_api\_get\_user\_bus        | -                             | removed   |
| afb\_api\_get\_system\_bus      | -                             | removed   |
| afb\_api\_rootdir\_get\_fd      | -                             | removed   |
| afb\_api\_rootdir\_open\_locale | -                             | removed   |
| afb\_api\_queue\_job            | -                             | replaced by afb\_job\_queue |
| afb\_api\_require\_api          | afb\_api\_require\_api        | unchanged |
| afb\_api\_broadcast\_event      | afb\_api\_broadcast\_event    | signature changed |
| afb\_api\_make\_event           | -                             | replaced by afb\_api\_new\_event |
| -                               | afb\_api\_new\_event          | new       |
| afb\_api\_call                  | afb\_api\_call                | signature changed |
| afb\_api\_call\_sync            | afb\_api\_call\_sync          | signature changed |
| afb\_api\_call\_legacy          | -                             | removed   |
| afb\_api\_call\_sync\_legacy    | -                             | removed   |
| afb\_api\_new\_api              | -                             | replaced by afb\_create\_api |
| afb\_api\_delete\_api           | -                             | replaced by afb\_api\_delete |
| -                               | afb\_api\_delete              | new       |
| afb\_api\_set\_verbs\_v2        | -                             | removed   |
| afb\_api\_set\_verbs\_v3        | -                             | removed   |
| -                               | afb\_api\_set\_verbs          | new       |
| afb\_api\_add\_verb             | afb\_api\_add\_verb           | signature changed |
| afb\_api\_del\_verb             | afb\_api\_del\_verb           | unchanged |
| afb\_api\_on\_event             | afb\_api\_on\_event           | signature changed |
| afb\_api\_on\_init              | afb\_api\_on\_init            | unchanged |
| afb\_api\_seal                  | afb\_api\_seal                | unchanged |
| afb\_api\_add\_alias            | afb\_api\_add\_alias          | unchanged |
| afb\_api\_event\_handler\_add   | afb\_api\_event\_handler\_add | signature changed |
| afb\_api\_event\_handler\_del   | afb\_api\_event\_handler\_del | unchanged |
| afb\_api\_require\_class        | afb\_api\_require\_class      | unchanged |
| afb\_api\_provide\_class        | afb\_api\_provide\_class      | unchanged |
| afb\_api\_settings              | afb\_api\_settings            | signature changed |

### Comparison of afb\_event\_t

| v3                     | V4                     | comment   |
|:----------------------:|:----------------------:|:----------|
| afb\_event\_is\_valid  | afb\_event\_is\_valid  | unchanged |
| afb\_event\_unref      | afb\_event\_unref      | unchanged |
| afb\_event\_addref     | afb\_event\_addref     | unchanged |
| afb\_event\_name       | afb\_event\_name       | unchanged |
| afb\_event\_broadcast  | afb\_event\_broadcast  | signature changed |
| afb\_event\_push       | afb\_event\_push       | signature changed |

### Comparison of afb\_req\_t

| v3                              | V4                       | comment   |
|:-------------------------------:|:------------------------:|:----------|
| afb\_req\_is\_valid             | afb\_req\_is\_valid          | unchanged |
| afb\_req\_get\_api              | afb\_req\_get\_api           | unchanged |
| afb\_req\_get\_vcbdata          | afb\_req\_get\_vcbdata       | unchanged |
| afb\_req\_get\_called\_api      | afb\_req\_get\_called\_api   | unchanged |
| afb\_req\_get\_called\_verb     | afb\_req\_get\_called\_verb  | unchanged |
| afb\_req\_wants\_lo\_level      | afb\_req\_wants\_log\_level  | unchanged |
| afb\_req\_get                   | -                            | removed   |
| afb\_req\_value                 | -                            | removed   |
| afb\_req\_path                  | -                            | removed   |
| afb\_req\_json                  | -                            | removed but migration facility |
| afb\_req\_reply                 | afb\_req\_reply              | signature changed |
| afb\_req\_reply\_f              | -                            | removed   |
| afb\_req\_reply\_v              | -                            | removed   |
| afb\_req\_context\_get          | afb\_req\_context\_get       | unchanged |
| afb\_req\_context\_set          | afb\_req\_context\_set       | unchanged |
| afb\_req\_context               | afb\_req\_context            | unchanged |
| afb\_req\_context\_make         | afb\_req\_context\_make      | unchanged |
| afb\_req\_context\_clear        | afb\_req\_context\_clear     | unchanged |
| afb\_req\_addref                | afb\_req\_addref             | unchanged |
| afb\_req\_unref                 | afb\_req\_unref              | unchanged |
| afb\_req\_session\_close        | afb\_req\_session\_close     | unchanged |
| afb\_req\_session\_set\_LOA     | afb\_req\_session\_set\_LOA  | unchanged |
| afb\_req\_subscribe             | afb\_req\_subscribe          | unchanged |
| afb\_req\_unsubscribe           | afb\_req\_unsubscribe        | unchanged |
| afb\_req\_subcall               | afb\_req\_subcall            | unchanged |
| afb\_req\_subcall\_legacy       | -                            | removed   |
| afb\_req\_subcall\_req          | -                            | removed   |
| afb\_req\_subcall\_sync\_legacy | -                            | removed   |
| afb\_req\_subcall\_sync         | afb\_req\_subcall\_sync      | unchanged |
| afb\_req\_verbose               | afb\_req\_verbose            | unchanged |
| afb\_req\_has\_permission       | -                            | removed   |
| afb\_req\_check\_permission     | afb\_req\_check\_permission  | unchanged |
| afb\_req\_get\_application\_id  | -                            | removed   |
| afb\_req\_get\_uid              | -                            | removed   |
| afb\_req\_get\_client\_info     | afb\_req\_get\_client\_info  | signature changed |
| afb\_req\_success               | -                            | removed   |
| afb\_req\_success\_f            | -                            | removed   |
| afb\_req\_success\_v            | -                            | removed   |
| afb\_req\_fail                  | -                            | removed   |
| afb\_req\_fail\_f               | -                            | removed   |
| afb\_req\_fail\_v               | -                            | removed   |
| afb\_stored\_req                | -                            | removed   |
| afb\_req\_store                 | -                            | removed   |
| afb\_req\_unstore               | -                            | removed   |


### new features

| V4                        | comment   |
|:-------------------------:|:----------|
| afb\_create\_api          | replaces afb\_api\_new\_api |
| afb\_job\_queue           | replaces afb\_api\_queue\_job and improved |
| afb\_job\_abort           | new       |
| afb\_req\_get\_userdata   | new       |
| afb\_req\_set\_userdata   | new       |

### Standardisation of error codes

Replies of requests in V4 include an integer status. That integer status is meant
to either be an error code or a short answer. By convention negative values are for
errors and positives or null values are for short answers.

The macros **AFB\_IS\_ERRNO**, **AFB\_USER\_ERRNO**, **AFB\_IS\_BINDER\_ERRNO** provide user's
method to check for reply status:

| value's range       | AFB\_IS\_ERRNO(value) | AFB\_IS\_USER\_ERRNO(value) | AFB\_IS\_BINDER\_ERRNO(value) | meaning            |
|:-------------------:|:---------------------:|:---------------------------:|:-----------------------------:|:-------------------|
| value >= 0          | false                 | false                       | false                         | application status |
| -1 >= value >= -999 | true                  | false                       | true                          | predefined error   |
| value >= -1000      | true                  | true                        | false                         | application error  |

It is allowed by the convention that application use a predefined
error status when applicable.

The predefined error code are:

| error name                      | HTTP                        | usable | Meaning                         |
|:--------------------------------|:----------------------------|:------:|:--------------------------------|
| AFB\_ERRNO\_INTERNAL\_ERROR     | 500 (INTERNAL SERVER ERROR) | yes    | Any unclassified internal error |
| AFB\_ERRNO\_OUT\_OF\_MEMORY     | 500 (INTERNAL SERVER ERROR) | yes    | Specific to memory depletion    |
| AFB\_ERRNO\_UNKNOWN\_API        | 404 (NOT FOUND)             |        | Invoked API does not exist      |
| AFB\_ERRNO\_UNKNOWN\_VERB       | 404 (NOT FOUND)             |        | Invoked verb does not exit      |
| AFB\_ERRNO\_NOT\_AVAILABLE      | 501 (NOT IMPLEMENTED)       | yes    | The service is not available    |
| AFB\_ERRNO\_UNAUTHORIZED        | 401 (UNAUTHORIZED)          | yes    | The client miss authorization   |
| AFB\_ERRNO\_INVALID\_TOKEN      | 401 (UNAUTHORIZED)          |        | The client has an invalid token |
| AFB\_ERRNO\_FORBIDDEN           | 402 (FORBIDDEN)             | yes    | The client has not the permission to perform the action |
| AFB\_ERRNO\_INSUFFICIENT\_SCOPE | 402 (FORBIDDEN)             | yes    | The scope associated to the client's token is insuficient |
| AFB\_ERRNO\_BAD\_API\_STATE     | 402 (FORBIDDEN)             | yes    | The called API is not ready     |
| AFB\_ERRNO\_NO\_REPLY           |                             |        | The service did not reply       |
| AFB\_ERRNO\_INVALID\_REQUEST    |                             | yes    | Some argument of the request are invalid |
| AFB\_ERRNO\_NO\_ITEM            |                             | yes    | An item is missing, depend on the context |
| AFB\_ERRNO\_BAD\_STATE          |                             | yes    | The current state can accept the request |
| AFB\_ERRNO\_DISCONNECTED        |                             |        | Connection to the service is broken |

The column *usable* means that the error can be used by applications.

