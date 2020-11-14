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
    if os.path.isdir(dir_path):
        files = []
        extensions_with_dot = ["." + ext  for ext in code.CPP_FILE_EXTENSIONS]
        file_system.FindFiles( files, dirs=[dir_path], extensions=extensions_with_dot )
        for file_iter in files:
            err = ExecuteClangFormatOnFile(clang_format_path, file_iter)
            if 0 != err:
                log.LOG(log.Level.ERROR, "Failed to format {}".format(file_iter))
    else:
        log.LOG(log.Level.ERROR, "Cannot find engine source directory: {}".format(dir_path))

if __name__ == "__main__":

    opt_parser = OptionParser()

    opt_parser.add_option("-i", "--input", action="store",
                         type="string", help="Input path to format", dest="input_path")

    opt_parser.add_option("-e", "--engine", action="store_true",
                         help="Format all engine sources", dest="format_engine")

    opt_parser.add_option("-t", "--test", action="store_true",
                         help="Format all test sources", dest="format_test")

    opt_parser.add_option("-s", "--sample", action="store_true",
                        help="Format all sample sources", dest="format_sample")

    (options, args) = opt_parser.parse_args()

    clang_format_path = os.path.join(host.GetUtlBinDirPath(), host.GetExeFileName("clang-format"))

    if os.path.isfile(clang_format_path):
        if options.format_engine :
            ExecuteClangFormatOnDir(clang_format_path, host.GetEngineSrcDirPath())
        if options.format_test :
            ExecuteClangFormatOnDir(clang_format_path, host.GetTestSrcDirPath())
        if options.format_sample :
            ExecuteClangFormatOnDir(clang_format_path, host.GetSampleSrcDirPath())
        if (not options.input_path is None):
            if os.path.exists(options.input_path):
                if os.path.isfile(options.input_path):
                    err = ExecuteClangFormatOnFile(clang_format_path, options.input_path)
                    if 0 != err:
                        log.LOG(log.Level.ERROR, "Failed to format {}".format(options.input_path))
                elif os.path.isdir(options.input_path):
                    ExecuteClangFormatOnDir(options.input_path)
                else:
                    log.LOG(log.Level.ERROR, "{} is not a supported input path".format(options.input_path))
            else:
                log.LOG(log.Level.ERROR, "{} does not exist".format(options.input_path))
    else:
        log.LOG(log.Level.ERROR, "Cannot find clang-format executable: {}".format(clang_format_path))

    log.LOG(log.Level.LOG, "File formatting finished")

    log.Exit()
