import subprocess as sp
from multiprocessing import Pool
import os
import re
import sys
import inspect
import datetime
import contextlib
import threading
import time
from ctypes import cdll

def remove_flags(flags, rm_flags):
    flags = flags.split(" ")
    output = []
    for s in flags:
        for rm in rm_flags:
            if s.startswith(rm):
                break
        else:
            output.append(s)
    return " ".join(output)

@contextlib.contextmanager
def import_scope(flags):
    prev = sys.getdlopenflags()
    sys.setdlopenflags(flags)
    yield
    sys.setdlopenflags(prev)

def run_cmd(cmd, cwd = None, err_msg= None, print_err = True):
    if cwd:
        cmd =  f"cd {cwd} && {cmd}"

    r = sp.run(cmd, shell=True, stdout=sp.PIPE, stderr=sp.STDOUT)
    s = r.stdout.decode('utf8')
    if r.returncode != 0:
        if print_err:
            sys.stderr.write(s)
        if err_msg is None:
            err_msg = f"Run cmd failed: {cmd}"
        if not print_err:
            err_msg += "\n"+s
        raise Exception(err_msg)
    if len(s) and s[-1] == '\n': s = s[:-1]
    return s


def find_exe(name, check_version=True):
    output = run_cmd(f'which {name}', err_msg=f'{name} not found')
    if check_version:
        version = run_cmd(output+" --version")
        v = re.findall("[0-9]+\\.[0-9]+\\.[0-9]+", version)
        if len(v) == 0:
            v = re.findall("[0-9]+\\.[0-9]+", version)
        assert len(v) != 0, f"Can not find version number from: {version}"
        version = "("+v[-1]+")"
    else:
        version = ""
    return output

def env_or_find(name, bname):
    if name in os.environ:
        return os.environ[name]
    return find_exe(bname)

def try_find_exe(*args):
    try:
        return find_exe(*args)
    except:
        #LOG.v(f"{args[0]} not found.")
        return ""

def get_cc_type(cc_path):
    bname = os.path.basename(cc_path)
    if "clang" in bname: return "clang"
    if "icc" in bname or "icpc" in bname: return "icc"
    if "g++" in bname: return "g++"
    print(f"Unknown cc type: {bname}")

def find_cache_path() :
    global project_name
    from pathlib import Path
    # path = str(Path.home())
    path = os.path.dirname(__file__)
    dirs = ["cache", project_name, os.path.basename(cc_path)]
    if os.environ.get("debug")=="1":
        dirs[-1] += "_debug"
    cache_name = "default"
    try:
        if "cache_name" in os.environ:
            cache_name = os.environ["cache_name"]
        else:
            # try to get branch name from git
            bs = run_cmd("git branch", os.path.dirname(__file__)).splitlines()
            for b in bs:
                if b.startswith("* "): break
            cache_name = b[2:]
        for c in " (){}": cache_name = cache_name.replace(c, "_")
    except:
        pass

    for name in cache_name.split("/"):
        dirs.insert(-1, name)
        #LOG.v("cache_name_split->:" + name)

    for d in dirs:
        path = os.path.join(path, d)
        if not os.path.isdir(path):
            os.mkdir(path)
        assert os.path.isdir(path)
    if path not in sys.path:
        sys.path.append(path)
    return path

def try_import_jit_utils_core(silent=None):
    global cc
    if cc:
        print("global jit_utils_core cc is alread import!")
        return
    if not (silent is None):
        prev = os.environ.get("log_silent", "0")
        os.environ["log_silent"] = str(int(silent))
    try:
        # if is in notebook, must log sync, and we redirect the log
        if is_in_ipynb: os.environ["log_sync"] = "1"
        import jit_utils_core as cc
        print("first import jit_utils_core cc!")
        if is_in_ipynb:
            global redirector
            redirector = cc.ostream_redirect(stdout=True, stderr=True)
            redirector.__enter__()
    except Exception as _:
        pass
    if not (silent is None):
        os.environ["log_silent"] = prev

def do_compile(args):
    cmd, cache_path, tea_path = args
    try_import_jit_utils_core(True)
    if cc:
        return cc.cache_compile(cmd, cache_path, tea_path)
    else:
        run_cmd(cmd)
        return True

def run_cmds(cmds, cache_path, teaflow_path):
    cmds = [ [cmd, cache_path, teaflow_path] for cmd in cmds ]
    with Pool(8) as p:
        p.map(do_compile, cmds)

def make_cache_dir(cache_path):
    if not os.path.isdir(cache_path):
        print(f"Create cache dir: {cache_path}")
        os.mkdir(cache_path)


project_name = "leetcode"
is_in_ipynb = False
cc = None
cc_path = env_or_find('cc_path', 'g++')
os.environ["cc_path"] = cc_path
cc_type = get_cc_type(cc_path)
cache_path = find_cache_path()
