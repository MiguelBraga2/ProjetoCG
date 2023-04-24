#include "Creator.h"
#include "Transformations/translation.hpp"

Group* Creator::drawCube(Group g, Point p) {
	Group* newGroup = g.clone();
	vector<Point> v;
	Translation* t = new Translation(p.getX(), p.getY(), p.getZ(), 0, false,v);

	newGroup->addTransformation(t);

	return newGroup;
}