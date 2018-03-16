#!/usr/bin/env python

import sys
import re

p = re.compile('( |\$|,|\||\"|\'|;|\[|\]|!|%|/|\(|\))')

print p.sub( '_', sys.argv[1])
