import os.path
import sys
from optparse import OptionParser
from subprocess import Popen, PIPE
from sbpy import host
from sbpy import log
from sbpy import code
from sbpy import file_system

def ExecuteClangFormatOnFile(clangFormat, filePath):
    process = Popen([clangFormat, "-i", filePath], stdout=PIPE, stderr=PIPE)
    (output, err) = process.communicate()
    return process.wait()

def ExecuteClangFormatOnDir(clang_format, dir_path):
    files = []
    extensions_with_dot = ["." + ext  for ext in code.CPP_FILE_EXTENSIONS]
    file_system.FindFiles( files, dirs=[dir_path], extensions=extensions_with_dot )
    for file_iter in files:
        err = ExecuteClangFormatOnFile(clang_format_path, file_iter)
        if 0 != err:
            log.LOG(log.Level.ERROR, "Failed to format {}".format(file_iter))

if __name__ == "__main__":

    optParser = OptionParser()

    optParser.add_option("-i", "--input", action="store",
                         type="string", help="Input path to format", dest="inputPath")

    optParser.add_option("-e", "--engine", action="store_true",
                         help="Format all engine sources", dest="formatEngine")

    (options, args) = optParser.parse_args()

    clang_format_path = os.path.join(host.GetUtlBinDirPath(), host.GetExeFileName("clang-format"))

    if os.path.isfile(clang_format_path):
        if options.formatEngine :
            engine_src_path = host.GetEngineSrcDirPath()
            if os.path.isdir(engine_src_path):
                ExecuteClangFormatOnDir(clang_format_path, engine_src_path)
            else:
                log.LOG(log.Level.ERROR, "Cannot find engine source directory: {}".format(engine_src_path))
        elif not (options.inputPath is None):
            if os.path.exists(options.inputPath):
                if os.path.isfile(options.inputPath):
                    err = ExecuteClangFormatOnFile(clang_format_path, options.inputPath)
                    if 0 != err:
                        log.LOG(log.Level.ERROR, "Failed to format {}".format(options.inputPath))
                elif os.path.isdir(options.inputPath):
                    ExecuteClangFormatOnDir(options.inputPath)
                else:
                    log.LOG(log.Level.ERROR, "{} is not a supported input path".format(options.inputPath))
            else:
                log.LOG(log.Level.ERROR, "{} does not exist".format(options.inputPath))
    else:
        log.LOG(log.Level.ERROR, "Cannot find clang-format executable: {}".format(clang_format_path))

    log.LOG(log.Level.LOG, "File formatting finished")

    log.Exit()
