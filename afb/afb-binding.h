/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/**
 * @mainpage
 *
 * @section brief Brief introduction
 *
 * This is part of the application framework binder micro-service and
 * is provided as the API for writing bindings.
 *
 * The normal usage is to include only one file as below:
 *
 * ```C
 * #define AFB_BINDING_VERSION 3
 * #include <afb/afb-binding.h>
 * ```
 *
 */
/**
 * @file afb/afb-binding.h
 */

#include <stdarg.h>
#include <stdint.h>
#include <json-c/json.h>

/**
 * @def AFB_BINDING_INTERFACE_VERSION

 *  * Version of the binding interface.
 *
 * This is intended to be test for tuning condition code.
 * It is of the form MAJOR * 1000 + REVISION.
 *
 * @see AFB_BINDING_UPPER_VERSION that should match MAJOR
 */
#define AFB_BINDING_INTERFACE_VERSION 3000

/**
 * @def AFB_BINDING_LOWER_VERSION
 *
 * Lowest binding API version supported.
 *
 * @see AFB_BINDING_VERSION
 * @see AFB_BINDING_UPPER_VERSION
 */
#define AFB_BINDING_LOWER_VERSION     1

/**
 * @def AFB_BINDING_UPPER_VERSION
 *
 * Upper binding API version supported.
 *
 * @see AFB_BINDING_VERSION
 * @see AFB_BINDING_LOWER_VERSION
 */
#define AFB_BINDING_UPPER_VERSION     3

/**
 * @def AFB_BINDING_VERSION
 *
 * This macro must be defined before including <afb/afb-binding.h> to set
 * the required binding API.
 */

#ifndef AFB_BINDING_VERSION
#error "\
\n\
\n\
  AFB_BINDING_VERSION should be defined before including <afb/afb-binding.h>\n\
  AFB_BINDING_VERSION defines the version of binding that you use.\n\
  Currently the version to use is 3 (older versions: 1 and 2 are obsolete).\n\
  Consider to add one of the following define before including <afb/afb-binding.h>:\n\
\n\
    #define AFB_BINDING_VERSION 3\n\
\n\
"
#elif AFB_BINDING_VERSION == 1
#  error "Support of binding version 1 is removed! Please switch to version 3."
#elif AFB_BINDING_VERSION == 2
#  error "Support of binding version 2 is removed! Please switch to version 3."
#endif

#if AFB_BINDING_VERSION != 0
# if AFB_BINDING_VERSION < AFB_BINDING_LOWER_VERSION || AFB_BINDING_VERSION > AFB_BINDING_UPPER_VERSION
#  error "Unsupported binding version AFB_BINDING_VERSION"
# endif
#endif

/***************************************************************************************************/
#include "afb-binding-predefs.h"
#include "afb-binding-v3.h"
#include "afb-binding-postdefs.h"
