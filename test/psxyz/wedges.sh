#!/bin/bash
#       $Id$
#
# Check wedges 

ps=wedges.ps
gmt set PROJ_ELLIPSOID Sphere

# Cartesian
gmt psbasemap -R0/6/0/3 -Jx1i -p135/35 -P -Xc -Bafg -K > $ps
echo 0.5 0.5  0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i -Gred -W2p >> $ps
echo 2.5 0.5  0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i -Gred >> $ps
echo 4.5 0.5  0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i -W1 >> $ps
echo 0.5 1.75 0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i+a -W2p >> $ps
echo 2.5 1.75 0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i+r -W2p >> $ps
echo 4.5 1.75 0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i -Gred >> $ps
echo 4.5 1.75 0 30 100 | gmt psxyz -R -J -O -K -p -Sw2i+a -W2p >> $ps
gmt psxy -R -J -O -T >> $ps