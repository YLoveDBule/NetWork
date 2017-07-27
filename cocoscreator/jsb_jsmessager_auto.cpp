#include "jsb_jsmessager_auto.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "JSMessager.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
JSClass  *jsb_JSMessager_class;
JSObject *jsb_JSMessager_prototype;

bool js_jsmessager_JSMessager_onConnectShutdown(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_onConnectShutdown : Invalid Native Object");
    if (argc == 2) {
        unsigned short arg0 = 0;
        uint16_t arg1;
        ok &= jsval_to_ushort(cx, args.get(0), &arg0);
        ok &= jsval_to_uint16(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_onConnectShutdown : Error processing arguments");
        bool ret = cobj->onConnectShutdown(arg0, arg1);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_onConnectShutdown : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_jsmessager_JSMessager_pushLString(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_pushLString : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        const char* arg1 = nullptr;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_pushLString : Error processing arguments");
        cobj->pushLString(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_pushLString : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_jsmessager_JSMessager_closeConnet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_closeConnet : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_closeConnet : Error processing arguments");
        cobj->closeConnet(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_closeConnet : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_jsmessager_JSMessager_houseProcMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_houseProcMsg : Invalid Native Object");
    if (argc == 5) {
        int arg0 = 0;
        unsigned short arg1 = 0;
        unsigned short arg2 = 0;
        void* arg3 = nullptr;
        unsigned short arg4 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_ushort(cx, args.get(1), &arg1);
        ok &= jsval_to_ushort(cx, args.get(2), &arg2);
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        ok &= jsval_to_ushort(cx, args.get(4), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_houseProcMsg : Error processing arguments");
        bool ret = cobj->houseProcMsg(arg0, arg1, arg2, arg3, arg4);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_houseProcMsg : wrong number of arguments: %d, was expecting %d", argc, 5);
    return false;
}
bool js_jsmessager_JSMessager_ask2Subserver(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_ask2Subserver : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        const char* arg1 = nullptr;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_ask2Subserver : Error processing arguments");
        cobj->ask2Subserver(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_ask2Subserver : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_jsmessager_JSMessager_dispatchNetStatusMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_dispatchNetStatusMsg : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->dispatchNetStatusMsg();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_dispatchNetStatusMsg : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_jsmessager_JSMessager_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_jsmessager_JSMessager_sendToJs(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_sendToJs : Invalid Native Object");
    if (argc == 5) {
        int arg0 = 0;
        unsigned short arg1 = 0;
        unsigned short arg2 = 0;
        void* arg3 = nullptr;
        unsigned short arg4 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_ushort(cx, args.get(1), &arg1);
        ok &= jsval_to_ushort(cx, args.get(2), &arg2);
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        ok &= jsval_to_ushort(cx, args.get(4), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_sendToJs : Error processing arguments");
        bool ret = cobj->sendToJs(arg0, arg1, arg2, arg3, arg4);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_sendToJs : wrong number of arguments: %d, was expecting %d", argc, 5);
    return false;
}
bool js_jsmessager_JSMessager_dispatchNetMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_dispatchNetMsg : Invalid Native Object");
    if (argc == 0) {
        cobj->dispatchNetMsg();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_dispatchNetMsg : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_jsmessager_JSMessager_endSendMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_endSendMsg : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_endSendMsg : Error processing arguments");
        cobj->endSendMsg(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_endSendMsg : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_jsmessager_JSMessager_houseNetMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_houseNetMsg : Invalid Native Object");
    if (argc == 5) {
        int arg0 = 0;
        unsigned short arg1 = 0;
        unsigned short arg2 = 0;
        void* arg3 = nullptr;
        unsigned short arg4 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_ushort(cx, args.get(1), &arg1);
        ok &= jsval_to_ushort(cx, args.get(2), &arg2);
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        ok &= jsval_to_ushort(cx, args.get(4), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_houseNetMsg : Error processing arguments");
        bool ret = cobj->houseNetMsg(arg0, arg1, arg2, arg3, arg4);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_houseNetMsg : wrong number of arguments: %d, was expecting %d", argc, 5);
    return false;
}
bool js_jsmessager_JSMessager_beginSendMsg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_beginSendMsg : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_beginSendMsg : Error processing arguments");
        cobj->beginSendMsg(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_beginSendMsg : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_jsmessager_JSMessager_pushServerAskResult(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_pushServerAskResult : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_pushServerAskResult : Error processing arguments");
        bool ret = cobj->pushServerAskResult(arg0, arg1);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_pushServerAskResult : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_jsmessager_JSMessager_sendMessage(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSMessager* cobj = nullptr;

    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx);
    obj.set(args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (JSMessager *)(proxy ? proxy->ptr : nullptr);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_sendMessage : Invalid Native Object");
    do {
        bool ok = true;
        if (argc == 5) {
            int arg0 = 0;
            ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
            if (!ok) { ok = true; break; }
            unsigned short arg1 = 0;
            ok &= jsval_to_ushort(cx, args.get(1), &arg1);
            if (!ok) { ok = true; break; }
            unsigned short arg2 = 0;
            ok &= jsval_to_ushort(cx, args.get(2), &arg2);
            if (!ok) { ok = true; break; }
            void* arg3 = nullptr;
            #pragma warning NO CONVERSION TO NATIVE FOR void*
			ok = false;
            if (!ok) { ok = true; break; }
            unsigned int arg4 = 0;
            ok &= jsval_to_uint32(cx, args.get(4), &arg4);
            if (!ok) { ok = true; break; }
            bool ret = cobj->sendMessage(arg0, arg1, arg2, arg3, arg4);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    do {
        bool ok = true;
        if (argc == 3) {
            int arg0 = 0;
            ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
            if (!ok) { ok = true; break; }
            unsigned short arg1 = 0;
            ok &= jsval_to_ushort(cx, args.get(1), &arg1);
            if (!ok) { ok = true; break; }
            unsigned short arg2 = 0;
            ok &= jsval_to_ushort(cx, args.get(2), &arg2);
            if (!ok) { ok = true; break; }
            bool ret = cobj->sendMessage(arg0, arg1, arg2);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_jsmessager_JSMessager_sendMessage : wrong number of arguments");
    return false;
}
bool js_jsmessager_JSMessager_pushNum(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_pushNum : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        void* arg1 = nullptr;
        unsigned int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        ok &= jsval_to_uint32(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_pushNum : Error processing arguments");
        cobj->pushNum(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_pushNum : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_jsmessager_JSMessager_pushLStringShortSize(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_pushLStringShortSize : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        const char* arg1 = nullptr;
        int32_t arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        ok &= jsval_to_int32(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_jsmessager_JSMessager_pushLStringShortSize : Error processing arguments");
        cobj->pushLStringShortSize(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_pushLStringShortSize : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_jsmessager_JSMessager_closeAllConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JSMessager* cobj = (JSMessager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_jsmessager_JSMessager_closeAllConnect : Invalid Native Object");
    if (argc == 0) {
        cobj->closeAllConnect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_jsmessager_JSMessager_closeAllConnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_jsmessager_JSMessager_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        JSMessager* ret = JSMessager::getInstance();
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<JSMessager>(cx, (JSMessager*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_jsmessager_JSMessager_getInstance : wrong number of arguments");
    return false;
}

bool js_jsmessager_JSMessager_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JSMessager* cobj = new (std::nothrow) JSMessager();

    js_type_class_t *typeClass = js_get_type_from_native<JSMessager>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "JSMessager"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_JSMessager_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (JSMessager)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        JSMessager *nobj = static_cast<JSMessager *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            JS::RootedValue flagValue(cx);
            JS_GetProperty(cx, jsobj, "__cppCreated", &flagValue);
            if (flagValue.isNullOrUndefined()){
                delete nobj;
            }
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_jsmessager_JSMessager(JSContext *cx, JS::HandleObject global) {
    jsb_JSMessager_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_JSMessager_class->name = "JSMessager";
    jsb_JSMessager_class->addProperty = JS_PropertyStub;
    jsb_JSMessager_class->delProperty = JS_DeletePropertyStub;
    jsb_JSMessager_class->getProperty = JS_PropertyStub;
    jsb_JSMessager_class->setProperty = JS_StrictPropertyStub;
    jsb_JSMessager_class->enumerate = JS_EnumerateStub;
    jsb_JSMessager_class->resolve = JS_ResolveStub;
    jsb_JSMessager_class->convert = JS_ConvertStub;
    jsb_JSMessager_class->finalize = js_JSMessager_finalize;
    jsb_JSMessager_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("onConnectShutdown", js_jsmessager_JSMessager_onConnectShutdown, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pushLString", js_jsmessager_JSMessager_pushLString, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("closeConnet", js_jsmessager_JSMessager_closeConnet, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("houseProcMsg", js_jsmessager_JSMessager_houseProcMsg, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ask2Subserver", js_jsmessager_JSMessager_ask2Subserver, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dispatchNetStatusMsg", js_jsmessager_JSMessager_dispatchNetStatusMsg, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_jsmessager_JSMessager_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendToJs", js_jsmessager_JSMessager_sendToJs, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dispatchNetMsg", js_jsmessager_JSMessager_dispatchNetMsg, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("endSendMsg", js_jsmessager_JSMessager_endSendMsg, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("houseNetMsg", js_jsmessager_JSMessager_houseNetMsg, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("beginSendMsg", js_jsmessager_JSMessager_beginSendMsg, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pushServerAskResult", js_jsmessager_JSMessager_pushServerAskResult, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendMessage", js_jsmessager_JSMessager_sendMessage, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pushNum", js_jsmessager_JSMessager_pushNum, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pushLStringShortSize", js_jsmessager_JSMessager_pushLStringShortSize, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("closeAllConnect", js_jsmessager_JSMessager_closeAllConnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_jsmessager_JSMessager_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_JSMessager_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_JSMessager_class,
        js_jsmessager_JSMessager_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_JSMessager_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "JSMessager"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<JSMessager>(cx, jsb_JSMessager_class, proto, JS::NullPtr());
}

void register_all_jsmessager(JSContext* cx, JS::HandleObject obj) {
    // Get the ns
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "jsb", &ns);

    js_register_jsmessager_JSMessager(cx, ns);
}

