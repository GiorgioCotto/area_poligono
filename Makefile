CXX := g++
TARGET := poligono_qt
SRC := poligono_qt.cpp

# Prova prima Qt6, altrimenti ripiega su Qt5.
QT_LIBS := $(shell pkg-config --libs Qt6Widgets 2>/dev/null)
QT_CFLAGS := $(shell pkg-config --cflags Qt6Widgets 2>/dev/null)

ifeq ($(strip $(QT_LIBS)),)
QT_LIBS := $(shell pkg-config --libs Qt5Widgets 2>/dev/null)
QT_CFLAGS := $(shell pkg-config --cflags Qt5Widgets 2>/dev/null)
endif

CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic $(QT_CFLAGS)
LDFLAGS := $(QT_LIBS)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
