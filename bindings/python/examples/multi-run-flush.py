#!/usr/bin/env python
#
# Simple script showing how to perform a non-continuous, multi-run session
# while changing cyclic buffer values.

from __future__ import print_function

import sys

from pysmu import Session, Mode


if __name__ == '__main__':
    session = Session()

    if not session.devices:
        sys.exit()

    for dev in session.devices:
        dev.channels['A'].mode = Mode.SVMI
        dev.channels['B'].mode = Mode.SIMV

    for x in range(21):
        # Flush the read/write queues for all the devices in the session.
        # Without doing this the values won't change between iterations due to
        # the queues being entirely filled with values from the previous
        # constant waveform.
        session.flush()

        for dev in session.devices:
            v = x * (5 / 20.0)
            i = ((x * (4 / 20.0)) / 10.0) - 0.2
            dev.channels['A'].constant(v)
            dev.channels['B'].constant(i)

        session.run(10)

        for idx, dev in enumerate(session.devices):
            print('dev: {}: chan A voltage: {}, chan B current: {}'.format(idx + 1, v, i))
            for x in dev.get_samples(10):
                print("{: 6f} {: 6f} {: 6f} {: 6f}".format(x[0][0], x[0][1], x[1][0], x[1][1]))
