import os

from distutils import sysconfig
site_packages_path = sysconfig.get_python_lib()

mdsplus_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

mdsplus_pth_file = os.path.join(site_packages_path, 'mdsplus.pth')

os.unlink(mdsplus_pth_file)

