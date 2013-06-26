TEMPLATE = subdirs
SUBDIRS = envelopecollide example1 example2 performance
example1.depends = envelopecollide
example2.depends = envelopecollide
performance.depends = envelopecollide
CONFIG += ordered
#SUBDIRS += build
