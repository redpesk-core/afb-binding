
# Predefined data types

Here is the list of predefined types provided to bindings
since version 4.

| type name  | C macro name                     | description                                            |
|------------|----------------------------------|--------------------------------------------------------|
| #opaque    | AFB\_PREDEFINED\_TYPE\_OPAQUE    | The opaque type is anything represented by its address |
| #stringz   | AFB\_PREDEFINED\_TYPE\_STRINGZ   | Type of zero terminated string                         |
| #json      | AFB\_PREDEFINED\_TYPE\_JSON      | Type of json string terminated by a zero               |
| #json_c    | AFB\_PREDEFINED\_TYPE\_JSON\_C   | Type of json object as handled by libjson-c            |
| #bytearray | AFB\_PREDEFINED\_TYPE\_BYTEARRAY | Type for arrays of bytes                               |
| #bool      | AFB\_PREDEFINED\_TYPE\_BOOL      | Type of boolean values                                 |
| #i32       | AFB\_PREDEFINED\_TYPE\_I32       | Type of signed 32 bit integers                         |
| #u32       | AFB\_PREDEFINED\_TYPE\_U32       | Type of unsigned 32 bit integers                       |
| #i64       | AFB\_PREDEFINED\_TYPE\_I64       | Type of signed 64 bit integers                         |
| #u64       | AFB\_PREDEFINED\_TYPE\_U64       | Type of unsigned 64 bit integers                       |
| #double    | AFB\_PREDEFINED\_TYPE\_DOUBLE    | Type of doubles                                        |

Below section describes the predefined types. For each of them,
the following items are explained:

- **length**: explain the meaning of the length
- **family**: what is the family of the type
- **converters**: implemented converters
- **updaters**: implemented updaters
- **examples**: some exemples of use

For all of them, data is represented by the address to its first byte.

## AFB\_PREDEFINED\_TYPE\_OPAQUE

The opaque type is anything represented by its address.

### length

The length is not used

### family

No family

### converters

The type AFB\_PREDEFINED\_TYPE\_OPAQUE converts to any of the below types:

- AFB\_PREDEFINED\_TYPE\_STRINGZ
- AFB\_PREDEFINED\_TYPE\_JSON
- AFB\_PREDEFINED\_TYPE\_JSON\_C

The conversion implies the creation of a unique anchor for the given data
and producing the string representation of that anchor. For example, if the
created anchor for  a data is the number 0x3f7c, the string representation
of the opacified data is "#@3f7c".

### updaters

No updater

### examples

In this example, an internal instance of `context` is wrapped to a data.

```C
/**
 * Get the data referencing the context, create it if needed.
 * The returned data has its reference count incremented and the
 * caller MUST call afb_data_unref.
 * @param context the context to reference
 * @param data address to return the data representing the context
 * @return 0 on success, a negative code otherwise
 */
int data_of_context(struct context *context, afb_data_t *data)
{
   int code = 0;
   if (context->data == NULL)
      code = afb_create_data_raw(&context->data, AFB_PREDEFINED_TYPE_OPAQUE, context, 0, 0, 0);
   *data = afb_data_addref(context->data); /* neutral if context->data is NULL */
   return code;
}
```

## AFB\_PREDEFINED\_TYPE\_BYTEARRAY

Type for arrays of bytes

### length

The count of byte in the array

### family

No family

### converters

No converter

### updaters

No updater

### examples



## AFB\_PREDEFINED\_TYPE\_STRINGZ

Type of zero terminated string UTF8, ASCII or any encoding compatible with
zero terminated byte string.

### length

The length in bytes including the tailing zero.
A length of zero is compatible with an address of NULL value.

### family

In the family of AFB\_PREDEFINED\_TYPE\_BYTEARRAY

It means that any data of type AFB\_PREDEFINED\_TYPE\_JSON is also
naturally of type AFB\_PREDEFINED\_TYPE\_BYTEARRAY.

### converters

- to AFB\_PREDEFINED\_TYPE\_OPAQUE: if the string is a valid opaque representation of
an opacified data, return that opacified data.

- to AFB\_PREDEFINED\_TYPE\_JSON: translate the string to its JSON representation.
Example: the string `HELLO` produces `"HELLO"`

### updaters

No updater

### examples

In this first example, the created `data` is a static const C string:

```C
   status = afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_STRINGZ, "hello", 1+strlen("hello"), 0, 0);
```

In this second example, the created `data` is a copy of the other string `value`:

```C
   status = afb_create_data_copy(&data, AFB_PREDEFINED_TYPE_STRINGZ, value, 1+strlen(value));
```

In this third example, the created `data` is a reference to field of `value`:

```C
   status = afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_STRINGZ, value->name, 1+strlen(value->name), unref_value, value);
```

Here the function `unref_value` is called when the created `data` is no more used,
meaning that the value `value->name` isn't needed anymore for the created data.

## AFB\_PREDEFINED\_TYPE\_JSON

Type of json string terminated by a zero, UTF8 or ASCII encoded.

### length

The length in bytes including the tailing zero.
A length of zero is compatible with an address of NULL value.

### family

In the family of AFB\_PREDEFINED\_TYPE\_STRINGZ.

It means that any data of type AFB\_PREDEFINED\_TYPE\_JSON is also
naturally of type AFB\_PREDEFINED\_TYPE\_STRINGZ.

### converters

- to AFB\_PREDEFINED\_TYPE\_OPAQUE: if the JSON string is a valid opaque representation of
an opacified data, return that opacified data.

- to AFB\_PREDEFINED\_TYPE\_JSON\_C: decode the JSON string to a json-c object and return a data holding it.

### updaters

No updater

### examples



## AFB\_PREDEFINED\_TYPE\_JSON\_C

Type of json object as handled by libjson-c

### length

The length is not used

### family

No family

### converters

- to AFB\_PREDEFINED\_TYPE\_OPAQUE: if the JSON string is a valid opaque representation of
an opacified data, return that opacified data.

- to AFB\_PREDEFINED\_TYPE\_JSON: encode the json-c object to its JSON string representation and return a data holding it.

### updaters

No updater

### examples

## AFB\_PREDEFINED\_TYPE\_BOOL

Type of boolean values

### length

Must be 1

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to and from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I32
- to AFB\_PREDEFINED\_TYPE\_U32
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_U64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I32
- to AFB\_PREDEFINED\_TYPE\_U32
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_U64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### examples



## AFB\_PREDEFINED\_TYPE\_I32

Type of signed 32 bit integers

### length

Must be 4

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to and from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### examples



## AFB\_PREDEFINED\_TYPE\_U32

Type of unsigned 32 bit integers

### length

Must be 4

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to and from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_U64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_I64
- to AFB\_PREDEFINED\_TYPE\_U64
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### examples



## AFB\_PREDEFINED\_TYPE\_I64

Type of signed 64 bit integers

### length

Must be 8

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to and from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- from AFB\_PREDEFINED\_TYPE\_JSON\_C
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### examples



## AFB\_PREDEFINED\_TYPE\_U64

Type of unsigned 64 bit integers

### length

Must be 8

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- to AFB\_PREDEFINED\_TYPE\_DOUBLE

### examples



## AFB\_PREDEFINED\_TYPE\_DOUBLE

Type of doubles

### length

Must be 8

### family

No family

### converters

- to and from AFB\_PREDEFINED\_TYPE\_JSON
- to and from AFB\_PREDEFINED\_TYPE\_JSON\_C

### updaters

- from AFB\_PREDEFINED\_TYPE\_JSON
- from AFB\_PREDEFINED\_TYPE\_JSON\_C

### examples


