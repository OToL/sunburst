import sys
from enum import Enum

class Level(Enum):
    LOG   = 1
    ERROR = 2
    FATAL_ERROR = 3

class Logger:
    
    def __init__(self):
        pass

    def logFatalError(self, msg, errorCode):
        self.m_ErrorCode = errorCode
        sys.stderr.write(msg)
        sys.exit(errorCode)

    def logError(self, msg, errorCode):
        self.m_ErrorCode = errorCode
        sys.stderr.write(msg)
        
    def logMsg(self, msg):
        sys.stdout.write(msg)

    def Log(self, level, msg, *args, **kargs):
        
        errorCode = -1
        if "code" in kargs:
           errorCode = kargs["code"] 

        {
            Level.LOG: lambda x,y: self.logMsg(x),
            Level.ERROR: lambda x, y: self.logError(x, y),
            Level.FATAL_ERROR: lambda x, y: self.logFatalError(x, y) 
        }[level](msg.format(args), errorCode)

    def ErrorCode(self):
        return self.m_ErrorCode

    def Success(self):
        return (0 == self.m_ErrorCode)
    
    m_ErrorCode = 0
    
g_DefaultLogger = Logger()

def LOG(level, msg, *args, **kargs):
    g_DefaultLogger.Log(level, msg, *args, **kargs)
    
def GetLastError():
    return g_DefaultLogger.ErrorCode()

def Exit():
    if not g_DefaultLogger.Success():
        sys.exit(g_DefaultLogger.ErrorCode())

