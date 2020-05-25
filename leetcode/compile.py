import os
import sys
# from .utils import  run_cmd, find_exe, \
#     try_find_exe, cc_path, cache_path, remove_flags, run_cmds, make_cache_dir, LOG
# from . import utils

from .utils import *
from . import utils as utils

import ctypes
from ctypes.util import find_library
import platform
import re

from . import pyjt_compiler
from .pyjt_compiler import compile as pyjt_compile

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
    return os.path.abspath(os.path.dirname(__file__))
    # return os.path.dirname(__file__)


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

cmd: python3-config --ldflags
'''
link_flags = f" -lstdc++ -ldl -shared "
# link_flags += f" -L/Users/wuyongyu/anaconda3/lib/python3.7/config-3.7m-darwin -lpython3.7m "

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
cc_flags += " -Wall -Werror -Wunused-function -Wno-unknown-pragmas -std=c++14 -fPIC -march=native "
# cc_flags += "  "

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
cc_flags += f" -I{project_path}/src "


opt_flags = ""
kernel_opt_flags = os.environ.get("kernel_flags", "") + opt_flags + " -fopenmp "
if ' -O' not in cc_flags:
    opt_flags += " -O2 "
    kernel_opt_flags += " -Ofast "


# compile first enter
def check_cache_compile():
    files_ = [
        "src/mflag/mflag.cpp",
        "src/flags.cpp",
        "src/log/tracer.cpp",
        "src/log/log.cpp",
        "src/file/file.cpp",
        "src/cache_compile.cpp",
        "src/cache_utils.cpp", # 通过Pybind11绑定 cache_compile 函数
    ]
    # files = [os.path.join(project_path, "src", k) for k in files_]
    files = files_
    global utils_core_files
    utils_core_files = files
    # here cc_path is "g++"
    recompile = compile(cc_path, cc_flags+f" {opt_flags} ", files, 'utils_core'+extension_suffix, True)
    print("recompile:", recompile)
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
            LOG.i("cache_compile:", cmd)
            return utils.cc.cache_compile(cmd, cache_path, project_path)
        else:
            LOG.i("no cache_compile ,so begin compile")
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
    # compile cache cpps
    if len(inputs) == 1 or combind_build:
        # On Mac OS: the build command is almost the same but it also requires passing
        # the -undefined dynamic_lookup flag so as to ignore missing symbols
        # when building the module more is : https://pybind11.readthedocs.io/en/stable/compiling.html
        if platform.platform().startswith("Darwin"):
            flags += " -undefined dynamic_lookup "
        LOG.i("begin compile cache_compile")
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
    print("compile binary begin!")
    print(cmds)
    run_cmds(cmds, cache_path, project_path)
    print("compile binary end!")
    # LOG.i(obj_files)
    link_temp =link + f" -L/Users/wuyongyu/anaconda3/lib/python3.7/config-3.7m-darwin -lpython3.7m "
    cmd = f"{compiler} {' '.join(obj_files)} {flags} {lto_flags} {link_temp} -o {output}"
    print(cmd)
    return do_compile(cmd)

# 如果已经加载jit_utils_core不用二次加载
# with utils.import_scope(import_flags):
#     print("first try import uitls core")
utils.try_import_utils_core()


make_cache_dir(cache_path)
make_cache_dir(os.path.join(cache_path, "jit"))
make_cache_dir(os.path.join(cache_path, "obj_files"))
make_cache_dir(os.path.join(cache_path, "gen"))

LOG.i("cache_compile_begin")
# compile cache
check_cache_compile()

print("check cache_compile end")


# 解析 JIT flags

def gen_jit_flags():
    all_src = run_cmd('find -L src | grep "cpp$"', project_path).splitlines()
    jit_declares = []
    re_def = re.compile("DEFINE_FLAG(_WITH_SETTER)?\\((.*?)\\);", re.DOTALL)

    flags_defs = []
    visit = {}

    for src_name in all_src:
        LOG.i(src_name)
        src_name = os.path.join(project_path, src_name)
        with open(src_name) as f:
            src = f.read()
        defs = re_def.findall(src)
        for _, args in defs:
            args = args.split(",")
            type = args[0].strip()
            name = args[1].strip()
            # LOG.i(type, " name:", name)
            if not has_cuda and "cuda" in name and name!="use_cuda":
                continue
            default = args[2].strip()
            doc = ",".join(args[3:])
            doc = eval(f"({doc})")
            LOG.vv(f"Find define {name} from {src_name}")
            if name in visit:
                continue
            visit[name] = 1
            jit_declares.append(f"DECLARE_FLAG({type}, {name});")
            # flags_defs.append(f"""
            #     /* {name}(type:{type}, default:{default}): {doc} */
            #     // @pyjt(__get__{name})
            #     {type} _get_FLAGS_{name}() {{ return FLAGS_{name}; }}
            #     // @pyjt(__set__{name})
            #     void _set_FLAGS_{name}({type} v) {{ set_FLAGS_{name}(v); }}
            #     {f'''// @pyjt(__set__{name})
            #     void _set_FLAGS_{name}(bool v) {{ set_FLAGS_{name}(v); }}
            #     ''' if type=="int" else ""}
            # """)
    jit_declares = "\n    ".join(jit_declares)
    jit_src = f"""
    #include "mflag/mflag.h"
    #include "flags.h"

    namespace leetcode {{
    
    {jit_declares}

    // @pyjt(flags)
    struct _Flags {{
        // @pyjt(__init__)
        _Flags() {{}}
        {"".join(flags_defs)}
    }};

    }} // leetcode
    """
    # LOG.vvvv(jit_src)
    with open(os.path.join(cache_path, "gen", "jit_flags.h"), 'w') as f:
        f.write(jit_src)

# 生成 gen/jit_flags.h
# gen_jit_flags() # debug

cc_flags += f' -I{cache_path} '

str1 = run_cmd("pwd")
# LOG.i(str1)
# gen pyjt
LOG.i("pyjt_compile begin..")
# pyjt_compile(cache_path, project_path)  # debug

files2 = run_cmd(f'find "{os.path.join(cache_path, "gen")}" | grep "cpp$"').splitlines()
files4 = run_cmd('find -L src | grep "cpp$"', project_path).splitlines()

files = files2 + files4

for v in utils_core_files:
    if v == "src/mflag/mflag.cpp" or v == "src/log/log.cpp" or v == "src/flags.cpp":
        continue
    files.remove(v)

# manual Link omp using flags(os.RTLD_NOW | os.RTLD_GLOBAL)
# if cc_type=="icc":
#     os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"
libname = {"clang":"omp", "icc":"iomp5", "g++":"gomp"}[cc_type]
libname = ctypes.util.find_library(libname)
assert libname is not None, "openmp library not found"
ctypes.CDLL(libname, os.RTLD_NOW | os.RTLD_GLOBAL)


print("compile leetcode core  begin..")
# print(files)
compile(cc_path, cc_flags+opt_flags, files, 'leetcode_core'+extension_suffix)
