#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime,timedelta

from pth_plotting import ReadPTHData,StackPTHData,PlotPTHData

import sys
import os


date=sys.argv[1]
filename = date+'_PTH_DATA'
txt_ext = '.txt'
img_ext = '.png'
path = os.path.join(os.getcwd(),filename+txt_ext)
PST_offset = -timedelta(hours=8)

raw_data = ReadPTHData(path,PST_offset)
data = StackPTHData(raw_data)
fig,axs = PlotPTHData(data)

fig.savefig(filename+img_ext,bbox_inches='tight')
