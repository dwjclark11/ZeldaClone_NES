#include "MouseControlSystem.h"

int MouseControlSystem::imageSrcX = 0;
int MouseControlSystem::imageSrcY = 0;
int MouseControlSystem::mouseRectX = 0;
int MouseControlSystem::mouseRectY = 0;
int MouseControlSystem::tileScaleX = 0;
int MouseControlSystem::tileScaleY = 0;
int MouseControlSystem::gridSize = 32;
int MouseControlSystem::boxColliderWidth = 0;
int MouseControlSystem::boxColliderHeight = 0;
int MouseControlSystem::boxColliderOffsetX = 0;
int MouseControlSystem::boxColliderOffsetY = 0;
int MouseControlSystem::layer = 0;
bool MouseControlSystem::isCollision = false;
bool MouseControlSystem::isTrigger = false;
bool MouseControlSystem::createTile = false;
bool MouseControlSystem::createObstacles = false;
bool MouseControlSystem::createBoxCollider = false;
bool MouseControlSystem::gridSnap = false;
std::string MouseControlSystem::imageID = "";
TriggerType MouseControlSystem::triggerType = NO_TRIGGER;
unsigned MouseControlSystem::triggerNum = 0;
int MouseControlSystem::triggerLevelNum = 0;

int MouseControlSystem::CanvasWidth = 992;
int MouseControlSystem::CanvasHeight = 992;