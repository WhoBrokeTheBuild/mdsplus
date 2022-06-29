import os

from distutils import sysconfig
site_packages_path = sysconfig.get_python_lib()

mdsplus_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
mdsplus_python_dir = os.path.join(mdsplus_dir, 'python')

mdsplus_pth_file = os.path.join(site_packages_path, 'mdsplus.pth')

with open(mdsplus_pth_file, 'wt') as f:
    f.write(mdsplus_python_dir)

