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

vector<float> generatePatches(vector<Point> controlPoints, vector<unsigned int> patchesIndexes, int tesselation, vector<unsigned int>* indexes) {
    vector<float> points;
    int index = 0;
    size_t pSize = patchesIndexes.size();
    float delta = 1.0f / (float) tesselation;
    float vecU[4], vecNU[4], vecV[4];

    float m[16] = {-1, 3, -3, 1,
                   3, -6, 3, 0,
                   -3, 3, 0, 0,
                   1, 0, 0, 0};

    for(int k=0; k< pSize; k+=16){

        Point p[16] = {controlPoints[patchesIndexes[k]], controlPoints[patchesIndexes[k+4]], controlPoints[patchesIndexes[k+8]], controlPoints[patchesIndexes[k+12]],
                       controlPoints[patchesIndexes[k+1]], controlPoints[patchesIndexes[k+5]], controlPoints[patchesIndexes[k+9]], controlPoints[patchesIndexes[k+13]],
                       controlPoints[patchesIndexes[k+2]], controlPoints[patchesIndexes[k+6]], controlPoints[patchesIndexes[k+10]], controlPoints[patchesIndexes[k+14]],
                       controlPoints[patchesIndexes[k+3]], controlPoints[patchesIndexes[k+7]], controlPoints[patchesIndexes[k+11]], controlPoints[patchesIndexes[k+15]]};

        Point a[16]; Point b[16]; // a = M * P, b = a * Mt
        multiplyMatrixPointMatrix(m, p, a);
        multiplyPointMatrixMatrix(a, m, b);

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
            points.push_back(f2.getX());points.push_back(f2.getY());points.push_back(f2.getZ());

            for(int j = 1; j <= tesselation; j++) {
                float v = delta * (float) j;

                getVectorF(v, vecV);
                Point f3, f4;
                multiplyPointVectorVector(c, vecV, &f3);
                multiplyPointVectorVector(nc, vecV, &f4);

                points.push_back(f3.getX());points.push_back(f3.getY());points.push_back(f3.getZ());
                points.push_back(f4.getX());points.push_back(f4.getY());points.push_back(f4.getZ());

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

vector<Point> readPatch(const string& fileName, vector<unsigned int>* indexes){
    ifstream file("../patches/" + fileName);
    vector<Point> controlPoints;

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else{
        string line;
        regex r("\\s+");
        getline(file, line); // Read the first line (number of patches)
        line = regex_replace(line, r, "");
        int nPatches = stoi(line);

        for(int i = 0; i < nPatches && getline(file, line); i++) {
            line = regex_replace(line, r, "");

            string first;
            stringstream ss(line);
            for(int j = 0; j < 16 && getline(ss, first, ','); j++) {
                indexes->push_back(stoi(first));
            }
        }

        getline(file, line); // Read the number of points
        line = regex_replace(line, r, "");
        int nPoints = stoi(line);

        regex re(R"(\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*)");
        smatch match;

        for(int i = 0; i < nPoints && getline(file, line); i++) {
            if (regex_search(line, match, re)) {
                Point p(stof(match.str(1)), stof(match.str(2)), stof(match.str(3)));
                controlPoints.push_back(p);
            }
        }
    }
    file.close();

    return controlPoints;
}