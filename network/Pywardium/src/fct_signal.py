#! /usr/bin/python
#---------------------------------------------

from src import fct_param

import signal
import time


def handler(signum, frame):
    fct_param.run = False

signal.signal(signal.SIGINT, handler)
