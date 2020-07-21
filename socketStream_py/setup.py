"""
The build/compilations setup

>> pip install -r requirements.txt
>> python setup.py install
"""
import pip
import logging
import pkg_resources
try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup


def _parse_requirements(file_path):
    pip_ver = pkg_resources.get_distribution('pip').version
    pip_version = list(map(int, pip_ver.split('.')[:2]))
    if pip_version >= [6, 0]:
        raw = pip.req.parse_requirements(file_path,
                                         session=pip.download.PipSession())
    else:
        raw = pip.req.parse_requirements(file_path)
    return [str(i.req) for i in raw]


# parse_requirements() returns generator of pip.req.InstallRequirement objects
try:
    install_reqs = _parse_requirements("socketStream_py/requirements.txt")
except Exception:
    logging.warning('Fail load requirements file, so using default ones.')
    install_reqs = []

setup(
    name='socketStream_py',
    version='1.0',
    url='https://github.com/yias/socketStream',
    author='Iason Batzianoulis',
    author_email='iasonbatz@gmail.com',
    license='GNU GPL v3',
    description='socketStream python module for TCP/IP communication',
    packages=["socketStream_py"],
    install_requires=install_reqs,
    include_package_data=True,
    python_requires='>=3.7',
    long_description="""This is an implementation of socketStream C++ package for TCP/IP 
    communication. The functionality is the same as the C++ package""",
    classifiers=[
        "Development Status :: 1 - Stable",
        "Environment :: Console",
        "Intended Audience :: Developers",
        "Intended Audience :: Information Technology",
        "Intended Audience :: Education",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU GPL v3 License",
        "Natural Language :: English",
        "Operating System :: OS Independent",
        "Topic :: Scientific/Engineering :: Computer Science",
        "Topic :: Scientific/Engineering :: Socket Communication",
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
    ],
    keywords="socket TCP python socketStream",
)