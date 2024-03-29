Functions of class **afb_event**
==============================

## General functions

### afb_event_is_valid

```c
/**
 * Checks whether the 'event' is valid or not.
 *
 * @param event the event to check
 *
 * @return 0 if not valid or 1 if valid.
 */
int afb_event_is_valid(
			afb_event_t event);
```

### afb_event_name

```c
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
const char *afb_event_name(
			afb_event_t event);
```

### afb_event_unref

```c
/**
 * Decrease the count of references to 'event'.
 * Call this function when the evenid is no more used.
 * It destroys the event_x2 when the reference count falls to zero.
 *
 * @param event the event
 */
void afb_event_unref(
			afb_event_t event);
```

### afb_event_addref

```c
/**
 * Increases the count of references to 'event'
 *
 * @param event the event
 *
 * @return the event
 */
afb_event_t *afb_event_addref(
			afb_event_t event);
```

## Pushing functions

### afb_event_broadcast

```c
/**
 * Broadcasts widely an event of 'event' with the data 'object'.
 * 'object' can be NULL.
 *
 * For convenience, the function calls 'json_object_put' for 'object'.
 * Thus, in the case where 'object' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * @param event the event to broadcast
 * @param object the companion object to associate to the broadcasted event (can be NULL)
 *
 * @return 0 in case of success or -1 in case of error
 */
int afb_event_broadcast(
			afb_event_t event,
			struct json_object *object);
```

### afb_event_push

```c
/**
 * Pushes an event of 'event' with the data 'object' to its observers.
 * 'object' can be NULL.
 *
 * For convenience, the function calls 'json_object_put' for 'object'.
 * Thus, in the case where 'object' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * @param event the event to push
 * @param object the companion object to associate to the pushed event (can be NULL)
 *
 * @Return
 *   *  1 if at least one client listen for the event
 *   *  0 if no more client listen for the event
 *   * -1 in case of error (the event can't be delivered)
 */
int afb_event_push(
			afb_event_t event,
			struct json_object *object);
```

