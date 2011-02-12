#
# Makefile for libXawPlus, styled for LINUX.
# An extended Xaw library with 3 D styled widgets, XPM support for all
# widgets and two new widgets: The IconList and the DrawingArea widget.
# This is a X11R6.6 based version but it also works fine for other X11R6
# releases.
#
# Copyright (c) Roland Krause 2002, roland_krause@freenet.de
#
# This module is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

prefix	= /usr/local

LIB_PATH =	-L/usr/X11/lib			# Libraries for X
INC_PATH =	-I. -I/usr/X11/include 		# Header files for X
INST_INCS=	${prefix}/include/X11/XawPlus
INST_LIBS=	${prefix}/lib

XAW=		./X11/XawPlus

CC =		gcc
FLAGS =		-fPIC -DHAS_WCHAR_H -O2

OBJS=		XawInit.o Vendor.o XawI18n.o UTF8.o StrToPmap.o LocPixmap.o \
		Box.o Paned.o Viewport.o Form.o Porthole.o \
		XawIm.o TextSrc.o TextSink.o TextAction.o TextPop.o TextTr.o Text.o \
		MultiSrc.o MultiSink.o AsciiSrc.o AsciiSink.o AsciiText.o \
		Sme.o SmeBSB.o SmeLine.o SimpleMenu.o Simple.o Label.o \
		MenuButton.o Toggle.o Panner.o Command.o Repeater.o \
		Grip.o Scrollbar.o List.o \
 		Tree.o DrawingArea.o StripChart.o Dialog.o

all:		lib
lib:		libXawPlus.a libXawPlus.so.3.1 libXaw.so.6.6 libXaw.so.7.0

clean:
		rm -f *.o *.a *.so*
distclean:	clean

# -- Build the libraries

libXawPlus.a:	$(OBJS)
		ranlib libXawPlus.a

libXawPlus.so.3.1: $(OBJS)
		$(CC) -shared -Wl,-soname,libXawPlus.so.3 \
		-o libXawPlus.so.3.1 $(OBJS)

libXaw.so.6.6: $(OBJS)
		$(CC) -shared -Wl,-soname,libXaw.so.6 \
		-o libXaw.so.6.6 $(OBJS)

libXaw.so.7.0: $(OBJS)
		$(CC) -shared -Wl,-soname,libXaw.so.7 \
		-o libXaw.so.7.0 $(OBJS)

# --- This only works as superuser root

install:
		rm -rf $(INST_INCS)
		mkdir -p $(INST_INCS)
		cp ./X11/XawPlus/*.h $(INST_INCS)
		cp libXawPlus.a $(INST_LIBS)
		rm -f $(INST_LIBS)/libXawPlus.so.3 $(INST_LIBS)/libXawPlus.so
		cp libXawPlus.so.3.1 $(INST_LIBS)
		ldconfig
		ln -s $(INST_LIBS)/libXawPlus.so.3.1 $(INST_LIBS)/libXawPlus.so

installxaw:
		rm -f	$(INST_LIBS)/libXaw.so.6 \
			$(INST_LIBS)/libXaw.so.7 \
			$(INST_LIBS)/libXaw.so
		cp libXaw.so.6.6 $(INST_LIBS)
		cp libXaw.so.7.0 $(INST_LIBS)
		ldconfig
		ln -s $(INST_LIBS)/libXaw.so.6.6 $(INST_LIBS)/libXaw.so

# --- Common

XawInit.o:	XawInit.c $(XAW)/XawInit.h
		$(CC) $(FLAGS) $(INC_PATH) -c XawInit.c
		ar r libXawPlus.a XawInit.o

Vendor.o:	Vendor.c $(XAW)/VendorEP.h $(XAW)/XawImP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Vendor.c
		ar r libXawPlus.a Vendor.o

# --- Internationalization (I18n)

XawI18n.o:	XawI18n.c XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c XawI18n.c
		ar r libXawPlus.a XawI18n.o

UTF8.o:		UTF8.c UTF8.h
		$(CC) $(FLAGS) $(INC_PATH) -c UTF8.c
		ar r libXawPlus.a UTF8.o

# --- XPM support

LocPixmap.o:	LocPixmap.c
		$(CC) $(FLAGS) $(INC_PATH) -c LocPixmap.c
		ar r libXawPlus.a LocPixmap.o

StrToPmap.o:	StrToPmap.c
		$(CC) $(FLAGS) $(INC_PATH) -c StrToPmap.c
		ar r libXawPlus.a StrToPmap.o

# --- Manager widgets

Box.o:		Box.c $(XAW)/XawInit.h $(XAW)/Box.h $(XAW)/BoxP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Box.c
		ar r libXawPlus.a Box.o

Paned.o:	Paned.c $(XAW)/XawInit.h $(XAW)/Grip.h \
		$(XAW)/Paned.h $(XAW)/PanedP.h $(XAW)/XawImP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Paned.c
		ar r libXawPlus.a Paned.o

Viewport.o:	Viewport.c $(XAW)/XawInit.h $(XAW)/Scrollbar.h \
		$(XAW)/Form.h $(XAW)/FormP.h $(XAW)/Viewport.h $(XAW)/ViewportP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Viewport.c
		ar r libXawPlus.a Viewport.o

Form.o:		Form.c $(XAW)/XawInit.h $(XAW)/Form.h $(XAW)/FormP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Form.c
		ar r libXawPlus.a Form.o

Porthole.o:	Porthole.c $(XAW)/XawInit.h $(XAW)/Porthole.h $(XAW)/PortholeP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Porthole.c
		ar r libXawPlus.a Porthole.o

# --- Text widget

XawIm.o:	XawIm.c $(XAW)/TextP.h $(XAW)/MultiSrc.h $(XAW)/MultiSinkP.h \
		$(XAW)/MultiSink.h $(XAW)/TextSinkP.h $(XAW)/TextSink.h \
		$(XAW)/XawImP.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c XawIm.c
		ar r libXawPlus.a XawIm.o

TextSrc.o:	TextSrc.c $(XAW)/XawInit.h $(XAW)/TextSrc.h $(XAW)/TextSrcP.h \
		$(XAW)/TextP.h $(XAW)/Text.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c TextSrc.c
		ar r libXawPlus.a TextSrc.o

TextSink.o:	TextSink.c $(XAW)/XawInit.h $(XAW)/TextP.h $(XAW)/Text.h \
		$(XAW)/TextSink.h $(XAW)/TextSinkP.h
		$(CC) $(FLAGS) $(INC_PATH) -c TextSink.c
		ar r libXawPlus.a TextSink.o

TextAction.o:	TextAction.c $(XAW)/TextP.h $(XAW)/Text.h $(XAW)/MultiSrc.h \
		$(XAW)/MultiSrcP.h $(XAW)/XawImP.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c TextAction.c
		ar r libXawPlus.a TextAction.o

TextTr.o:	TextTr.c
		$(CC) $(FLAGS) $(INC_PATH) -c TextTr.c
		ar r libXawPlus.a TextTr.o

Text.o:		Text.c $(XAW)/XawInit.h $(XAW)/Simple.h $(XAW)/SimpleP.h \
		$(XAW)/Scrollbar.h $(XAW)/TextP.h $(XAW)/Text.h $(XAW)/MultiSink.h \
		$(XAW)/MultiSinkP.h $(XAW)/XawImP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Text.c
		ar r libXawPlus.a Text.o

# --- AsciiText widget

AsciiSrc.o:	AsciiSrc.c $(XAW)/XawInit.h \
		$(XAW)/TextSrc.h $(XAW)/TextSrcP.h $(XAW)/AsciiSrc.h $(XAW)/AsciiSrcP.h \
		$(XAW)/MultiSrc.h $(XAW)/MultiSrcP.h
		$(CC) $(FLAGS) $(INC_PATH) -c AsciiSrc.c
		ar r libXawPlus.a AsciiSrc.o

AsciiSink.o:	AsciiSink.c $(XAW)/XawInit.h $(XAW)/AsciiSrcP.h $(XAW)/AsciiSrc.h \
		$(XAW)/TextP.h $(XAW)/Text.h $(XAW)/TextSink.h $(XAW)/TextSinkP.h \
		$(XAW)/AsciiSink.h $(XAW)/AsciiSinkP.h
		$(CC) $(FLAGS) $(INC_PATH) -c AsciiSink.c
		ar r libXawPlus.a AsciiSink.o

AsciiText.o:	AsciiText.c $(XAW)/XawInit.h $(XAW)/XawImP.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/TextP.h $(XAW)/Text.h \
		$(XAW)/AsciiSrc.h $(XAW)/AsciiSink.h $(XAW)/AsciiText.h $(XAW)/AsciiTextP.h
		$(CC) $(FLAGS) $(INC_PATH) -c AsciiText.c
		ar r libXawPlus.a AsciiText.o

MultiSrc.o:	MultiSrc.c $(XAW)/XawInit.h $(XAW)/MultiSrc.h $(XAW)/MultiSrcP.h \
		$(XAW)/XawImP.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c MultiSrc.c
		ar r libXawPlus.a MultiSrc.o

MultiSink.o:	MultiSink.c $(XAW)/XawInit.h $(XAW)/TextP.h $(XAW)/Text.h \
		$(XAW)/MultiSrc.h $(XAW)/MultiSrcP.h \
		$(XAW)/MultiSink.h $(XAW)/MultiSinkP.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c MultiSink.c
		ar r libXawPlus.a MultiSink.o

TextPop.o:	TextPop.c $(XAW)/TextP.h $(XAW)/Text.h $(XAW)/AsciiText.h \
		$(XAW)/Command.h $(XAW)/Form.h $(XAW)/Toggle.h XawI18n.h
		$(CC) $(FLAGS) $(INC_PATH) -c TextPop.c
		ar r libXawPlus.a TextPop.o

# --- Simple menue widget

Sme.o:		Sme.c $(XAW)/XawInit.h $(XAW)/Sme.h $(XAW)/SmeP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Sme.c
		ar r libXawPlus.a Sme.o

SmeBSB.o:	SmeBSB.c $(XAW)/StrToPmap.h $(XAW)/XawInit.h $(XAW)/Cardinals.h $(XAW)/SimpleMenu.h \
		$(XAW)/SmeBSB.h $(XAW)/SmeBSBP.h
		$(CC) $(FLAGS) $(INC_PATH) -c SmeBSB.c
		ar r libXawPlus.a SmeBSB.o

SmeLine.o:	SmeLine.c $(XAW)/XawInit.h $(XAW)/Sme.h $(XAW)/SmeP.h \
		$(XAW)/SmeLine.h $(XAW)/SmeLineP.h
		$(CC) $(FLAGS) $(INC_PATH) -c SmeLine.c
		ar r libXawPlus.a SmeLine.o

SimpleMenu.o:	SimpleMenu.c $(XAW)/XawInit.h UTF8.h $(XAW)/SmeBSB.h \
		$(XAW)/SimpleMenu.h $(XAW)/SimpleMenP.h
		$(CC) $(FLAGS) $(INC_PATH) -c SimpleMenu.c
		ar r libXawPlus.a SimpleMenu.o

# --- Toggles, buttons, lists ...

Simple.o:	Simple.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Simple.c
		ar r libXawPlus.a Simple.o

Label.o:	Label.c $(XAW)/StrToPmap.h $(XAW)/XawInit.h UTF8.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Label.h $(XAW)/LabelP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Label.c
		ar r libXawPlus.a Label.o

Command.o:	Command.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Label.h $(XAW)/LabelP.h \
		$(XAW)/Command.h $(XAW)/CommandP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Command.c
		ar r libXawPlus.a Command.o

MenuButton.o:	MenuButton.c  $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Label.h $(XAW)/LabelP.h \
		$(XAW)/Command.h $(XAW)/CommandP.h $(XAW)/MenuButton.h $(XAW)/MenuButtoP.h
		$(CC) $(FLAGS) $(INC_PATH) -c MenuButton.c
		ar r libXawPlus.a MenuButton.o

Toggle.o:	Toggle.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Label.h $(XAW)/LabelP.h \
		$(XAW)/Command.h $(XAW)/CommandP.h $(XAW)/Toggle.h $(XAW)/ToggleP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Toggle.c
		ar r libXawPlus.a Toggle.o

Repeater.o:	Repeater.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Label.h $(XAW)/LabelP.h \
		$(XAW)/Command.h $(XAW)/CommandP.h $(XAW)/Repeater.h $(XAW)/RepeaterP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Repeater.c
		ar r libXawPlus.a Repeater.o

Panner.o:	Panner.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Panner.h $(XAW)/PannerP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Panner.c
		ar r libXawPlus.a Panner.o

Grip.o:		Grip.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Grip.h $(XAW)/GripP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Grip.c
		ar r libXawPlus.a Grip.o

Scrollbar.o:	Scrollbar.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/Scrollbar.h $(XAW)/ScrollbarP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Scrollbar.c
		ar r libXawPlus.a Scrollbar.o

List.o:		List.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/List.h $(XAW)/ListP.h
		$(CC) $(FLAGS) $(INC_PATH) -c List.c
		ar r libXawPlus.a List.o

# --- Miscelaneous

Tree.o:		Tree.c $(XAW)/XawInit.h $(XAW)/Tree.h $(XAW)/TreeP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Tree.c
		ar r libXawPlus.a Tree.o

DrawingArea.o:	DrawingArea.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/DrawingArea.h $(XAW)/DrawingAreaP.h
		$(CC) $(FLAGS) $(INC_PATH) -c DrawingArea.c
		ar r libXawPlus.a DrawingArea.o

StripChart.o:	StripChart.c $(XAW)/XawInit.h \
		$(XAW)/Simple.h $(XAW)/SimpleP.h $(XAW)/StripChart.h $(XAW)/StripCharP.h
		$(CC) $(FLAGS) $(INC_PATH) -c StripChart.c
		ar r libXawPlus.a StripChart.o

Dialog.o:	Dialog.c $(XAW)/StrToPmap.h $(XAW)/XawInit.h \
		$(XAW)/Form.h $(XAW)/FormP.h $(XAW)/AsciiText.h $(XAW)/Command.h $(XAW)/Label.h \
		$(XAW)/Dialog.h $(XAW)/DialogP.h
		$(CC) $(FLAGS) $(INC_PATH) -c Dialog.c
		ar r libXawPlus.a Dialog.o

