CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/fileHandling -Isrc/app/processes

GTK_CFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
GTK_LIBS   = $(shell pkg-config --libs gtkmm-3.0)

# -------------------------
# Console target
# -------------------------
MAIN_TARGET = encrypt_decrypt
MAIN_SRC = main.cpp \
           src/app/processes/ProcessManagement.cpp \
           src/app/fileHandling/IO.cpp \
           src/app/fileHandling/ReadEnv.cpp \
           src/app/encryptDecrypt/Cryption.cpp
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# -------------------------
# Cryption target (optional)
# -------------------------
CRYPTION_TARGET = cryption
CRYPTION_SRC = src/app/encryptDecrypt/CryptionMain.cpp \
               src/app/encryptDecrypt/Cryption.cpp \
               src/app/fileHandling/IO.cpp \
               src/app/fileHandling/ReadEnv.cpp
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)

# -------------------------
# GUI target
# -------------------------
GUI_TARGET = encrypt_decrypt_gui
GUI_SRC = src/app/gui/Main.cpp \
          src/app/gui/MainWindow.cpp \
          src/app/processes/ProcessManagement.cpp \
          src/app/encryptDecrypt/Cryption.cpp \
          src/app/fileHandling/IO.cpp \
          src/app/fileHandling/ReadEnv.cpp
GUI_OBJ = $(GUI_SRC:.cpp=.o)

# -------------------------
# Default build
# -------------------------
all: $(MAIN_TARGET) $(CRYPTION_TARGET) $(GUI_TARGET)

# -------------------------
# Build rules
# -------------------------
$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GUI_TARGET): $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) $(GUI_OBJ) -o $@ $(GTK_LIBS)

# -------------------------
# Object compilation rules
# -------------------------
# GUI objects need GTK flags
src/app/gui/%.o: src/app/gui/%.cpp
	$(CXX) $(CXXFLAGS) $(GTK_CFLAGS) -c $< -o $@

# Process, encrypt/decrypt, fileHandling objects (non-GUI)
src/app/processes/%.o: src/app/processes/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/app/encryptDecrypt/%.o: src/app/encryptDecrypt/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/app/fileHandling/%.o: src/app/fileHandling/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Root-level objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -------------------------
# Clean
# -------------------------
clean:
	rm -f $(MAIN_OBJ) $(CRYPTION_OBJ) $(GUI_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET) $(GUI_TARGET)

.PHONY: clean all
