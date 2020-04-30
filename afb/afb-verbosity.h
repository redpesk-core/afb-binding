/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/** @defgroup AFB_LOGGING
 *  @{ */

#define AFB_SYSLOG_LEVEL_EMERGENCY	0
#define AFB_SYSLOG_LEVEL_ALERT		1
#define AFB_SYSLOG_LEVEL_CRITICAL	2
#define AFB_SYSLOG_LEVEL_ERROR		3
#define AFB_SYSLOG_LEVEL_WARNING	4
#define AFB_SYSLOG_LEVEL_NOTICE		5
#define AFB_SYSLOG_LEVEL_INFO		6
#define AFB_SYSLOG_LEVEL_DEBUG		7

#define AFB_SYSLOG_MASK_WANT(verbomask,level)	((verbomask) & (1 << (level)))

#define AFB_SYSLOG_MASK_WANT_EMERGENCY(x)	AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_EMERGENCY)
#define AFB_SYSLOG_MASK_WANT_ALERT(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_ALERT)
#define AFB_SYSLOG_MASK_WANT_CRITICAL(x)	AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_CRITICAL)
#define AFB_SYSLOG_MASK_WANT_ERROR(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_ERROR)
#define AFB_SYSLOG_MASK_WANT_WARNING(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_WARNING)
#define AFB_SYSLOG_MASK_WANT_NOTICE(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_NOTICE)
#define AFB_SYSLOG_MASK_WANT_INFO(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_INFO)
#define AFB_SYSLOG_MASK_WANT_DEBUG(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_DEBUG)

/**
 * Transform a mask of verbosity to its significant level of verbosity.
 * 
 * @param verbomask the mask
 * 
 * @return the upper level that is not null, truncated to AFB_SYSLOG_LEVEL_DEBUG
 * 
 * @example _afb_verbomask_to_upper_level_(5) -> 2
 * @example _afb_verbomask_to_upper_level_(16) -> 4
 */
static inline int _afb_verbomask_to_upper_level_(int verbomask)
{
	int result = 0;
	while ((verbomask >>= 1) && result < AFB_SYSLOG_LEVEL_DEBUG)
		result++;
	return result;
}

/*
 * Macros for logging messages
 */

#if defined(AFB_BINDING_PRAGMA_NO_VERBOSE_DATA)

# define AFB_API_VERBOSE(api,level,...) \
		do { if(level <= AFB_VERBOSITY_LEVEL_ERROR) \
			afb_api_verbose(api,level,__FILE__,__LINE__,NULL,__VA_ARGS__); \
		else afb_api_verbose(api,level,__FILE__,__LINE__,NULL); } while(0)

# define AFB_REQ_VERBOSE(req,level,...) \
		do { if(level <= AFB_VERBOSITY_LEVEL_ERROR) \
			afb_req_verbose(req,level,__FILE__,__LINE__,NULL,__VA_ARGS__); \
		else afb_req_verbose(req,level,__FILE__,__LINE__,NULL); } while(0)

#elif defined(AFB_BINDING_PRAGMA_NO_VERBOSE_DETAILS)

# define AFB_API_VERBOSE(api,level,...) \
	afb_api_verbose(api,level,NULL,0,NULL,__VA_ARGS__)

# define AFB_REQ_VERBOSE(req,level,...) \
	afb_req_verbose(req,level,NULL,0,NULL,__VA_ARGS__)

#else

# define AFB_API_VERBOSE(api,level,...) \
	afb_api_verbose(api,level,__FILE__,__LINE__,__func__,__VA_ARGS__)

# define AFB_REQ_VERBOSE(req,level,...) \
	afb_req_verbose(req,level,__FILE__,__LINE__,__func__,__VA_ARGS__)

#endif

#define _AFB_API_LOGGING_(api,llevel,...) \
        do{ if(afb_api_wants_log_level((api),(llevel))) AFB_API_VERBOSE((api),(llevel),__VA_ARGS__); }while(0)
#define _AFB_REQ_LOGGING_(req,llevel,...) \
        do{ if(afb_req_wants_log_level((req),(llevel))) AFB_REQ_VERBOSE((req),(llevel),__VA_ARGS__); }while(0)

#define AFB_API_ERROR(api,...)		_AFB_API_LOGGING_(api,AFB_SYSLOG_LEVEL_ERROR,__VA_ARGS__)
#define AFB_API_WARNING(api,...)	_AFB_API_LOGGING_(api,AFB_SYSLOG_LEVEL_WARNING,__VA_ARGS__)
#define AFB_API_NOTICE(api,...)		_AFB_API_LOGGING_(api,AFB_SYSLOG_LEVEL_NOTICE,__VA_ARGS__)
#define AFB_API_INFO(api,...)		_AFB_API_LOGGING_(api,AFB_SYSLOG_LEVEL_INFO,__VA_ARGS__)
#define AFB_API_DEBUG(api,...)		_AFB_API_LOGGING_(api,AFB_SYSLOG_LEVEL_DEBUG,__VA_ARGS__)

#define AFB_REQ_ERROR(req,...)		_AFB_REQ_LOGGING_(req,AFB_SYSLOG_LEVEL_ERROR,__VA_ARGS__)
#define AFB_REQ_WARNING(req,...)	_AFB_REQ_LOGGING_(req,AFB_SYSLOG_LEVEL_WARNING,__VA_ARGS__)
#define AFB_REQ_NOTICE(req,...)		_AFB_REQ_LOGGING_(req,AFB_SYSLOG_LEVEL_NOTICE,__VA_ARGS__)
#define AFB_REQ_INFO(req,...)		_AFB_REQ_LOGGING_(req,AFB_SYSLOG_LEVEL_INFO,__VA_ARGS__)
#define AFB_REQ_DEBUG(req,...)		_AFB_REQ_LOGGING_(req,AFB_SYSLOG_LEVEL_DEBUG,__VA_ARGS__)

#define AFB_ERROR(...)			AFB_API_ERROR(afb_get_root_api(),__VA_ARGS__)
#define AFB_WARNING(...)		AFB_API_WARNING(afb_get_root_api(),__VA_ARGS__)
#define AFB_NOTICE(...)			AFB_API_NOTICE(afb_get_root_api(),__VA_ARGS__)
#define AFB_INFO(...)			AFB_API_INFO(afb_get_root_api(),__VA_ARGS__)
#define AFB_DEBUG(...)			AFB_API_DEBUG(afb_get_root_api(),__VA_ARGS__)

/** @} */
