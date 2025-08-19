
# Functions of class **afb\_timer**


## Function afb\_timer\_create

```c
/**
 * Creates a timer handler object
 *
 * @param timer pointer to receive the created timer result
 * @param absolute boolean indicatying if the start is given in absolute (relative to epoch)
 * @param start_sec start time in seconds
 * @param start_ms millisecond part of the start time
 * @param count count of time the handler must be called (zero for endlessly)
 * @param period_ms the period in milliseconds between to calls to the handler
 * @param accuracy_ms the expected accuracy in the period
 * @param handler the handler callback
 * @param closure closure pointer for the handler
 * @param autounref boolean if not null the timer is automatically unreferenced when finished
 *
 * @return 0 in case of success or a negative -errno like value
 */
int
afb_timer_create(
	afb_timer_t *timer,
	int absolute,
	time_t start_sec,
	unsigned start_ms,
	unsigned count,
	unsigned period_ms,
	unsigned accuracy_ms,
	afb_timer_handler_t handler,
	void *closure,
	int autounref
);
```

## Function afb\_timer\_addref

```c
/**
 * Add one reference to the 'timer' object
 *
 * @param timer the timer object to reference
 *
 * @return the referenced timer object
 */
afb_timer_t
afb_timer_addref(
	afb_timer_t timer
);
```

## Function afb\_timer\_unref

```c
/**
 * Remove one reference to the 'timer' object and
 * destroys the object if not more referenced
 *
 * @param timer the timer object to unreference
 */
void
afb_timer_unref(
	afb_timer_t timer
);
```

## Function afb\_timer\_modify\_period

```c
/**
 * Modify the period of a periodic timer.
 * The method can be used within the timer handler.
 * The new period is taken into account immediately.
 * So after return, the timer triggers after the given period.
 *
 * @param timer the timer object whose period is to modify
 * @param period_ms the new required period for the timer
 */
void
afb_timer_modify_period(
	afb_timer_t timer,
	unsigned period_ms
);
```
