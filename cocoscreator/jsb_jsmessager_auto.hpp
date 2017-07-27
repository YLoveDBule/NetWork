#include "base/ccConfig.h"
#ifndef __jsmessager_h__
#define __jsmessager_h__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_JSMessager_class;
extern JSObject *jsb_JSMessager_prototype;

bool js_jsmessager_JSMessager_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_jsmessager_JSMessager_finalize(JSContext *cx, JSObject *obj);
void js_register_jsmessager_JSMessager(JSContext *cx, JS::HandleObject global);
void register_all_jsmessager(JSContext* cx, JS::HandleObject obj);
bool js_jsmessager_JSMessager_onConnectShutdown(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_pushLString(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_closeConnet(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_houseProcMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_ask2Subserver(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_dispatchNetStatusMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_sendToJs(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_dispatchNetMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_endSendMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_houseNetMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_beginSendMsg(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_pushServerAskResult(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_sendMessage(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_pushNum(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_pushLStringShortSize(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_closeAllConnect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_jsmessager_JSMessager_JSMessager(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __jsmessager_h__
