/*
 * Copyright (C) 2015-2023 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <functional>
#include <utility>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

/* check the version */
#if !defined(AFB_BINDING_VERSION)
# define AFB_BINDING_VERSION 4
#elif AFB_BINDING_VERSION != 4
# error "AFB_BINDING_VERSION must be 4"
#endif

/* get C definitions of bindings */
extern "C" {
#include <afb/afb-binding.h>
}

namespace afb {
/*************************************************************************/
/* pre-declaration of classes                                            */
/*************************************************************************/

class api;
class arg;
class data;
class event;
class req;
class type;

/*************************************************************************/
/* declaration of functions                                              */
/*************************************************************************/
#if 0
int broadcast_event(const char *name, struct json_object *object = nullptr);

event new_event(const char *name);

void verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args);

void verbose(int level, const char *file, int line, const char * func, const char *fmt, ...);

int queue_job(void (*callback)(int signum, void *arg), void *argument, void *group, int timeout);

int require_api(const char *apiname, bool initialized = true);

int add_alias(const char *apiname, const char *aliasname);

int verbosity();

bool wants_errors();
bool wants_warnings();
bool wants_notices();
bool wants_infos();
bool wants_debugs();

void call(const char *api, const char *verb, struct json_object *args, void (*callback)(void*closure, int iserror, struct json_object *result, afb_api_t api), void *closure);

template <class T> void call(const char *api, const char *verb, struct json_object *args, void (*callback)(T*closure, int iserror, struct json_object *result, afb_api_t api), T *closure);

bool callsync(const char *api, const char *verb, struct json_object *args, int &status, unsigned &nreplies, afb_data_t replies[]);


	int queue_job(queue_cb callback, void *argument, void *group, int timeout) const;
inline int api::queue_job(queue_cb callback, void *argument, void *group, int timeout) const { return afb_api_queue_job(api_, callback, argument, group, timeout); }


	api create_api(const char *apiname, const char *info, int noconcurrency, mainctl_cb mainctl, void *closure) const;
	api create_api(const std::string &apiname, const std::string &info, int noconcurrency, mainctl_cb mainctl, void *closure) const;

inline api api::new_api(const char *apiname, const char *info, int noconcurrency, api::mainctl_cb mainctl, void *closure) const { afb_api_t a; afb_api_new_api(api_, &a, apiname, info, noconcurrency, mainctl, closure); return api(a); }
inline api api::new_api(const std::string &apiname, const std::string &info, int noconcurrency, api::mainctl_cb mainctl, void *closure) const { return new_api(apiname.c_str(), info.c_str(), noconcurrency, mainctl, closure); }

inline int api::add_alias(const char *name, const char *as_name) const { return afb_api_add_alias(api_, name, as_name); }
inline int api::add_alias(const std::string &name, const std::string &as_name) const { return afb_api_add_alias(api_, name.c_str(), as_name.c_str()); }
	int add_alias(const char *name, const char *as_name) const;
	int add_alias(const std::string &name, const std::string &as_name) const;

inline int queue_job(void (*callback)(int signum, void *arg), void *argument, void *group, int timeout)
	{ return root().queue_job(callback, argument, group, timeout); }

inline int add_alias(const char *apiname, const char *aliasname)
	{ return root().add_alias(apiname, aliasname); }

#endif

/*************************************************************************/
/* globals facilities                                                    */
/*************************************************************************/
static inline int alias(const char *from_name, const char *as_name) noexcept
	{ return afb_alias_api(from_name, as_name); }

/*************************************************************************/
/* logging facility                                                      */
/*************************************************************************/

class log_level {
	enum afb_syslog_levels level_;
public:
	log_level(enum afb_syslog_levels level) : level_{level} {}
	explicit operator int() const noexcept { return (int)level_; }
	explicit operator enum afb_syslog_levels() const noexcept { return level_; }

#if LOGLEVEL_OK
	void operator ()(const char *fmt, va_list args) const noexcept;
	void operator ()(const char *fmt, ...) const noexcept;
	void operator ()(req req, const char *fmt, va_list args) const noexcept;
	void operator ()(req req, const char *fmt, ...) const noexcept;
	void operator ()(api api, const char *fmt, va_list args) const noexcept;
	void operator ()(api api, const char *fmt, ...) const noexcept;
#endif
};

static const log_level Emergency(AFB_SYSLOG_LEVEL_EMERGENCY);
static const log_level Alert(AFB_SYSLOG_LEVEL_ALERT);
static const log_level Critical(AFB_SYSLOG_LEVEL_CRITICAL);
static const log_level Error(AFB_SYSLOG_LEVEL_ERROR);
static const log_level Warning(AFB_SYSLOG_LEVEL_WARNING);
static const log_level Notice(AFB_SYSLOG_LEVEL_NOTICE);
static const log_level Info(AFB_SYSLOG_LEVEL_INFO);
static const log_level Debug(AFB_SYSLOG_LEVEL_DEBUG);

class logger {
public:
	/* Virtual verbosity functions */
	virtual int wants_log_level(int level) const noexcept = 0;
	virtual void vverbose(int level, const char *file, int line, const char *func, const char *fmt, va_list args) const noexcept = 0;

	/* Derived verbosity functions */
	void verbose(int level, const char *file, int line, const char *func, const char *fmt, ...) const noexcept
		{
			va_list args;
			va_start(args, fmt);
			vverbose(level, file, line, func, fmt, args);
			va_end(args);
		}

	bool wants(const log_level level) const noexcept
		{ return (bool)wants_log_level(int(level)); }
	void vverbose(const log_level level, const char *fmt, va_list args) const noexcept
		{ return vverbose(int(level), nullptr, 0, nullptr, fmt, args); }
	void verbose(const log_level level, const char *fmt, ...) const noexcept
		{
			va_list args;
			va_start(args, fmt);
			vverbose(int(level), nullptr, 0, nullptr, fmt, args);
			va_end(args);
		}
};

/*************************************************************************/
/* binding's exceptions                                                  */
/*************************************************************************/

// generic afb::error
class error : public std::runtime_error
{
public: error(const char *arg) : std::runtime_error{arg} {}
};

class type_not_found_error : public error
{
public: type_not_found_error(const char *arg) : error{arg} {}
};

class invalid_type_error : public error
{
public: invalid_type_error(const char *arg) : error{arg} {}
};

class invalid_data_error : public error
{
public: invalid_data_error(const char *arg) : error{arg} {}
};

class convert_data_error : public error
{
public: convert_data_error(const char *arg) : error{arg} {}
};

class create_data_error : public error
{
public: create_data_error(const char *arg) : error{arg} {}
};

/*************************************************************************/
/* classes for data and data's type                                      */
/*************************************************************************/

class type;
class data;

/**
 * class wrapping afb_type_t
 */
class type
{
protected:
	/** wrapped type */
	afb_type_t type_;

	/** internal lookup */
	void _lookup_(const char *name)
		{ if (afb_type_lookup(&type_, name) < 0)
			throw type_not_found_error("type-not-found"); }

public:
	/** default constructor */
	type() noexcept
		: type_{nullptr} {}

	/** initialisation constructor */
	type(afb_type_t a_type) noexcept
		: type_{a_type} {}

	/** copy constructor */
	type(const type &other) noexcept
		: type_{other.type_} {}

	/** initialisation constructor */
	type(const char *name)
		{ _lookup_(name); }
	type(const std::string &name)
		{ _lookup_(name.c_str()); }

	/** unwrap */
	operator afb_type_t() const noexcept
		{ return type_; }

	/** assign */
	type &operator=(afb_type_t type) noexcept
		{ type_ = type;
		  return *this; }
	type &operator=(const type &other) noexcept
		{ return *this = other.type_; }
	type &operator=(const char *name)
		{ _lookup_(name);
		  return *this; }
	type &operator=(const std::string &name)
		{ return *this = name.c_str(); }

	/** validity */
	operator bool() const noexcept
		{ return is_valid(); }
	bool is_valid() const noexcept
		{ return type_ != nullptr; }
	void validate() const
		{ if (!is_valid())
			throw invalid_type_error("invalid-type"); }

	/** comparison */
	bool operator ==(const type &other) const noexcept
		{ return type_ == other.type_; }

	/** name */
	const char *name() const {
		validate();
		return afb_type_name(type_);
	}
	operator const char *() const
		{ return name(); }

	/** creation of data */
	data create_raw(const void *pointer, size_t size, void (*dispose)(void*), void *closure) const;
	data create_alloc(void **pointer, size_t size) const;
	data create_copy(const void *pointer, size_t size) const;

	/** conversion of data */
	data convert(data data) const;

	/** check names */
	static bool exists(const char *name)
		{ afb_type_t t;
		  return afb_type_lookup(&t, name) == 0; }
	static bool exists(const std::string &name)
		{ return exists(name.c_str()); }
};

/**
 * class wrapping afb_data_t
 */
class data
{
	friend class req;
protected:
	/** wrapped data */
	afb_data_t data_;

	/** conversion */
	void _convert_(afb_type_t a_type, afb_data_t a_data)
		{ if (afb_data_convert(a_data, a_type, &data_) < 0)
			throw convert_data_error("convert-data-failed"); }

	/** creation */
	void _create_(int status)
		{ if (status < 0)
			throw create_data_error("create-data-failed"); }

public:
	/** default constructor */
	data() noexcept
		: data_{nullptr} {}

	/** initialisation constructor */
	data(afb_data_t a_data) noexcept
		: data_{a_data} {}

	/** copy constructor */
	data(const data &other) noexcept
		: data_{other.data_} {}
	data(data &&other) noexcept
		: data_{other.data_} { other.data_ = nullptr; }

	/** creation constructors */
	data(afb_type_t a_type, const void *pointer, size_t size, void (*dispose)(void*), void *closure)
		{ _create_(afb_create_data_raw(&data_, a_type, pointer, size, dispose, closure)); }
	data(afb_type_t a_type, const void *pointer, size_t size)
		{ _create_(afb_create_data_copy(&data_, a_type, pointer, size)); }
	data(afb_type_t a_type, void **pointer, size_t size)
		{ _create_(afb_create_data_alloc(&data_, a_type, pointer, size)); }
	data(afb::type type, const void *pointer, size_t size, void (*dispose)(void*), void *closure)
		{ type.validate();
		  _create_(afb_create_data_raw(&data_, type, pointer, size, dispose, closure)); }
	data(afb::type type, const void *pointer, size_t size)
		{ type.validate();
		  _create_(afb_create_data_copy(&data_, type, pointer, size)); }
	data(afb::type type, void **pointer, size_t size)
		{ type.validate();
		  _create_(afb_create_data_alloc(&data_, type, pointer, size)); }

	/** conversion constructors */
	data(afb_type_t a_type, afb_data_t a_data)
		{ _convert_(a_type, a_data); }
	data(afb_type_t a_type, const data &other)
		{ _convert_(a_type, other.data_); }
	data(afb::type type, afb_data_t a_data)
		{ type.validate();
		  _convert_(type, a_data); }
	data(afb::type type, const data &other)
		{ type.validate();
		  _convert_(type, other.data_); }

	/** destruction */
	~data()
		{}

	/** unwrap */
	operator afb_data_t() const
		{ return data_; }

	/** assign */
	data &operator=(afb_data_t a_data)
		{ data_ = a_data;
		  return *this; }
	data &operator=(const data &other)
		{ return *this = other.data_; }
	data &operator=(data &&other)
		{ data_ = other.data_;
		  other.data_ = nullptr;
		  return *this; }

	/** validation */
	bool is_valid() const
		{ return afb_data_is_valid(data_); }
	operator bool() const
		{ return is_valid(); }
	void validate() const
		{ if (!is_valid())
			throw invalid_data_error("invalid-data"); }

	/** comparison */
	bool operator ==(const data &other) const
		{ return data_ == other.data_; }

	/** reference count */
	void unref()
		{ afb_data_unref(data_); }
	data &addref()
		{ afb_data_addref(data_);
		  return *this; }
	const data &addref() const
		{ afb_data_addref(data_);
		  return *this; }

	/** access components */
	afb::type type() const
		{ return afb::type(afb_data_type(data_)); }
	void *pointer() const
		{ return afb_data_ro_pointer(data_); }
	void *operator *() const
		{ return afb_data_ro_pointer(data_); }
	size_t size() const
		{ return afb_data_size(data_); }

	/** conversion */
	data convert(afb::type t) const
		{ return data(t, *this); }
};

/*************************************************************************/
/* effective members of class afb::type                                  */
/*************************************************************************/

/** creation of data */
inline data type::create_raw(const void *pointer, size_t size, void (*dispose)(void*), void *closure) const {
	return data(*this, pointer, size, dispose, closure);
}

inline data type::create_alloc(void **pointer, size_t size) const {
	return data(*this, pointer, size);
}

inline data type::create_copy(const void *pointer, size_t size) const {
	return data(*this, pointer, size);
}

/** conversion of data */
inline data type::convert(data other_data) const {
	return data(*this, other_data);
}

/** predefined types */
static inline type OPAQUE() { return type(AFB_PREDEFINED_TYPE_OPAQUE); }
static inline type STRINGZ() { return type(AFB_PREDEFINED_TYPE_STRINGZ); }
static inline type JSON() { return type(AFB_PREDEFINED_TYPE_JSON); }
static inline type JSON_C() { return type(AFB_PREDEFINED_TYPE_JSON_C); }
static inline type BYTEARRAY() { return type(AFB_PREDEFINED_TYPE_BYTEARRAY); }
static inline type BOOL() { return type(AFB_PREDEFINED_TYPE_BOOL); }
static inline type I32() { return type(AFB_PREDEFINED_TYPE_I32); }
static inline type U32() { return type(AFB_PREDEFINED_TYPE_U32); }
static inline type I64() { return type(AFB_PREDEFINED_TYPE_I64); }
static inline type U64() { return type(AFB_PREDEFINED_TYPE_U64); }
static inline type DOUBLE() { return type(AFB_PREDEFINED_TYPE_DOUBLE); }

/** legacy predefined types */
inline type type_opaque() { return OPAQUE(); }
inline type type_stringz() { return STRINGZ(); }
inline type type_json() { return JSON(); }
inline type type_json_c() { return JSON_C(); }





/*************************************************************************/
/* effective declaration of classes                                      */
/*************************************************************************/


/**
 * class intended to manage received set of data: params,
 * and asynchronous replies
 */
template <unsigned count_>
class dataset
{
protected:
	afb_data_t array_[count_];
public:
	dataset() {}

	/* copy method */
	dataset(const dataset &other) = delete;

	class ref {
	private:
		afb_data_t *item_;
		friend class dataset;
		ref(afb_data_t *item) : item_{item} {}
	public:
		operator afb::data() const { return data(*item_); }
		ref operator=(const data &data) const { *item_ = data; return *this; }
	};

	/* access */
	ref operator[](int idx) { return ref(&array_[idx]); }
	ref operator[](unsigned idx) { return ref(&array_[idx]); }
	unsigned count() const { return count_; }
	afb_data_t const *data() const { return array_; }
};

/**
 * class intended to manage received set of data: params,
 * and asynchronous replies
 */
class received_data
{
protected:
	/** recorded count of data */
	unsigned size_;

	/** recorded array of data */
	afb_data_t const *array_;

public:
	/** common constructor */
	received_data(unsigned size, afb_data_t const array[]) noexcept
		: size_{size}, array_{array} {}

	/** copy constructor */
	received_data(const received_data &other) noexcept
		: size_{other.size_}, array_{other.array_} {}

	/** as pair */
	operator std::pair<unsigned, afb_data_t const *>() const noexcept
		{ return std::pair<unsigned, afb_data_t const *>(size_, array_); }

	/** count of data */
	unsigned size() const noexcept
		{ return size_; }

	operator unsigned() const noexcept
		{ return size_; }

	/** get the array of data */
	afb_data_t const *array() const noexcept
		{ return array_; }

	operator afb_data_t const *() const noexcept
		{ return array_; }

	/** add a reference on each data */
	void addref() const noexcept
		{ afb_data_array_addref(size_, array_); }

	/** add a reference on each data */
	void unref() const noexcept
		{ afb_data_array_unref(size_, array_); }

	/** access the data */
	data operator[](unsigned idx) const noexcept
		{ return data(array_[idx]); }

	data operator[](int idx) const noexcept
		{ return data(array_[idx]); }

	data at(unsigned idx) const {
		if (size_ <= idx)
			throw std::out_of_range("out of range");
		return data(array_[idx]);
	}

	data at(int idx) const {
		if (idx < 0 || size_ <= idx)
			throw std::out_of_range("out of range");
		return data(array_[idx]);
	}
};

/* apis */
class api : public logger
{
protected:
	afb_api_t api_;
public:
	using call_cb = afb_call_callback_t;
	using event_cb = afb_event_handler_t;
	using verb_cb = afb_req_callback_t;
	using mainctl_cb = afb_api_callback_t;
	using queue_cb = void (*)(int signum, void *arg);

	api();
	api(afb_api_t a);
	api(const api &other);
	api &operator=(const api &other);

	operator afb_api_t() const;
	afb_api_t operator->() const;

	/* General functions */
	const char *name() const;
	int alias(const char *as_name) noexcept
		{ return afb::alias(name(), as_name); }
	void *get_userdata() const;
	void set_userdata(void *value) const;
	int require_api(const char *name, int initialized) const;
	int require_api(const std::string &name, int initialized) const;

	/* Verbosity functions */
	int wants_log_level(int level) const noexcept final
		{ return afb_api_wants_log_level(api_, level); }
	void vverbose(int level, const char *file, int line, const char *func, const char *fmt, va_list args) const noexcept final
		{ afb_api_vverbose(api_, level, file, line, func, fmt, args); }

	/* Data retrieval functions */
	struct json_object *settings() const;

	/* Calls and job functions */
	void call(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], call_cb callback, void *closure) const;
	void call(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], call_cb callback, void *closure) const;
	int callsync(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const;
	int callsync(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const;

	/* Event functions */
	int broadcast_event(const char *name, unsigned nparams, afb_data_t const params[]) const;
	int broadcast_event(const std::string &name, unsigned nparams, afb_data_t const params[]) const;
	event new_event(const char *name) const;
	event new_event(const std::string &name) const;
	int event_handler_add(const char *pattern, event_cb callback, void *closure) const;
	int event_handler_add(const std::string &pattern, event_cb callback, void *closure) const;
	int event_handler_del(const char *pattern, void **closure) const;
	int event_handler_del(const std::string &pattern, void **closure) const;

	/* Dynamic api functions */
	int set_verbs(const afb_verb_t *verbs) const;
	int add_verb(const char *verb, const char *info, verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const;
	int add_verb(const std::string &verb, const std::string &info, verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const;
	int del_verb(const char *verb, void **vcbdata) const;
	int del_verb(const std::string &verb, void **vcbdata) const;
	int provide_class(const char *name) const;
	int provide_class(const std::string &name) const;
	int require_class(const char *name) const;
	int require_class(const std::string &name) const;
	void seal() const;
	int delete_api() const;
};

/* events */
class event
{
	afb_event_t event_;
public:
	event();
	event(afb_event_t e);
	event(const event &other);
	event(event &&other);
	~event();

	event &operator=(afb_event_t e);
	event &operator=(const event &other);
	event &operator=(event &&other);

	operator afb_event_t() const;
	afb_event_t extract();
	afb_event_t twin() const;

	operator bool() const;
	bool is_valid() const;

	int broadcast() const;
	int broadcast(afb_data_t data) const;
	int broadcast(unsigned nparams, afb_data_t const params[]) const;
	template <unsigned n> int broadcast(const dataset<n> &params) const;
	int broadcast(const std::vector<afb_data_t> &&params) const;

	int push() const;
	int push(afb_data_t data) const;
	int push(unsigned nparams, afb_data_t const params[]) const;
	template <unsigned n> int push(const dataset<n> &params) const;
	int push(const std::vector<afb_data_t> &&params) const;

	void unref();
	void addref();
	const char *name() const;
};

/* req(uest) */
class req : public logger
{
	afb_req_t req_;

public:
	req() = delete;
	req(afb_req_t r);
	req(const req &other);
	req &operator=(const req &other);

	operator afb_req_t() const;
	afb_req_t operator->() const;

	operator bool() const;
	bool is_valid() const;

	afb::api api() const;

	void reply(int status = 0) const noexcept;
	void reply(int status, afb_data_t data) const noexcept;
	void reply(int status, unsigned nreplies, afb_data_t const replies[]) const noexcept;
	template <unsigned n> void reply(int status, const dataset<n> &replies) const noexcept;
	void reply(int status, received_data replies) const noexcept;
	void reply(int status, const std::vector<afb_data_t> &&params) const noexcept;

	void addref() const;

	void unref() const;

	void session_close() const;

	bool session_set_LOA(unsigned level) const
		{ return !afb_req_session_set_LOA(req_, level); }


	bool subscribe(const event &event) const;

	bool unsubscribe(const event &event) const;

	void subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), void *closure) const;
	template <class T> void subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(T *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), T *closure) const;

	int subcallsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, int &status, unsigned &nreplies, afb_data_t replies[]) const;


	/* Verbosity functions */
	int wants_log_level(int level) const noexcept final
		{ return afb_req_wants_log_level(req_, level); }
	void vverbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args) const noexcept final
		{ afb_req_vverbose(req_, level, file, line, func, fmt, args); }

	struct json_object *get_client_info() const;

	template < class T = void >
	class contextclass {

		friend class req;
		afb_req_t req_;
		contextclass(afb_req_t r) : req_(r) {}
		static void default_destroyer(T*t) { delete t; }

	public:
		inline operator bool() const { return bool(get()); }
		inline operator T *() const { return get(); }
		inline operator T &() const { return *get(); }
		inline T* get() const {
			T* val = nullptr;
			afb_req_context_get(req_, reinterpret_cast<void**>(&val));
			return val;
		}

		inline void set(T *value, void (*destroyer)(T*) = default_destroyer) const {
			afb_req_context_set(req_,
				reinterpret_cast<void*>(value),
				reinterpret_cast<void(*)(void*)>(destroyer),
				reinterpret_cast<void*>(value));
		}

		inline void unset() { afb_req_context_drop(req_); }
		inline void clear() { afb_req_context_drop(req_); }
/*
		inline T *lazy(T *(*allocator)() = []()->T*{return new T();}, void (*destroyer)(T*) = [](T*t){delete t;}) const {
			return reinterpret_cast<T*>(
				afb_req_context(req_, 0,
					[allocator](void*)->T*{return allocator();},
					reinterpret_cast<void(*)(void*)>(destroyer),
					nullptr));
		}

		template <class I>
		inline T *lazy(I *i, T *(*allocator)(I*) = [](I*i)->T*{return new T(i);}, void (*destroyer)(T*) = [](T*t){delete t;}) const {
			return reinterpret_cast<T*>(
				afb_req_context(req_, 0,
					[allocator](void*i)->T*{return allocator(reinterpret_cast<I*>(i));},
					reinterpret_cast<void(*)(void*)>(destroyer),
					reinterpret_cast<void*>(i)));
		}
*/
	};

	template < class T > contextclass<T> context() const { return contextclass<T>(req_); }

	received_data parameters() const noexcept
		{
			const afb_data_t *params;
			unsigned count = afb_req_parameters(req_, &params);
			return received_data(count, params);
		}

	bool try_convert(unsigned idx, type type) const noexcept
		{ return !afb_req_param_convert(req_, idx, afb_type_t(type), nullptr); }

	bool try_convert(int idx, type type) const noexcept
		{ return try_convert(unsigned(idx), type); }

	bool try_convert(unsigned idx, type type, data &data) const noexcept
		{ return !afb_req_param_convert(req_, idx, afb_type_t(type), &data.data_); }

	bool try_convert(int idx, type type, data &data) const noexcept
		{ return try_convert(unsigned(idx), type, data); }

	data convert(unsigned idx, type type) const
		{
			int rc = afb_req_param_convert(req_, idx, afb_type_t(type), nullptr);
			if (rc)
				throw new convert_data_error("can't convert parameter");
		}

	data convert(int idx, type type) const
		{ return convert(unsigned(idx), type); }
};



/*************************************************************************/
/* effective declaration of classes                                      */
/*************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template < class _T_ >
struct client_context {

	inline _T_ *get(afb_req_t req) const {
		_T_ *val = nullptr;
		afb_req_context(req, reinterpret_cast<void**>(&val), _init_, nullptr);
		return val;
	}

private:

	static int _init_(void*, void **value, void (**freecb)(void*), void **freeclo) {
		_T_ *t = new _T_();
		*value = *freeclo = t;
		*freecb = _destroy_;
		return 0;
	}
	static void _destroy_(void *ptr) { delete reinterpret_cast<_T_*>(ptr); }
};

template < class _T_ >
struct api_context {

	inline _T_ *get(afb_req_t req) const {
		afb_api_t api = afb_req_get_api(req);
		void *ptr = afb_api_get_userdata(api);
		return reinterpret_cast<_T_*>(ptr);
	}
};

template < class _T_ >
struct vcb_context {

	inline _T_ *get(afb_req_t req) const {
		void *ptr = afb_req_get_vcbdata(req);
		return reinterpret_cast<_T_*>(ptr);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(void*,int,afb::received_data,afb::req)>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	_F_(closure, status, afb::received_data(nreplies, replies), req);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(void*,int,afb::received_data,afb::req)>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), req);
}

template <class _C_, void (_C_::*_F_)(void*,int,afb::received_data,afb::req) const>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	reinterpret_cast<const _C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), req);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(void*,int,afb::received_data,afb::api)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	_F_(closure, status, afb::received_data(nreplies, replies), api);
}

template <void (*_F_)(void*,int,afb::received_data)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	_F_(closure, status, afb::received_data(nreplies, replies));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(int,afb::received_data,afb::api)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), api);
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data,afb::api) const>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<const _C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), api);
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies));
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data) const>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<const _C_*>(closure)->_F_(status, afb::received_data(nreplies, replies));
}

/*************************************************************************/
/* effective members of class afb::api                                   */
/*************************************************************************/

inline api::api() : api_{nullptr} { }
inline api::api(afb_api_t a) : api_{a} { }
inline api::api(const api &other) : api_{other.api_} {}
inline api &api::operator=(const api &other) { api_ = other.api_; return *this;}
inline api::operator afb_api_t() const { return api_; }
inline afb_api_t api::operator->() const { return api_; }
inline const char *api::name() const { return afb_api_name(api_); }
inline void *api::get_userdata() const { return afb_api_get_userdata(api_); }
inline void api::set_userdata(void *value) const { afb_api_set_userdata(api_, value); }
inline int api::require_api(const char *name, int initialized) const { return afb_api_require_api(api_, name, initialized); }
inline int api::require_api(const std::string& name, int initialized) const { return afb_api_require_api(api_, name.c_str(), initialized); }
inline struct json_object *api::settings() const { return afb_api_settings(api_); }
inline void api::call(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], api::call_cb callback, void *closure) const { afb_api_call(api_, apiname, verb, nparams, params, callback, closure); }
inline void api::call(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], api::call_cb callback, void *closure) const { afb_api_call(api_, apiname.c_str(), verb.c_str(), nparams, params, callback, closure); }
inline int api::callsync(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const { return afb_api_call_sync(api_, apiname, verb, nparams, params, &status, &nreplies, replies); }
inline int api::callsync(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const { return afb_api_call_sync(api_, apiname.c_str(), verb.c_str(), nparams, params, &status, &nreplies, replies); }

inline int api::broadcast_event(const char *name, unsigned nparams, afb_data_t const params[]) const { return afb_api_broadcast_event(api_, name, nparams, params); }
inline int api::broadcast_event(const std::string &name, unsigned nparams, afb_data_t const params[]) const { return afb_api_broadcast_event(api_, name.c_str(), nparams, params); }
inline event api::new_event(const char *name) const { afb_event_t e; afb_api_new_event(api_, name, &e); return event(e); }
inline event api::new_event(const std::string &name) const { return new_event(name.c_str()); }
inline int api::event_handler_add(const char *pattern, api::event_cb callback, void *closure) const { return afb_api_event_handler_add(api_, pattern, callback, closure); }
inline int api::event_handler_add(const std::string &pattern, api::event_cb callback, void *closure) const { return afb_api_event_handler_add(api_, pattern.c_str(), callback, closure); }
inline int api::event_handler_del(const char *pattern, void **closure) const { return afb_api_event_handler_del(api_, pattern, closure); }
inline int api::event_handler_del(const std::string &pattern, void **closure) const { return afb_api_event_handler_del(api_, pattern.c_str(), closure); }
inline int api::set_verbs(const afb_verb_t *verbs) const { return afb_api_set_verbs(api_, verbs); }
inline int api::add_verb(const char *verb, const char *info, api::verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const { return afb_api_add_verb(api_, verb, info, callback, vcbdata, auth, session, glob); }
inline int api::add_verb(const std::string &verb, const std::string &info, api::verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const { return afb_api_add_verb(api_, verb.c_str(), info.c_str(), callback, vcbdata, auth, session, glob); }
inline int api::del_verb(const char *verb, void **vcbdata) const { return afb_api_del_verb(api_, verb, vcbdata); }
inline int api::del_verb(const std::string &verb, void **vcbdata) const { return afb_api_del_verb(api_, verb.c_str(), vcbdata); }
inline int api::provide_class(const char *name) const { return afb_api_provide_class(api_, name); }
inline int api::provide_class(const std::string &name) const { return afb_api_provide_class(api_, name.c_str()); }
inline int api::require_class(const char *name) const { return afb_api_require_class(api_, name); }
inline int api::require_class(const std::string &name) const { return afb_api_require_class(api_, name.c_str()); }
inline void api::seal() const { afb_api_seal(api_); }
inline int api::delete_api() const { return afb_api_delete(api_); }

/*************************************************************************/
/* effective members of class afb::event                                 */
/*************************************************************************/

inline event::event() : event_{nullptr} { }
inline event::event(afb_event_t e) : event_{e} { }
inline event::event(event &&other) : event_{other.extract()} { }
inline event::event(const event &other) : event_{other.twin()} { }
inline event::~event() { }
inline event &event::operator=(afb_event_t e)
{
	afb_event_t x = std::exchange(event_, e);;
	if (x)
		afb_event_unref(x);
	return *this;
}
inline event &event::operator=(const event &other) { return *this = other.twin(); }
inline event &event::operator=(event &&other) { return *this = other.extract(); }

inline afb_event_t event::extract() { return std::exchange(event_, nullptr); }
inline afb_event_t event::twin() const { return afb_event_addref(event_); }
inline event::operator afb_event_t() const { return event_; }

inline event::operator bool() const { return is_valid(); }
inline bool event::is_valid() const { return afb_event_is_valid(event_); }

inline int event::broadcast() const { return afb_event_broadcast(event_, 0, nullptr); }
inline int event::broadcast(afb_data_t data) const { return afb_event_broadcast(event_, 1, &data); }
inline int event::broadcast(unsigned nparams, afb_data_t const params[]) const { return afb_event_broadcast(event_, nparams, params); }
template <unsigned n> int event::broadcast(const dataset<n> &params) const { return broadcast(params.count(), params.data()); }
inline int event::broadcast(const std::vector<afb_data_t> &&params) const { return broadcast((unsigned)params.size(), params.data()); }

inline int event::push() const { return afb_event_push(event_, 0, nullptr); }
inline int event::push(afb_data_t data) const { return afb_event_push(event_, 1, &data); }
inline int event::push(unsigned nparams, afb_data_t const params[]) const { return afb_event_push(event_, nparams, params); }
template <unsigned n> int event::push(const dataset<n> &params) const { return push(params.count(), params.data()); }
inline int event::push(const std::vector<afb_data_t> &&params) const { return push((unsigned)params.size(), params.data()); }

inline void event::unref() { if (event_) afb_event_unref(event_); event_ = nullptr; }
inline void event::addref() { if (event_) afb_event_addref(event_); }
inline const char *event::name() const { return afb_event_name(event_); }

/*************************************************************************/
/* effective members of class afb::req                                   */
/*************************************************************************/

inline req::req(afb_req_t r) : req_(r) {}
inline req::req(const req &other) : req_(other.req_) {}
inline req &req::operator=(const req &other) { req_ = other.req_; return *this; }

inline req::operator afb_req_t() const { return req_; }
inline afb_req_t req::operator->() const { return req_; }

inline req::operator bool() const { return is_valid(); }
inline bool req::is_valid() const { return afb_req_is_valid(req_); }

inline api req::api() const { return afb_req_get_api(req_); }

inline void req::reply(int status) const noexcept
	{ afb_req_reply(req_, status, 0, nullptr); }
inline void req::reply(int status, afb_data_t data) const noexcept
	{ afb_req_reply(req_, status, 1, &data); }

inline void req::reply(int status, unsigned nreplies, afb_data_t const replies[]) const noexcept
	{ afb_req_reply(req_, status, nreplies, replies); }
template <unsigned n> void req::reply(int status, const dataset<n> &replies) const noexcept
	{ afb_req_reply(req_, status, replies.count(), replies.data()); }
inline void req::reply(int status, received_data replies) const noexcept
	{
		replies.addref();
		reply(status, replies.size(), replies.array());
	}

inline void req::reply(int status, const std::vector<afb_data_t> &&replies) const noexcept
	{ reply(status, (unsigned)replies.size(), replies.data()); }

inline void req::addref() const { afb_req_addref(req_); }

inline void req::unref() const { afb_req_unref(req_); }

inline void req::session_close() const { afb_req_session_close(req_); }

inline bool req::subscribe(const event &event) const { return !afb_req_subscribe(req_, event); }

inline bool req::unsubscribe(const event &event) const { return !afb_req_unsubscribe(req_, event); }

inline void req::subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), void *closure) const
{
	afb_req_subcall(req_, api, verb, nparams, params, flags, callback, closure);
}

template <class T>
inline void req::subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(T *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), T *closure) const
{
	subcall(api, verb, nparams, params, flags, reinterpret_cast<afb_subcall_callback_t>(callback), reinterpret_cast<void*>(closure));
}

inline int req::subcallsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, int &status, unsigned &nreplies, afb_data_t replies[]) const
{
	return afb_req_subcall_sync(req_, api, verb, nparams, params, flags, &status, &nreplies, replies);
}

inline struct json_object *req::get_client_info() const
{
	return afb_req_get_client_info(req_);
}

/* commons */
inline api root() { api a(afbBindingRoot); return a; }
inline int broadcast_event(const char *name, unsigned nparams, afb_data_t const params[])
	{ return root().broadcast_event(name, nparams, params); }

inline event new_event(const char *name)
	{ return root().new_event(name); }

inline void verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args)
	{ root().verbose(level, file, line, func, fmt, args); }

inline void verbose(int level, const char *file, int line, const char * func, const char *fmt, ...)
	{ va_list args; va_start(args, fmt); root().verbose(level, file, line, func, fmt, args); va_end(args); }

inline int require_api(const char *apiname, bool initialized)
	{ return root().require_api(apiname, int(initialized)); }

inline int logmask()
	{ return afb_get_logmask(); }

inline bool wants_errors()
	{ return AFB_SYSLOG_MASK_WANT_ERROR(logmask()); }

inline bool wants_warnings()
	{ return AFB_SYSLOG_MASK_WANT_WARNING(logmask()); }

inline bool wants_notices()
	{ return AFB_SYSLOG_MASK_WANT_NOTICE(logmask()); }

inline bool wants_infos()
	{ return AFB_SYSLOG_MASK_WANT_INFO(logmask()); }

inline bool wants_debugs()
	{ return AFB_SYSLOG_MASK_WANT_DEBUG(logmask()); }

inline void call(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], void (*callback)(void*closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api), void *closure)
{
	root().call(api, verb, nparams, params, callback, closure);
}

template <class T>
inline void call(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], void (*callback)(T*closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api), T *closure)
{
	root().call(api, verb, nparams, params, reinterpret_cast<afb_call_callback_t>(callback), reinterpret_cast<void*>(closure));
}

inline int callsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[])
{
	return root().callsync(api, verb, nparams, params, status, nreplies, replies);
}





#if LOGLEVEL_OK
inline void log_level::operator ()(const char *fmt, va_list args) const noexcept
{
	root().vverbose(*this, fmt, args);
}
inline void log_level::operator ()(const char *fmt, ...) const noexcept
{
	va_list args;
	va_start(args, fmt);
	root().vverbose(*this, fmt, args);
	va_end(args);
}
inline void log_level::operator ()(req req, const char *fmt, va_list args) const noexcept
{
	req.vverbose(*this, fmt, args);
}
inline void log_level::operator ()(req req, const char *fmt, ...) const noexcept
{
	va_list args;
	va_start(args, fmt);
	req.vverbose(*this, fmt, args);
	va_end(args);
}
inline void log_level::operator ()(api api, const char *fmt, va_list args) const noexcept
{
	api.vverbose(*this, fmt, args);
}
inline void log_level::operator ()(api api, const char *fmt, ...) const noexcept
{
	va_list args;
	va_start(args, fmt);
	api.vverbose(*this, fmt, args);
	va_end(args);
}
#endif
/*************************************************************************/
/* declaration of the binding's authorization s                          */
/*************************************************************************/

constexpr afb_auth auth_no()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_No;
	return r;
}

constexpr afb_auth auth_yes()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Yes;
	return r;
}

constexpr afb_auth auth_token()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Token;
	return r;
}

constexpr afb_auth auth_LOA(unsigned loa)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_LOA;
	r.loa = loa;
	return r;
}

constexpr afb_auth auth_permission(const char *permission)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Permission;
	r.text = permission;
	return r;
}

constexpr afb_auth auth_not(const afb_auth *other)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Not;
	r.first = other;
	return r;
}

constexpr afb_auth auth_not(const afb_auth &other)
{
	return auth_not(&other);
}

constexpr afb_auth auth_or(const afb_auth *first, const afb_auth *next)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Or;
	r.first = first;
	r.next = next;
	return r;
}

constexpr afb_auth auth_or(const afb_auth &first, const afb_auth &next)
{
	return auth_or(&first, &next);
}

constexpr afb_auth auth_and(const afb_auth *first, const afb_auth *next)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_And;
	r.first = first;
	r.next = next;
	return r;
}

constexpr afb_auth auth_and(const afb_auth &first, const afb_auth &next)
{
	return auth_and(&first, &next);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrappers for verb callbacks
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(afb::req,afb::received_data)>
void verbcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	try {
		_F_(afb::req(req), afb::received_data(nparams, params));
	}
	catch (std::exception &e) {
		AFB_REQ_ERROR(req, "Exception %s", e.what());
		afb_req_reply(req, AFB_ERRNO_INTERNAL_ERROR, 0, nullptr);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_client(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	client_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_client(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	client_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_api(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	api_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_api(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	api_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_vcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	vcb_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_vcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	vcb_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

constexpr afb_verb_t verb(
	const char *name,
	afb_req_callback_t callback,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, callback, auth, info, vcbdata, session, glob };
}

template <void (*_F_)(afb_req_t,unsigned,afb_data_t const[])>
constexpr afb_verb_t verb(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, _F_, auth, info, vcbdata, session, glob };
}

template <void (*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb<_F_>, auth, info, vcbdata, session, glob };
}

/*
template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb_client(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_client<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
constexpr afb_verb_t verb_client(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_client<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb_api(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_api<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
constexpr afb_verb_t verb_api(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_api<_C_, _F_>, auth, info, vcbdata, session, glob };
}
*/

constexpr afb_verb_t verbend()
{
	return { 0, 0, 0, 0, 0, 0, 0 };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrappers for binding callbacks
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

typedef afb_ctlid_t ctlid;
typedef afb_ctlarg_t ctlarg;

template <int (*_F_)(afb_api_t, afb_ctlid_t, const afb_ctlarg_t, void *)>
constexpr afb_binding_t binding(
	const char *name,
	const afb_verb_t *verbs,
	const char *info,
	const char *specification = nullptr,
	bool noconcurrency = false,
	void *userdata = nullptr
)
{
	return {
		name, specification, info, verbs, _F_, userdata,
		nullptr, nullptr, nullptr, static_cast<unsigned>(noconcurrency) };
};

template <int (*_F_)(afb::api, afb::ctlid, const afb::ctlarg, void *)>
int bindingcb(afb_api_t api, afb_ctlid_t ctlid, const afb_ctlarg_t ctlarg, void *userdata)
{
	try {
		return _F_(afb::api(api), ctlid, ctlarg, userdata);
	}
	catch(std::exception &e) {
		AFB_API_ERROR(api, "Exception %s", e.what());
		return AFB_ERRNO_INTERNAL_ERROR;
	}
}

struct binding_
{
	afb_binding_t value;

	constexpr binding_(
		const char *api,
		const char *specification,
		const char *info,
		const struct afb_verb_v4 *verbs,
		afb_api_callback_x4_t mainctl,
		void *userdata,
		const char *provide_class,
		const char *require_class,
		const char *require_api,
		unsigned noconcurrency)
			: value{ api, specification, info, verbs,
				mainctl, userdata, provide_class,
				require_class, require_api, noconcurrency }
			{}

	constexpr binding_(const char *api)
			: value{ api, nullptr, nullptr, nullptr,
				nullptr, nullptr, nullptr,
				nullptr, nullptr, 0 }
			{}


	constexpr operator afb_binding_t () const { return value; }


	constexpr binding_ specification(const char *specification) const
		{ return  binding_(
				value.api, specification, value.info, value.verbs,
				value.mainctl, value.userdata, value.provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ info(const char *info) const
		{ return  binding_(
				value.api, value.specification, info, value.verbs,
				value.mainctl, value.userdata, value.provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ verbs(const struct afb_verb_v4 *verbs) const
		{ return  binding_(
				value.api, value.specification, value.info, verbs,
				value.mainctl, value.userdata, value.provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ mainctl(afb_api_callback_x4_t mainctl) const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				mainctl, value.userdata, value.provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	template <int (*_F_)(afb::api, afb::ctlid, const afb::ctlarg, void *)>
	constexpr binding_ mainctl() const
		{ return mainctl(bindingcb<_F_>); }

	constexpr binding_ userdata(void *userdata) const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				value.mainctl, userdata, value.provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ provide_class(const char *provide_class) const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				value.mainctl, value.userdata, provide_class,
				value.require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ require_class(const char *require_class) const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				value.mainctl, value.userdata, value.provide_class,
				require_class, value.require_api, value.noconcurrency); }

	constexpr binding_ require_api(const char *require_api) const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				value.mainctl, value.userdata, value.provide_class,
				value.require_class, require_api, value.noconcurrency); }

	constexpr binding_ noconcurency() const
		{ return  binding_(
				value.api, value.specification, value.info, value.verbs,
				value.mainctl, value.userdata, value.provide_class,
				value.require_class, value.require_api, 1); }
};

constexpr binding_ binding(
	const char *name,
	const afb_verb_t *verbs = nullptr,
	const char *info = nullptr,
	afb_api_callback_t mainctl = nullptr,
	const char *specification = nullptr,
	bool noconcurrency = false,
	void *userdata = nullptr
)
{
	return binding_(
		name, specification, info, verbs, mainctl, userdata,
		nullptr, nullptr, nullptr, static_cast<unsigned>(noconcurrency) );
};

template <int (*_F_)(afb::api, afb::ctlid, const afb::ctlarg, void *)>
constexpr binding_ binding(
	const char *name,
	const afb_verb_t *verbs = nullptr,
	const char *info = nullptr,
	const char *specification = nullptr,
	bool noconcurrency = false,
	void *userdata = nullptr
)
{
	return binding_(
		name, specification, info, verbs, bindingcb<_F_>, userdata,
		nullptr, nullptr, nullptr, static_cast<unsigned>(noconcurrency) );
};


/*************************************************************************/
/***                         E N D                                     ***/
/*************************************************************************/
}
