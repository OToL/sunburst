import platform
import os.path

def GetDynLibExt():

    curr_system = platform.system()
    dynLibExt = "unknown"
    
    if (curr_system.lower() == "linux"):    
        dynLibExt = "so"
    elif (curr_system.lower() == "darwin"):
        dynLibExt = "dylib"

    return dynLibExt

def GetExeExt():
    
    curr_system = platform.system().lower()
    exeExt = "unknown"
    
    if (curr_system == "darwin"):    
        exeExt = ""

    return exeExt

def GetExeFileName(name):
    
    curr_system = platform.system().lower()
    exe_name = ""
    
    if (curr_system == "darwin") or (curr_system == "linux"):    
        exe_name = name

    return exe_name

def GetPlatformDirName():

    host_full_id = "unknown"
    curr_system = platform.system().lower()

    if (curr_system == "darwin"):
        host_full_id = "macos_x86_64"
    elif (curr_system == "linux"):
        host_full_id = "linux_x86_64"

    return host_full_id

def GetUtlBinDirPath():

    return os.path.join(os.path.dirname(__file__), "..", "..", "..", "bin", GetPlatformDirName())

def GetEngineSrcDirPath():

    return os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "src")
