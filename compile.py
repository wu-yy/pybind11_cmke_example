import os
import sys
import subprocess as sp
from utils import  run_cmd, find_exe, \
    try_find_exe, cc_path, cache_path, remove_flags, run_cmds, make_cache_dir
from ctypes import cdll
import utils
import ctypes
from ctypes import cdll
from ctypes.util import find_library
import platform

print(platform.platform() )
'''
python path
'''
python_path = sys.executable

py3_config_path = sys.executable+"-config"
assert os.path.isfile(python_path)
if not os.path.isfile(py3_config_path) :
    py3_config_path = sys.executable + '3-config'

# extension_suffix : .cpython-37m-darwin.so
extension_suffix = run_cmd(py3_config_path+" --extension-suffix")
'''
py3_config_path : /Users/wuyongyu/anaconda3/bin/python3-config
'''

assert os.path.isfile(py3_config_path)

def find_project_path():
    return os.path.dirname(__file__)


is_debug = 0
def check_debug_flags():
    global is_debug
    is_debug = 0
    if os.environ.get("debug")=="1":
        is_debug = 1
        global cc_flags
        cc_flags += " -g -DNODE_MEMCHECK "

cc_type = "g++"
'''
ctypes
'''
dlopen_flags = os.RTLD_NOW | os.RTLD_GLOBAL #| os.RTLD_DEEPBIND
import_flags = os.RTLD_NOW | os.RTLD_GLOBAL #| os.RTLD_DEEPBIND

'''
project path
'''
project_path = find_project_path()

'''
core_link_flags = ""
'''
core_link_flags = ""


'''
link 
'''
link_flags = f" -lstdc++ -ldl -shared "
lto_flags = ""
if os.environ.get("enable_lto") == "1":
    if cc_type == "icc":
        lto_flags = " -flto -ipo -ipo-c "
    elif cc_type == "g++":
        lto_flags = " -flto -fuse-linker-plugin "
    else:
        lto_flags = " -flto "

'''
cuda
'''
def get_full_path_of_executable(name):
    full_path = os.path.abspath(name)
    while os.path.islink(full_path):
        full_path = os.path.realpath(full_path)
    if os.path.isfile(full_path) and os.access(full_path, os.X_OK):
        return full_path
    return get_full_path_of_executable(find_exe(name))

def check_cuda():
    if nvcc_path == "":
        return
    global cc_flags, has_cuda, core_link_flags, cuda_dir, cuda_lib, cuda_include
    cuda_dir = os.path.dirname(get_full_path_of_executable(nvcc_path))
    assert cuda_dir.endswith("bin") and "cuda" in cuda_dir, f"Wrong cuda_dir: {cuda_dir}"
    cuda_include = os.path.abspath(os.path.join(cuda_dir, "..", "include"))
    cuda_lib = os.path.abspath(os.path.join(cuda_dir, "..", "lib64"))
    #cuda_include2 = os.path.join(project_path, "extern","cuda","inc")
    cuda_include2 = ""
    cc_flags += f" -DHAS_CUDA -I'{cuda_include}' -I'{cuda_include2}' "
    core_link_flags += f" -lcudart -L'{cuda_lib}' "
    # ctypes.CDLL(cuda_lib+"/libcudart.so", import_flags)
    ctypes.CDLL(cuda_lib+"/libcudart.so", dlopen_flags)
    has_cuda = 1

def env_or_try_find(name, bname):
    if name in os.environ:
        return os.environ[name]
    return try_find_exe(bname)

has_cuda = 0
nvcc_path = env_or_try_find('nvcc_path', '/usr/local/cuda/bin/nvcc')
check_cuda()
nvcc_flags = os.environ.get("nvcc_flags", "")
if has_cuda:
    nvcc_flags += cc_flags + link_flags
    def convert_nvcc_flags(nvcc_flags):
        # nvcc don't support -Wall option
        nvcc_flags = nvcc_flags.replace("-Wall", "")
        nvcc_flags = nvcc_flags.replace("-Wno-unknown-pragmas", "")
        nvcc_flags = nvcc_flags.replace("-fopenmp", "")
        nvcc_flags = nvcc_flags.replace("-march", "-Xcompiler -march")
        nvcc_flags = nvcc_flags.replace("-Werror", "")
        nvcc_flags = nvcc_flags.replace("-fPIC", "-Xcompiler -fPIC")
        nvcc_flags = nvcc_flags.replace("-fdiagnostics", "-Xcompiler -fdiagnostics")
        nvcc_flags += f" -x cu --cudart=shared -ccbin='{cc_path}' --use_fast_math "
        # nvcc warning is noise
        nvcc_flags += " -w "
        nvcc_flags += f" -I'{os.path.join(project_path, 'extern/cuda/inc')}' "
        if os.environ.get("cuda_debug", "0") == "1":
            nvcc_flags += " -G "
        return nvcc_flags
    nvcc_flags = convert_nvcc_flags(nvcc_flags)

'''
cc_flags
'''
cc_flags = " " + os.environ.get("cc_flags", "")
cc_flags += " -Wall -Werror -Wno-unknown-pragmas -std=c++14 -fPIC -march=native "
# On Mac OS: the build command is almost the same but it also requires passing
# the -undefined dynamic_lookup flag so as to ignore missing symbols
# when building the module more is : https://pybind11.readthedocs.io/en/stable/compiling.html
if platform.platform().startswith("Darwin"):
    cc_flags += " -undefined dynamic_lookup "

cc_flags += " -fdiagnostics-color=always "
check_debug_flags()
# build cache_compile
#
# cc_flags += f" "
# get the pybind11 include path

#pybind_include:
#-I/Users/wuyongyu/anaconda3/include/python3.7m
#-I/Users/wuyongyu/anaconda3/lib/python3.7/site-packages/pybind11/include
pybind_include = run_cmd(python_path+" -m pybind11 --includes")
cc_flags += pybind_include
cc_flags += f" -I{project_path} "

opt_flags = ""
kernel_opt_flags = os.environ.get("kernel_flags", "") + opt_flags + " -fopenmp "
if ' -O' not in cc_flags:
    opt_flags += " -O2 "
    kernel_opt_flags += " -Ofast "


# compile first enter
def check_cache_compile():
    files = [
        "file/file.cpp",
        "cache_compile.cpp",
        "cache_utils.cpp", # 通过Pybind11绑定 cache_compile 函数
    ]
    global utils_core_files
    utils_core_files = files
    # here cc_path is "g++"
    recompile = compile(cc_path, cc_flags+f" {opt_flags} ", files, 'utils_core'+extension_suffix, True)

    return

    if recompile and utils.cc:
        print("jit_utils updated, please restart python. stop")
        sys.exit(0)
    if not utils.cc:
        with utils.import_scope(import_flags):
            utils.try_import_utils_core()
        assert utils.cc
        # recompile, generate cache key
        compile(cc_path, cc_flags+f" {opt_flags} ", files, 'utils_core'+extension_suffix, True)


def compile(compiler, flags, inputs, output, combind_build=False):
    def do_compile(cmd):
        if utils.cc:  # NOTE(wuyongyu)
            print("compile: tea_utils.cc is not none!")
            return utils.cc.cache_compile(cmd, cache_path, project_path)
        else:
            #print(f"do_compile:{cmd}")
            run_cmd(cmd)
            return True
    link = link_flags
    # if output is core, add core_link_flags
    if output.startswith("leetcode_core"):
        link = link + core_link_flags
    output = os.path.join(cache_path, output)

    # don't recompile object file in inputs
    obj_files = []
    new_inputs = []
    for name in inputs:
        if name.endswith(".o"):
            obj_files.append(name)
        else:
            new_inputs.append(os.path.join(project_path, name))
            obj_files.append(os.path.join(
                cache_path, "obj_files", os.path.basename(name)+".o"))
    inputs = new_inputs
    if len(inputs) == 1 or combind_build:
        cmd = f"{compiler} {' '.join(inputs)} {flags} {link} -o {output}"
        #print("compile cmd:", cmd)
        return do_compile(cmd)
    # split compile object file and link
    # remove -l -L -Wl flags when compile object files
    oflags = remove_flags(flags, ['-l', '-L', '-Wl,'])

    cmds = []
    for input, obj_file in zip(inputs, obj_files):
        cc = compiler
        nflags = oflags
        if has_cuda and input.endswith(".cu"):
            nflags = convert_nvcc_flags(oflags)
            cc = nvcc_path
        cmd = f"{cc} {input} {nflags} -c {lto_flags} -o {obj_file}"
        cmds.append(cmd)
    print("line 216 cmds:", cmds)
    run_cmds(cmds, cache_path, project_path)

    cmd = f"{compiler} {' '.join(obj_files)} {flags} {lto_flags} {link} -o {output}"
    # LOG.v(cmd)
    return do_compile(cmd)

# 如果已经加载jit_utils_core不用二次加载
with utils.import_scope(import_flags):
    print("first try import uitls core")
    utils.try_import_utils_core()

make_cache_dir(cache_path)
make_cache_dir(os.path.join(cache_path, "jit"))
make_cache_dir(os.path.join(cache_path, "obj_files"))
make_cache_dir(os.path.join(cache_path, "gen"))

# compile cache
check_cache_compile()