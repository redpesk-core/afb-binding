# Binding v3 to v4

> **authors**: Valentin Lefebvre <valentin.lefebvre@iot.bzh>
> **date**: 2022, june 8th

This documentations will touch on change to proceed for upgrade a binding written in V3 to a binding written V4.
To see a former document about v4 migration which compares all `afb_*` functions between v3 and v4 bindings, please follow this [link](http://git.ovh.iot/redpesk/redpesk-core/afb-binding/-/blob/master-next/migration-to-binding-v4.md).

## Summary

[1. Entry point](#1-entry-point)
[2. verbs](#2-verbs)
[3. config file](#3-configuration-file)
[4. plugins](#4-plugins)
[5. afb functions](#5-afb-functions)

## 1. Entry point

### a) structure definition

- **---V3---**

    ```c
    /**
    * @brief Structure of the binding
    */
    const struct afb_binding_v3 afbBindingV3 = {
        .api = "ble-binding",
        .specification = NULL,
        .info = "Service which manage bluetooth low energy devices.",
        .verbs = _afb_verbs_ble,    // <-- list of verbs
        .preinit = binding_preinit, // <-- preinit handler
        .init = binding_init,       // <-- init handler
        .userdata = NULL,
        .provide_class = NULL,
        .require_class = NULL,
        .require_api = NULL,
        .noconcurrency = 0
    };
    ```

- **---V4---**

    In V4 you will lose *init* and *preinit* section. Instead You will inform about a *controller* handler which will take care about the init and preinit !

    ```c
    /**
    * @brief Structure of the binding
    *
    */
    const struct afb_binding_v4 afbBindingV4 = {
        .api = "ble-binding",
        .specification = NULL,
        .info = "Service which manage bluetooth low energy devices.",
        .verbs = _afb_verbs_ble,    // <-- list of verbs
        .mainctl = binding_ctl,     // <-- main handler
        .userdata = NULL,
        .provide_class = NULL,
        .require_class = NULL,
        .require_api = NULL,
        .noconcurrency = 0
    };
    ```

### b) main functions

- **---V3---**

    ```c
    /**
    * @brief Binding preinit Callback
    *
    * @param api   the api that receive the callback
    */
    int binding_preinit(afb_api_t api);

    /**
    * @brief Binding init Callback
    *
    * @param api   the api that receive the callback
    */
    int binding_init(afb_api_t api);
    ```

- **---V4---**

    The new main handler will be called multiple time. Each time `ctlid` will show you in which process you are (Pre init, init, etc...)

    ```c
    /**
    * @brief Binding Callback
    *
    * @param api       the api that receive the callback
    * @param ctlid     identifier of the reason of the call (@see afb_ctlid)
    * @param ctlarg    data associated to the call
    * @param userdata  the userdata of the api (@see afb_api_get_userdata)
    */
    int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata) {
        switch(ctlid) {
            case afb_ctlid_Root_Entry:
                AFB_API_NOTICE(api, "Unexpected Root Entry at path=%s", ctlarg->root_entry.path);
                break;

            case afb_ctlid_Pre_Init:
                AFB_API_NOTICE(api, "Pre-initialization at path=%s", ctlarg->pre_init.path);
                if (_preinit_binding(api, ctlarg->pre_init.config) < 0 ){
                    AFB_API_ERROR(api, "Failed during pre-initialization");
                    return -1;
                }
                afb_api_seal(api);
                AFB_API_NOTICE(api, "Preinitialization finished");
                break;

            case afb_ctlid_Init:
                AFB_API_NOTICE(api, "Initialization");
                if ( _init_binding(api) < 0) {
                    AFB_API_ERROR(api, "Failed during Initialization");
                    return -1;
                }
                AFB_API_NOTICE(api, "Initialization finished");
                break;

            case afb_ctlid_Class_Ready:
                AFB_API_NOTICE(api, "Required classes are ready");
                break;

            case afb_ctlid_Orphan_Event:
                AFB_API_NOTICE(api, "The event %s is not handled", ctlarg->orphan_event.name);
                break;

            case afb_ctlid_Exiting:
                AFB_API_NOTICE(api, "Shuting down was called wide code = %i", ctlarg->exiting.code);
                break;

            default:
                break;
        }
        return 0;
    }
    ```

## 2. Verbs

### a) vers handler

- **---V3---**

    ```c
    /**
    * @brief Ping verb Callback
    *
    * @param request   Client request
    */
    void ping(afb_req_t request);

    /**
    * @brief Info verb Callback
    *
    * @param request   Client request
    */
    void info (afb_req_t request);

    /**
    * @brief List of api's verbs
    *
    */
    static const struct afb_verb_v3 _afb_verbs_ble[] = {
        {
            .verb = "ping",
            .callback = ping,
            .auth = NULL,
            .info = "Check if binding running well by ping-pong request",
            .vcbdata = NULL,
            .session = AFB_SESSION_NONE,
            .glob = 0
        },
        {
            .verb = "info",
            .callback = info,
            .auth = NULL,
            .info = "Get current ble-binding configuration",
            .vcbdata = NULL,
            .session = AFB_SESSION_NONE,
            .glob = 0
        }
    }
    ```

- **---V4---**

    Verbs are now declared with the request folowing by data like a c-standard main

    ```c
    /**
    * @brief Check if binding running well by ping-pong request
    *
    * @param request   Client request
    * @param argc      Arguments count
    * @param argv      array of arguments
    */
    void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]);

    /**
    * @brief Get current ble-binding configuration
    *
    * @param request   Client request
    * @param argc      Arguments count
    * @param argv      array of arguments
    */
    void info(afb_req_t request, unsigned argc, afb_data_t const argv[]);

    /**
    * @brief List of api's verbs
    *
    */
    static const struct afb_verb_v4 _afb_verbs_ble[] = {
        {
            .verb = "ping",
            .callback = ping,
            .auth = NULL,
            .info = "Check if binding running well by ping-pong request",
            .vcbdata = NULL,
            .session = AFB_SESSION_NONE,
            .glob = 0
        },
        {
            .verb = "info",
            .callback = info,
            .auth = NULL,
            .info = "Get current ble-binding configuration",
            .vcbdata = NULL,
            .session = AFB_SESSION_NONE,
            .glob = 0
        }
    };
    ```

### b) Get arguments

- **---V3---**

    In V3 argument can be found directly in JSON in the request structure:

    ```c
    // get arguments
    json_object *argsJ = afb_req_json(request);
    // Parse arguments from argJ
    ...
    ```

- **---V4---**

    In V4 you will get verbs arguments at *function arguments*. In *afb_data_t* type. You can then convert in JSON in case of arguments staying in JSON format.

    ```c
    // Get data
    afb_data_t arg_data;
    if (afb_req_param_convert(request, 0, AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0) {
        afb_req_reply(request, AFB_ERRNO_INVALID_REQUEST, 0, NULL);
        return;
    }
    // Convert data in JSON
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json) {
        afb_req_reply(request, AFB_ERRNO_INVALID_REQUEST, 0, NULL);
        return;
    }
    // Parse arguments from args_json
    ...
    ```

### c) push response

- **---V3---**

    ```c
    // Example with json
    afb_req_success(req, responseJ, NULL);
    // Example for string:
    afb_req_success(req,NULL,"response is in string!");
    ```

- **---V4---**

    In this new version, you should create an *afb_data_t*

    ```c
    // Exemple with info verb for JSON:
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_JSON_C, binding_data.info_json, 0, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    // Example with error message for string:
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, errorMsg, sizeof(errorMsg)+1, NULL, NULL);
    afb_req_reply(request, STATUS_ERROR, 1, &reply);
    ```

## 3. configuration file

- **---V3---**

    In this version, json configuration file would be get via **lib controller** with a specific path and a specific middle-name...

- **---V4---**

    With this new version, config file will be give in the command line **afb-binding**

    ```bash
    afb-binder --binding=${BINDING_PATH}/lib/afb-ble-binding.so:${BINDING_PLUGIN_PATH}/plugin/ciss-bosh-ble-plugin.json
    ```

    Then you will be able to get, in source code, the config in *binding_ctl()* function at pre-init process, like this:

    > ```c
    > ctlarg->pre_init.config
    > ```

    ```c
    int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata) {
        switch(ctlid) {
        // ...
        case afb_ctlid_Pre_Init:
                    AFB_API_NOTICE(api, "Pre-initialization at path=%s", ctlarg->pre_init.path);
                    if (_preinit_binding(api, ctlarg->pre_init.config) < 0 ){ // <-- pre init the binding with the JSON config
                        AFB_API_ERROR(api, "Failed during pre-initialization");
                        return -1;
                    }
                    afb_api_seal(api);
                    AFB_API_NOTICE(api, "Preinitialization finished");
                    break;
        // ...
        }
    ```

## 4. plugins

- **---V3---**
    Plugins was deals with **lib controllers**

- **---V4---**

    Now plugins should be deals with new functions of libhelpers. Sources can be found at **afb-libhelpers** repo in **afb-plugin-helpers.c**

Here is an example of how to load a plugin thanks a config JSON file. We will load the plugin **ciss-bosh-ble-sensor.so** and call one of this public function **catch_data()**

1. config file JSON
   ```bash
    {
        "metadata": {
            "uid": "afb-ble-binding",
            "version": "1.0.0",
            "api": "ble-binding",
            "info": "Config sample for Bluetooth Low Energy binding"
        },
        "plugins": {
            "uid": "ciss-bosh-ble-sensor-plugin",
            "info": "A plugin to catch Ciss bosh ble sensor",
            "libs": "ciss-bosh-ble-sensor.so"
        },
        "sensors": [
            {
                "handler": "ciss-bosh-ble-sensor-plugin#catch_data"
            }
        ]
    }
    ```

2. source code - Should be at preinit process !

    ```c
    #include <afb-libhelpers/afb-plugin-helpers.h>

    // Get info from config
    char *uid = NULL;
    char *libs = NULL;
    ret = wrap_json_unpack(plugins_json, "{ss ss}"
                , "uid", &uid, "libs", &libs);

    // Load the shared library
    plugins_store *pstore;
    plugin_load(afbBindingRoot, uid, libs, &pstore);

    // Get the callback pointer of the plugin
    char function_name[] = "catch_data";
    void *callback = plugin_get_cb(afbBindingRoot, pstore, function_name);

    // Call the callback
    ret = callback();
    ```

## 5. afb functions

### a) timers

- **---V3---**
    In V3 timers event can be used with function **TimerEvtStart** and **TimerEvtStop**

- **---V4---**
    Both these function don't exist anymore : use **afb_timer_create** instead :  [doc](http://redpesk-doc-internal.lorient.iot/docs/en/master/developer-guides/reference-v4/func-afb-timer.html).
