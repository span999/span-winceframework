#
# this is used create the executable by py2exe.
#

from distutils.core import setup
import py2exe

__targetFile = 'target.py'

setup(console=[__targetFile])
