编译成功后，会在cmake-build-debug 相同目录下生成leetcode.pyd文件，这个就是python可以直接导入的库，运行：


'''
import leetcode
leetcode.add(1,3)

'''



python setup.py build_ext --inplace