#!/bin/bash
script_dir=$(readlink -f $(dirname ${0}))

for py in python python2 python3; do
  if ( ${py} -c pass >/dev/null 2>&1 ); then
    echo "Removing MDSplus for ${py}..."
    if ${py} ${script_dir}/python_module_remove.py; then
      echo "Success"
    else
      echo "Failed"
    fi
    echo ""
  fi
done

