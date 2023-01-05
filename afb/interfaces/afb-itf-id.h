/*
 * Copyright (C) 2015-2023 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/*
 * @defgroup specific interfaces
 * @{
 */
enum afb_itf_id
{
	/** for special uses, like being a marker */
	AFB_ITF_ID_NONE = 0,

	/** for the HTTP interface V1 for requests X4 */
	AFB_ITF_ID_REQ_HTTP_X4V1 = 1,

	/* mark the end of the predefined ids */
	_AFB_ITF_ID_END_
};

typedef enum afb_itf_id afb_itf_id_t;

static inline
int
afb_itf_id_is_valid(
	int id
) {
	return AFB_ITF_ID_NONE <= id && id < _AFB_ITF_ID_END_;
}

/* @} */