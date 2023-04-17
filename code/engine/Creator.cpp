#include "Creator.h"
#include "Transformations/translation.hpp"

Group* Creator::drawCube(Group g, Point p) {
	Group* newGroup = g.clone();
	Translation* t = new Translation(p.getX(), p.getY(), p.getZ());

	newGroup->addTransformation(t);

	return newGroup;
}