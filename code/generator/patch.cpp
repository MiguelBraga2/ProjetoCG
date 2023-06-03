#include <regex>
#include <fstream>
#include "patch.hpp"
#include "../shared/matrixOp.hpp"
#include <string>
#include <sstream>


void getVectorF(float f, float *v) {
    v[0] = f * f * f;
    v[1] = f * f;
    v[2] = f;
    v[3] = 1;
}

void getVectorFDeriv(float f, float *v) {
    v[0] = 3 * f * f;
    v[1] = 2 * f;
    v[2] = 1;
    v[3] = 0;
}

void getDerivU(float u, float v, Point *p, Point *res) {
    float vecV[4], vecUDeriv[4];
    getVectorF(v, vecV);

    Point pn[4];
    getVectorFDeriv(u, vecUDeriv);
    multiplyVectorPointMatrix(vecUDeriv, p, pn);
    multiplyPointVectorVector(pn, vecV, res);
}

void getDerivV(float u, float v, Point *p, Point *res) {
    float vecU[4], vecVDeriv[4];
    getVectorF(u, vecU);

    Point pn[4];
    getVectorFDeriv(v, vecVDeriv);
    multiplyVectorPointMatrix(vecU, p, pn);
    multiplyPointVectorVector(pn, vecVDeriv, res);
}

Point getNormal(float u, float v, Point *p) {
    Point derivU, derivV;
    getDerivU(u, v, p, &derivU);
    getDerivV(u, v, p, &derivV);
    Point r = Point::crossProduct(derivU, derivV);
    r.normalize();
    return r;
}


vector<float> generatePatches(vector<Point> controlPoints, vector<unsigned int> patchesIndexes, int tesselation, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoord, float* radiusSphere, Point approxCenter) {
    vector<float> points;
    int index = 0;
    size_t pSize = patchesIndexes.size();
    float delta = 1.0f / (float) tesselation;
    float vecU[4], vecNU[4], vecV[4];

    float m[16] = {-1, 3, -3, 1,
                   3, -6, 3, 0,
                   -3, 3, 0, 0,
                   1, 0, 0, 0};

    if (patchesIndexes.size() == 0){
        pSize = controlPoints.size();
    }

    for(int k=0; k< pSize; k+=16){
        Point p[16];
        if (patchesIndexes.size() > 0) {
            p[0] = controlPoints[patchesIndexes[k]]; p[1] = controlPoints[patchesIndexes[k + 4]];
            p[2] = controlPoints[patchesIndexes[k + 8]]; p[3] = controlPoints[patchesIndexes[k + 12]];
            p[4] = controlPoints[patchesIndexes[k + 1]]; p[5] = controlPoints[patchesIndexes[k + 5]];
            p[6] = controlPoints[patchesIndexes[k + 9]]; p[7] = controlPoints[patchesIndexes[k + 13]];
            p[8] = controlPoints[patchesIndexes[k + 2]]; p[9] = controlPoints[patchesIndexes[k + 6]];
            p[10] = controlPoints[patchesIndexes[k + 10]]; p[11] = controlPoints[patchesIndexes[k + 14]];
            p[12] = controlPoints[patchesIndexes[k + 3]]; p[13] = controlPoints[patchesIndexes[k + 7]];
            p[14] = controlPoints[patchesIndexes[k + 11]]; p[15] = controlPoints[patchesIndexes[k + 15]];
        }
        else{
            p[0] = controlPoints[k]; p[1] = controlPoints[k+4];
            p[2] = controlPoints[k+8]; p[3] = controlPoints[k+12];
            p[4] = controlPoints[k+1]; p[5] = controlPoints[k+5];
            p[6] = controlPoints[k+9]; p[7] = controlPoints[k+13];
            p[8] = controlPoints[k+2]; p[9] = controlPoints[k+6];
            p[10] = controlPoints[k+10]; p[11] = controlPoints[k+14];
            p[12] = controlPoints[k+3]; p[13] = controlPoints[k+7];
            p[14] = controlPoints[k+11]; p[15] = controlPoints[k+15];
        }
        Point a[16]; Point b[16]; // a = M * P, b = a * Mt
        multiplyMatrixPointMatrix(m, p, a);
        multiplyPointMatrixMatrix(a, m, b);

        float textInc = 1.0f/(float)tesselation;
        float distToCenter;
        for (int i = 0; i < tesselation; i++) {
            float u = delta * (float) i;
            float nextU = delta * (float) (i+1);

            getVectorF(u, vecU);
            getVectorF(nextU, vecNU);
            Point c[4], nc[4]; // C = U * b
            multiplyVectorPointMatrix(vecU, b, c);
            multiplyVectorPointMatrix(vecNU, b, nc);

            getVectorF(0, vecV);
            Point f1, f2;
            multiplyPointVectorVector(c, vecV, &f1);
            multiplyPointVectorVector(nc, vecV, &f2);

            points.push_back(f1.getX());points.push_back(f1.getY());points.push_back(f1.getZ());

            distToCenter = f1.distanceTo(approxCenter);
            if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;

            Point normal1 = getNormal(u, 0, b);
            if (normal1.getX() != normal1.getX()){
                normals->push_back((*normals)[normals->size()]-3);
                normals->push_back((*normals)[normals->size()]-2);
                normals->push_back((*normals)[normals->size()]-1);
            }else{
                normals->push_back(normal1.getX());
                normals->push_back(normal1.getY());
                normals->push_back(normal1.getZ());
            }

            textCoord->push_back(0);
            textCoord->push_back(1-i*textInc);
            points.push_back(f2.getX());points.push_back(f2.getY());points.push_back(f2.getZ());
            distToCenter = f2.distanceTo(approxCenter);
            if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
            Point normal2 = getNormal(nextU, 0, b);
            if (normal2.getX() != normal2.getX()){
                normals->push_back((*normals)[normals->size()]-3);
                normals->push_back((*normals)[normals->size()]-2);
                normals->push_back((*normals)[normals->size()]-1);
            }else{
                normals->push_back(normal2.getX());
                normals->push_back(normal2.getY());
                normals->push_back(normal2.getZ());
            }
            textCoord->push_back(0);
            textCoord->push_back(1-(i+1)*textInc);

            for(int j = 1; j <= tesselation; j++) {
                float v = delta * (float) j;

                getVectorF(v, vecV);
                Point f3, f4;
                multiplyPointVectorVector(c, vecV, &f3);
                multiplyPointVectorVector(nc, vecV, &f4);

                points.push_back(f3.getX());points.push_back(f3.getY());points.push_back(f3.getZ());
                distToCenter = f3.distanceTo(approxCenter);
                if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
                Point normal3 = getNormal(u, v, b);
                if (normal3.getX() != normal3.getX()){
                    normals->push_back((*normals)[normals->size()]-3);
                    normals->push_back((*normals)[normals->size()]-2);
                    normals->push_back((*normals)[normals->size()]-1);
                }else{
                    normals->push_back(normal3.getX());
                    normals->push_back(normal3.getY());
                    normals->push_back(normal3.getZ());
                }
                textCoord->push_back(j*textInc);
                textCoord->push_back(1-i*textInc);

                points.push_back(f4.getX());points.push_back(f4.getY());points.push_back(f4.getZ());
                distToCenter = f4.distanceTo(approxCenter);
                if (distToCenter > *radiusSphere) *radiusSphere = distToCenter;
                Point normal4 = getNormal(nextU, v, b);
                if (normal4.getX() != normal4.getX()){
                    normals->push_back((*normals)[normals->size()]-3);
                    normals->push_back((*normals)[normals->size()]-2);
                    normals->push_back((*normals)[normals->size()]-1);
                }else{
                    normals->push_back(normal4.getX());
                    normals->push_back(normal4.getY());
                    normals->push_back(normal4.getZ());
                }
                textCoord->push_back(j*textInc);
                textCoord->push_back(1-(i+1)*textInc);

                indexes->push_back(index);
                indexes->push_back(index+1);
                indexes->push_back(index+2);

                indexes->push_back(index+3);
                indexes->push_back(index+2);
                indexes->push_back(index+1);

                index += 2;
            }

            index += 2;
        }
    }

    return points;
}

vector<Point> readPatch(const string& fileName, vector<unsigned int>* indexes, Point* approxCenter){
    ifstream file("../patches/" + fileName);
    vector<Point> controlPoints;
    int format = 0; // 0 - normal (with Indexes) 1 - Rhino (without indexes and y->z)

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else{
        string line;
        regex r("\\s+");
        getline(file, line); // Read the first line (number of patches)
        if (line[0] == 'r'){
            format = 1;
        }
        else {
            line = regex_replace(line, r, "");
            int nPatches = stoi(line);

            for (int i = 0; i < nPatches && getline(file, line); i++) {
                line = regex_replace(line, r, "");

                string first;
                stringstream ss(line);
                for (int j = 0; j < 16 && getline(ss, first, ','); j++) {
                    indexes->push_back(stoi(first));
                }
            }

            getline(file, line); // Read the number of points
            line = regex_replace(line, r, "");
            int nPoints = stoi(line);
        }

        regex re(R"(\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*)");
        smatch match;

        for(int i = 0; getline(file, line); i++) {
            if (regex_search(line, match, re)) {
                if (format == 1){
                    Point p(stof(match.str(1)), stof(match.str(3)), stof(match.str(2)));
                    controlPoints.push_back(p);
                    approxCenter->setX(approxCenter->getX()+p.getX());
                    approxCenter->setY(approxCenter->getY()+p.getY());
                    approxCenter->setZ(approxCenter->getZ()+p.getZ());
                }
                else if (format == 0){
                    Point p(stof(match.str(1)), stof(match.str(2)), stof(match.str(3)));
                    controlPoints.push_back(p);
                    approxCenter->setX(approxCenter->getX()+p.getX());
                    approxCenter->setY(approxCenter->getY()+p.getY());
                    approxCenter->setZ(approxCenter->getZ()+p.getZ());
                }

            }
        }
    }
    approxCenter->setX(approxCenter->getX()/controlPoints.size());
    approxCenter->setY(approxCenter->getY()/controlPoints.size());
    approxCenter->setZ(approxCenter->getZ()/controlPoints.size());
    std::cout << approxCenter->toString() << endl;
    file.close();

    return controlPoints;
}