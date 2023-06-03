#include "sphere.hpp"
#include "plane.hpp"
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Sphere::Sphere(float radius, Point center) : radius(radius), center(center) {

}

Sphere::Sphere() {
}

void Sphere::setRadius(float radius) {
    this->radius = radius;
}

float Sphere::getRadius() {
    return radius;
}

void Sphere::setCenter(Point center) {
    this->center.setPoint(center.getX(), center.getY(), center.getZ());
}

Point Sphere::getCenter() {
    return this->center;
}

Volume* Sphere::clone() {
    return new Sphere(this->radius, this->center);
}


bool Sphere::test(float *matrix, Camera *camera) {
    
    Point vecD(camera->getLookAtPosition().getX() - camera->getPosition().getX(), camera->getLookAtPosition().getY() - camera->getPosition().getY(), camera->getLookAtPosition().getZ() - camera->getPosition().getZ());    
    vecD.normalize();
    Point centerNP(camera->getPosition().getX() + camera->getNear() * vecD.getX(), camera->getPosition().getY()+ camera->getNear() * vecD.getY(), camera->getPosition().getZ() + camera->getNear() * vecD.getZ());
    float d = centerNP.getX() * vecD.getX() + centerNP.getY() * vecD.getY() + centerNP.getZ() * vecD.getZ();
    Plane np(vecD.getX(), vecD.getY(), vecD.getZ() , -d);

    float distance = np.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }

    Point minusVecD(-vecD.getX(), -vecD.getY(), -vecD.getZ());
    Point centerFP(camera->getPosition().getX() + camera->getFar() * vecD.getX(), camera->getPosition().getY() + camera->getFar() * vecD.getY(), camera->getPosition().getZ() + camera->getFar() * vecD.getZ());
    d = centerFP.getX() * minusVecD.getX() + centerFP.getY() * minusVecD.getY() + centerFP.getZ() * minusVecD.getZ();
    Plane fp(minusVecD.getX(), minusVecD.getY(), minusVecD.getZ(), -d);

    distance = fp.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }

    float hNear = 2 * tan(camera->getFov() / 2) * camera->getNear();
	float wNear = hNear * camera->getRatio();
    float hFar = 2 * tan(camera->getFov() / 2) * camera->getFar();
	float wFar = hFar * camera->getRatio();

    Point right = Point::crossProduct(vecD, Point(camera->getUpVector().getX(), camera->getUpVector().getY(), camera->getUpVector().getZ()));
    right.normalize();

    Point realUp = Point::crossProduct(right, vecD);
    realUp.normalize();

    /*  
        ftl = fc + (up * Hfar/2) - (right * Wfar/2)
        ftr = fc + (up * Hfar/2) + (right * Wfar/2)
	    fbl = fc - (up * Hfar/2) - (right * Wfar/2)
	    fbr = fc - (up * Hfar/2) + (right * Wfar/2)

	
    	ntl = nc + (up * Hnear/2) - (right * Wnear/2)
	    ntr = nc + (up * Hnear/2) + (right * Wnear/2)
	    nbl = nc - (up * Hnear/2) - (right * Wnear/2)
	    nbr = nc - (up * Hnear/2) + (right * Wnear/2)
    
    */
    Point ftl(centerFP.getX() + (realUp.getX() * hFar/2)  - (right.getX() * wFar/2),
              centerFP.getY() + (realUp.getY() * hFar/2)  - (right.getY() * wFar/2),
              centerFP.getZ() + (realUp.getZ() * hFar/2)  - (right.getZ() * wFar/2));

    Point ftr(centerFP.getX() + (realUp.getX() * hFar/2)  + (right.getX() * wFar/2),
              centerFP.getY() + (realUp.getY() * hFar/2)  + (right.getY() * wFar/2),
              centerFP.getZ() + (realUp.getZ() * hFar/2)  + (right.getZ() * wFar/2));
   
    Point fbl(centerFP.getX() - (realUp.getX() * hFar/2)  - (right.getX() * wFar/2),
              centerFP.getY() - (realUp.getY() * hFar/2)  - (right.getY() * wFar/2),
              centerFP.getZ() - (realUp.getZ() * hFar/2)  - (right.getZ() * wFar/2));
   
    Point fbr(centerFP.getX() - (realUp.getX() * hFar/2)  + (right.getX() * wFar/2),
              centerFP.getY() - (realUp.getY() * hFar/2)  + (right.getY() * wFar/2),
              centerFP.getZ() - (realUp.getZ() * hFar/2)  + (right.getZ() * wFar/2));
   
    Point ntl(centerNP.getX() + (realUp.getX() * hNear/2) - (right.getX() * wNear/2),
              centerNP.getY() + (realUp.getY() * hNear/2) - (right.getY() * wNear/2),
              centerNP.getZ() + (realUp.getZ() * hNear/2) - (right.getZ() * wNear/2));

    Point ntr(centerNP.getX() + (realUp.getX() * hNear/2) + (right.getX() * wNear/2),
              centerNP.getY() + (realUp.getY() * hNear/2) + (right.getY() * wNear/2),
              centerNP.getZ() + (realUp.getZ() * hNear/2) + (right.getZ() * wNear/2));

    Point nbl(centerNP.getX() - (realUp.getX() * hNear/2) - (right.getX() * wNear/2),
              centerNP.getY() - (realUp.getY() * hNear/2) - (right.getY() * wNear/2),
              centerNP.getZ() - (realUp.getZ() * hNear/2) - (right.getZ() * wNear/2));

    Point nbr(centerNP.getX() - (realUp.getX() * hNear/2) + (right.getX() * wNear/2),
              centerNP.getY() - (realUp.getY() * hNear/2) + (right.getY() * wNear/2),
              centerNP.getZ() - (realUp.getZ() * hNear/2) + (right.getZ() * wNear/2));

    // Top
    Point v1(ftl.getX() - ntl.getX(), ftl.getY() - ntl.getY(), ftl.getZ()- ntl.getZ());
    Point v2(ntr.getX() - ntl.getX(), ntr.getY() - ntl.getY(), ntr.getZ() - ntl.getZ());
    Point n = Point::crossProduct(v1, v2);
    n.normalize();
    d = ftl.getX() * n.getX() + ftl.getY() * n.getY() + ftl.getZ() * n.getZ();
    Plane tp(n.getX(), n.getY(), n.getZ(), -d);

    distance = tp.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }
    // Bottom
    v1.setPoint(fbl.getX() - nbl.getX(), fbl.getY() - nbl.getY(), fbl.getZ()- nbl.getZ());
    v2.setPoint(nbr.getX() - nbl.getX(), nbr.getY() - nbl.getY(), nbr.getZ() - nbl.getZ());
    n = Point::crossProduct(v2, v1);
    n.normalize();
    d = fbl.getX() * n.getX() + fbl.getY() * n.getY() + fbl.getZ() * n.getZ();
    Plane bp(n.getX(), n.getY(), n.getZ(), -d);

    distance = bp.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }

    // Left 
    v1.setPoint(ftl.getX() - ntl.getX(), ftl.getY() - ntl.getY(), ftl.getZ()- ntl.getZ());
    v2.setPoint(nbl.getX() - ntl.getX(), nbl.getY() - ntl.getY(), nbl.getZ()- ntl.getZ());
    n = Point::crossProduct(v2, v1);
    n.normalize();
    d = ftl.getX() * n.getX() + ftl.getY() * n.getY() + ftl.getZ() * n.getZ();
    Plane lp(n.getX(), n.getY(), n.getZ(), -d);

    distance = lp.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }

    // Right 
    v1.setPoint(ftr.getX() - ntr.getX(), ftr.getY() - ntr.getY(), ftr.getZ()- ntr.getZ());
    v2.setPoint(nbr.getX() - ntr.getX(), nbr.getY() - ntr.getY(), nbr.getZ()- ntr.getZ());
    n = Point::crossProduct(v1, v2);
    n.normalize();
    d = ftr.getX() * n.getX() + ftr.getY() * n.getY() + ftr.getZ() * n.getZ();
    Plane rp(n.getX(), n.getY(), n.getZ(), -d);

    distance = rp.distanceToPoint(this->center);
    if (distance < -this->radius) {
        return false;
    }

    return true;
}
