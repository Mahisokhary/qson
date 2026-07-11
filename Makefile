.SECONDARY:
.SILENT:

BUILD_DIR := build
INSTALL_DIR :=	# Defaults to root dir


RESOLVE_SOURCES = $(shell find $1 -name "*$2" -printf "%p ")

include $(wildcard make/*.mak)

