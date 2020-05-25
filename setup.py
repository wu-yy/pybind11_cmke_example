import os
from setuptools import setup, find_packages
path = os.path.dirname(__file__)
setup(
    name='leetcode',
    version='0.1.0',
    packages=["leetcode"],
    license='GPL-2',
    python_requires='>=3.7',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
        'Natural Language :: English',
        'Programming Language :: Python :: 3 :: Only',
        'Programming Language :: Python :: Implementation :: CPython',
    ],
    package_dir={'':path},
    package_data={'': ['*', '*/*', '*/*/*','*/*/*/*','*/*/*/*/*','*/*/*/*/*/*']},
    install_requires=[
        "pybind11",
        "numpy",
        "tqdm",
        "pillow",
        "astunparse",
    ],
)