# Woah, this be a comment.

RM = rm -rf

CXX = g++

DEBUG = -O0 -g3
RELEASE = -O2

#LIBDIRS := \
#-L"/dir" \

LIBS := -lGL -lGLU -lm -lpthread -lGLEW -ljson-c -lglfw3 -lX11 -lXrandr -lXi -lXxf86vm

INC += \
-I"./" \
-I"./rapidxml-1.13" \

# Root dir
OBJS := \
./CharSim.o \

# Subdir: framework/
OBJS += \
./framework/ClipRunner.o \
./framework/CollisionDetection.o \
./framework/ControlNode.o \
./framework/Exceptions.o \
./framework/GameEnvironment.o \
./framework/GameLoop.o \
./framework/InverseKinematics.o \
./framework/Math.o \
./framework/Mesh.o \
./framework/MorphMesh.o \
./framework/Picking.o \
./framework/Renderer.o \
./framework/ResourceManager.o \
./framework/SceneGraph.o \
./framework/SceneNode.o \
./framework/SkinnedMesh.o \
./framework/SkinnedMorphMesh.o \

# Subdir: materials/
OBJS += \
./materials/BaseMaterial.o \
./materials/DiffuseMaterial.o \

#Subdir: characters/dragon/
OBJS += \
./characters/dragon/DragonBustNode.o \
./characters/dragon/DragonController.o \
./characters/dragon/DragonEyeNode.o \
./characters/dragon/DragonFeaturesNode.o \
./characters/dragon/DragonSim.o \
./characters/dragon/SkydomeNode.o \

BIN = charsim

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CXX) -D__GXX_EXPERIMENTAL_CXX0X__ $(INC) $(RELEASE) -Wall -c -fmessage-length=0 -std=c++11 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

all: charsim
	
charsim: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	$(CXX) -o "charsim" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
clean:
	$(RM) *.o ./materials/*.o
	$(RM) *.d ./materials/*.d
	$(RM) *.o ./framework/*.o
	$(RM) *.d ./framework/*.d
	$(RM) *.o ./characters/dragon/*.o
	$(RM) *.d ./characters/dragon/*.d
	$(RM) charsim
