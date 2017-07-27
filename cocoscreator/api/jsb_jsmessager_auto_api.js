/**
 * @module jsmessager
 */
var jsb = jsb || {};

/**
 * @class JSMessager
 */
jsb.JSMessager = {

/**
 * @method onConnectShutdown
 * @param {unsigned short} arg0
 * @param {unsigned char} arg1
 * @return {bool}
 */
onConnectShutdown : function (
short, 
char 
)
{
    return false;
},

/**
 * @method pushLString
 * @param {int} arg0
 * @param {char} arg1
 * @param {int} arg2
 */
pushLString : function (
int, 
char, 
int 
)
{
},

/**
 * @method closeConnet
 * @param {int} arg0
 */
closeConnet : function (
int 
)
{
},

/**
 * @method houseProcMsg
 * @param {int} arg0
 * @param {unsigned short} arg1
 * @param {unsigned short} arg2
 * @param {void} arg3
 * @param {unsigned short} arg4
 * @return {bool}
 */
houseProcMsg : function (
int, 
short, 
short, 
void, 
short 
)
{
    return false;
},

/**
 * @method ask2Subserver
 * @param {int} arg0
 * @param {char} arg1
 * @param {int} arg2
 */
ask2Subserver : function (
int, 
char, 
int 
)
{
},

/**
 * @method dispatchNetStatusMsg
 * @return {bool}
 */
dispatchNetStatusMsg : function (
)
{
    return false;
},

/**
 * @method update
 * @param {float} arg0
 */
update : function (
float 
)
{
},

/**
 * @method sendToJs
 * @param {int} arg0
 * @param {unsigned short} arg1
 * @param {unsigned short} arg2
 * @param {void} arg3
 * @param {unsigned short} arg4
 * @return {bool}
 */
sendToJs : function (
int, 
short, 
short, 
void, 
short 
)
{
    return false;
},

/**
 * @method dispatchNetMsg
 */
dispatchNetMsg : function (
)
{
},

/**
 * @method endSendMsg
 * @param {int} arg0
 */
endSendMsg : function (
int 
)
{
},

/**
 * @method houseNetMsg
 * @param {int} arg0
 * @param {unsigned short} arg1
 * @param {unsigned short} arg2
 * @param {void} arg3
 * @param {unsigned short} arg4
 * @return {bool}
 */
houseNetMsg : function (
int, 
short, 
short, 
void, 
short 
)
{
    return false;
},

/**
 * @method beginSendMsg
 * @param {int} arg0
 */
beginSendMsg : function (
int 
)
{
},

/**
 * @method pushServerAskResult
 * @param {int} arg0
 * @param {int} arg1
 * @return {bool}
 */
pushServerAskResult : function (
int, 
int 
)
{
    return false;
},

/**
 * @method sendMessage
* @param {int|int} int
* @param {unsigned short|unsigned short} short
* @param {unsigned short|unsigned short} short
* @param {void} void
* @param {unsigned int} int
* @return {bool|bool}
*/
sendMessage : function(
int,
short,
short,
void,
int 
)
{
    return false;
},

/**
 * @method pushNum
 * @param {int} arg0
 * @param {void} arg1
 * @param {unsigned int} arg2
 */
pushNum : function (
int, 
void, 
int 
)
{
},

/**
 * @method pushLStringShortSize
 * @param {int} arg0
 * @param {char} arg1
 * @param {short} arg2
 */
pushLStringShortSize : function (
int, 
char, 
short 
)
{
},

/**
 * @method closeAllConnect
 */
closeAllConnect : function (
)
{
},

/**
 * @method getInstance
 * @return {JSMessager}
 */
getInstance : function (
)
{
    return JSMessager;
},

/**
 * @method JSMessager
 * @constructor
 */
JSMessager : function (
)
{
},

};
