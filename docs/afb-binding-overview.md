# Overview of the bindings

In the redpesk terminology, a **binding** is a software component
loaded by the micro-service connector **afb-binder**.

Technically, care is taken that bindings can be loaded by any
other micro-service connector implementing the correct software
interface.

The micro-service framework is involved for inter-connection
of bindings. This allows flexible configuration of implementation
architecture and specially isolation of sensitive functionalities
for (1) firewalling accesses (2) resilience in case of failures.

A **binding** is loaded by the **binder** either isolated
or in group when security allows and when performance is expected.

A **binding** can provide 0, 1 or more **API**. **API** are the
service implementations and provide a set (not empty) of **verbs**
endpoints and a set (eventually empty) of **events**.

***Bindings*** are micro services components.
Such bindings are connected together by the micro
service framework. The framework is able to run the
bindings in the same process, in different processes
on the same machine or in different computer.

This book describes how to write a **binding**
or in other words how to add a new **service** to the system.

This section target developers.

This section shortly explain how to write a binding
using the C programming language.

## Nature of a binding

A ***binding*** is an independent piece of software providing a
service pluggable into the system. That service is offered
through named endpoints receiving requests. These named
endpoints are called **verbs**.

**Verbs**** are grouped in coherent sets called **API**
(**A**pplication **P**rogramming **I**nterface).

Bindings are usually compiled as a shared library and
are dynamically loaded by the micro-services  ***binder***.

The **API** is designated and accessed through its name.
It contains several **verbs** that implement the ***binding***
functionalities. Each of these **verbs** is a **method** that
processes requests of applications and sends results.

The **methods** of the ***bindings*** are noted **api/verb**
where **api** is the **API** name of the binding and **verb** is
the **method**'s name within the **API**.

The name of an **API** can be made of any characters except:

- the control characters (\u0000 .. \u001f)
- the characters of the set { ' ', '"', '#', '%', '&',
   '\'', '/', '?', '`', '\x7f' }

The names of the **verbs** can be any character.

Since version 4 of the framework, distinction is made between upper case
and lower case latin letters.
So **API/VERB** DOES NOT MATCH **Api/Verb** or **api/verb** anymore.

## Versions of the bindings

The versions 1 and 2 of the binding API are now obsoleted.

The versions 3 and 4 are available. The version 3 is
still supported and documented here.

The version 4 is the now growing version.
