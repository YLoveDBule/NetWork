
--------------------------------
-- @module LUAMessager
-- @parent_module luant

--------------------------------
-- 
-- @function [parent=#LUAMessager] onConnectShutdown 
-- @param self
-- @param #unsigned short sockid
-- @param #unsigned char shutReason
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] closeConnet 
-- @param self
-- @param #int sid
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] ask2Subserver 
-- @param self
-- @param #int recorder
-- @param #char ip
-- @param #int port
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] dispatchNetMsg 
-- @param self
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] endSendMsg 
-- @param self
-- @param #int socket
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] houseNetMsg 
-- @param self
-- @param #int wSocketID
-- @param #unsigned short maincmd
-- @param #unsigned short subcmd
-- @param #void pData
-- @param #unsigned short wDataSize
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] beginSendMsg 
-- @param self
-- @param #int value
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- @overload self, int, unsigned short, unsigned short, void, unsigned int         
-- @overload self, int, unsigned short, unsigned short         
-- @function [parent=#LUAMessager] sendMessage
-- @param self
-- @param #int socketId
-- @param #unsigned short wMainCmdID
-- @param #unsigned short wSubCmdID
-- @param #void pData
-- @param #unsigned int sendSize
-- @return bool#bool ret (return value: bool)

--------------------------------
-- 
-- @function [parent=#LUAMessager] closeAllConnect 
-- @param self
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] getInstance 
-- @param self
-- @return LUAMessager#LUAMessager ret (return value: luant.LUAMessager)
        
--------------------------------
-- 
-- @function [parent=#LUAMessager] LUAMessager 
-- @param self
-- @return LUAMessager#LUAMessager self (return value: luant.LUAMessager)
        
return nil
