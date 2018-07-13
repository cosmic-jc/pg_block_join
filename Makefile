EXTENSION = similarity
MODULE_big = similarity
OBJS = main.o lev_v1.o lev_v2.o lev_v3.o lev_v4.o lev_v5.o lev_t.o jac.o jac1.o jac2.o
DATA = similarity--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/similarity
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
