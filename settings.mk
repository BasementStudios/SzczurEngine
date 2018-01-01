#
# Settings
#
 
MXE=no
MXE_DIR=/mnt/e/Libraries/MXE/mxe
SLIENT=yes
 
#$(error Sprawdz plik konfiguracyjny settings.mk ;D)
# Zmień na odpowiednie ścieżki (btw, ważne dla Windowsa chyba)
 
 
INC_DIR_SFML_32 := c:/Biblioteki/GCC-2.4.2-SFML/include
BIN_DIR_SFML_32 := c:/Biblioteki/GCC-2.4.2-SFML/lib


INC_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/include
LIB_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/lib
BIN_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/bin
 
INC_DIR_BOOST_32 := c:/Biblioteki/boost_1_65_1/include
BIN_DIR_BOOST_32 := 
INC_DIR_BOOST_64 := c:/Biblioteki/boost_1_65_1/include
BIN_DIR_BOOST_64 :=

HEADER_INC_SOL2 := c:/Biblioteki/lua/sol

INC_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/include
LIB_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/lib
BIN_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/bin

HEADER_INC_JSON := c:/Biblioteki/json
CXXFLAGS_STATIC_BOOST  :=
LDFLAGS_STATIC_BOOST  :=
CXXFLAGS_DYNAMIC_BOOST :=
LDFLAGS_DYNAMIC_BOOST :=