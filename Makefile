# Credit for the Auto-dependecy generation portion
# of this file goes to Tom Tromey <tromey@cygnus.com>.
# This file is based on a post that focused on
# this method. The post can be found at:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Adapted for SDL et al. by Zackary Sullivan

# CXX = g++

# DEPDIR := .deps
# $(shell mkdir -p $(DEPDIR) >/dev/null)
# DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# CXXFLAGS = `sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

# LDFLAGS = `sdl2-config --libs` -lm -lSDL2_image -lexpat -lSDL2_ttf -lSDL2_mixer

# # OBJS = $(shell find . -regex ".*\.cpp" -printf "%P " | sed 's/.cpp/.o/g')
# SRCPTH = $(shell find . -regex ".*\.cpp" | tr '\n' ' ')
# SRCFLS = $(notdir $(SRCPTH))
# OBJDIR  = build
# $(shell mkdir -p $(OBJDIR) >/dev/null)
# OBJFLS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFLS))

# EXEC = run

# $(DEPDIR)/%.d: ;
# .PRECIOUS: $(DEPDIR)/%.d

# $(OBJDIR)/%.o: %.cpp
# $(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
# 	@echo "Creating Object $(notdir $@)"
# 	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
# 	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

# $(EXEC): $(OBJFLS)
# 	@echo "Creating Executable $@"
# 	@$(CXX) $(CXXFLAGS) -o $@ $(OBJFLS) $(LDFLAGS)

# clean:
# 	@echo "Deleting Objects"
# 	@rm -rf $(OBJDIR)
# 	@echo "Deleting Executable"
# 	@rm -rf $(EXEC)
# 	@echo "Deleting Dependecy List"
# 	@rm -rf $(DEPDIR)
# 	@echo "Deleting Images"
# 	@rm -rf frames/*.bmp

# include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(OBJS))))

# Credit for the Auto-dependecy generation portion
# of this file goes to Tom Tromey <tromey@cygnus.com>.
# This file is based on a post that focused on
# this method. The post can be found at:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Adapted for SDL et al. by Zackary Sullivan

CXX = g++

DEPDIR := .deps
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CXXFLAGS = `sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# OBJS = $(shell find . -regex ".*\.cpp" -printf "%P " | sed 's/.cpp/.o/g')
SRCPTH = $(shell find . -regex ".*\.cpp" | tr '\n' ' ')
SRCFLS = $(notdir $(SRCPTH))
OBJDIR  = build
$(shell mkdir -p $(OBJDIR) >/dev/null)
OBJFLS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFLS))

EXEC = run

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
# $(OBJDIR)/%.o: $(SRCFLS) $(DEPDIR)/%.d
	@echo "Creating Object $(notdir $@)"
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

$(EXEC): $(OBJFLS)
	@echo "Creating Executable $@"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJFLS) $(LDFLAGS)

clean:
	@echo "Deleting Objects"
	@rm -rf $(OBJDIR)
	@echo "Deleting Executable"
	@rm -rf $(EXEC)
	@echo "Deleting Dependecy List"
	@rm -rf $(DEPDIR)
	@echo "Deleting Images"
	@rm -rf frames/*.bmp

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(OBJS))))
