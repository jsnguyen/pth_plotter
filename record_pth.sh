#!/bin/bash

echo "Collecting pressure, temperature, humidity data. Press [CTRL+C] to stop."

PTH_SERVER_IP="10.0.0.44"
INTERVAL="10s"

COUNTER=0
PREV_DATE=""
while true; do
  CURR_DATE="$(date +"%Y_%m_%d")"
  OUTFILE="${CURR_DATE}_PTH_DATA.txt"

  UNIX_TIME="$(date +%s%3N)"
  PTH_DATA="$(curl -s ${PTH_SERVER_IP})"
  echo "${UNIX_TIME} : ${PTH_DATA}" >> ${OUTFILE}

  echo "Taking data point number: ${COUNTER}"
  COUNTER=$((COUNTER + 1))

  if [ "${CURR_DATE}" != "${PREV_DATE}" ] && [ "${PREV_DATE}" != "" ]; then
    ./plot_pth.py ${PREV_DATE}
  fi

  PREV_DATE=${CURR_DATE}

  sleep ${INTERVAL}
done
