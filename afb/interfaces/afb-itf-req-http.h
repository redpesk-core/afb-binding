/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include "../afb-binding.h"
#include "afb-itf-id.h"

/*************************************************************
* The interface to HTTP functions is only available for bindings V4
*************************************************************/

/**
 * Name of the HTTP interfaces
 */
#define AFB_ITF_NAME_REQ_HTTP_X4V1 "REQ/HTTP@X4.1"

/**
 * Definitions of HTTP interfaces
 */
struct afb_itf_req_http_x4v1
{
	/** get an header value */
	int (*header)(afb_req_x4_t req, const char *name, const char **value);

	/** reply a redirect to the given URL */
	int (*redirect)(afb_req_x4_t req, const char *url, int add_query_part, int permanent);

	/** reply a single data with content type and headers */
	int (*reply)(afb_req_x4_t req, int code, afb_data_x4_t data, const char *type, const char **headers);
};

typedef const struct afb_itf_req_http_x4v1 *afb_itf_req_http_x4v1_t;

#if AFB_BINDING_VERSION >= 4

#define AFB_ITF_ID_REQ_HTTP_V1   AFB_ITF_ID_REQ_HTTP_X4V1
#define AFB_ITF_NAME_REQ_HTTP_V1 AFB_ITF_NAME_REQ_HTTP_X4V1
typedef afb_itf_req_http_x4v1_t  afb_itf_req_http_v1_t;

/* current version is version 1 */
#define AFB_ITF_ID_REQ_HTTP       AFB_ITF_ID_REQ_HTTP_V1
#define AFB_ITF_NAME_REQ_HTTP     AFB_ITF_NAME_REQ_HTTP_V1
#define afb_itf_req_http_t        afb_itf_req_http_v1_t
#define afb_req_get_itf_req_http  afb_req_get_itf_req_http_v1
#define afb_itf_req_http_header   afb_itf_req_http_header_v1
#define afb_itf_req_http_redirect afb_itf_req_http_redirect_v1
#define afb_itf_req_http_reply    afb_itf_req_http_reply_v1

/**
 * Get in itf the HTTP interface for the request or check if existing
 *
 * @param req    the request
 * @param itf    if not NULL, where to store the interface pointer
 *
 * @return 0 in case of success or a negative code
 */
static inline
int
afb_req_get_itf_req_http_v1(
	afb_req_t req,
	afb_itf_req_http_v1_t *itf
) {
	return afb_req_get_interface_by_id(req, AFB_ITF_ID_REQ_HTTP_V1, (void**)itf);
}

/**
 * Get an header value of the HTTP request
 *
 * @param itf    the interface pointer
 * @param req    the request
 * @param name   name of the header
 * @param value  where to store the value, can not be NULL
 *
 * @return 0 in case of success or a negative code
 */
static inline
int
afb_itf_req_http_header_v1(
	afb_itf_req_http_v1_t itf,
	afb_req_t req,
	const char *name,
	const char **value
) {
	return itf->header(req, name, value);
}

/**
 * Send a redirect reply for the request
 *
 * @param itf    the interface pointer
 * @param req    the request
 * @param url    the URL of the redirection
 * @param absolute if not null the url is absolute, otherwise it is relative
 * @param permanent if not null the redirect is permanent otherwise it is temporarily
 *
 * @return 0 in case of success or a negative code
 */
static inline
int
afb_itf_req_http_redirect_v1(
	afb_itf_req_http_v1_t itf,
	afb_req_t req,
	const char *url,
	int absolute,
	int permanent
) {
	return itf->redirect(req, url, absolute, permanent);
}

/**
 * Send an HTTP reply with optionnaly headers and/or content-type
 *
 * @param itf    the interface pointer
 * @param req    the request
 * @param code   the HTTP code of the reply
 * @param data   the data to send or NULL
 * @param type   the content type of the data or NULL
 * @param headers if not NULL an array of string pointers NULL terminated
 *                the count of string must be even being KEY/VALUE pairs
 *
 * @return 0 in case of success or a negative code
 */
static inline
int
afb_itf_req_http_reply_v1(
	afb_itf_req_http_v1_t itf,
	afb_req_t req,
	int code,
	afb_data_t data,
	const char *type,
	const char **headers
) {
	return itf->reply(req, code, data, type, headers);
}

#endif