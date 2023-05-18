#include "plane.hpp"

/**
 * Generates a group of triangles that combine into a grid, making a square (plane) of given length
 * @param length the length of the larger square
 * @param grid number of smaller squares per side
 * @param direction defines the direction of the plane (for each coordinate, 0 - no direction)
 * For example, 0 in the y direction means the plane is parallel to the y=0 plane
 * @param initial the plane to start generating the plane
 * @param clockWiseDir true if direction is set to clockwise, false if direction is set to counterclockwise
 * @return a list of generated triangles
 */
vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index, vector<float>* normals, vector<unsigned int>* normalsIndexes, int *normalIndex, vector<float>* textCoord, vector<unsigned int>* textCoordIndexes, int *textCoordIndex){
    vector<float> vertices;
    float step = length / (float) grid; // side of each of the smaller squares

    Point base (initial.getX(), initial.getY(), initial.getZ());

    if(direction.getY() == 0 && !clockWiseDir) {
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
    } else if (direction.getY() == 0) {
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
    } else if (direction.getX() == 0 && !clockWiseDir) {
        normals->push_back(1);
        normals->push_back(0);
        normals->push_back(0);
    } else if (direction.getX() == 0) {
        normals->push_back(-1);
        normals->push_back(0);
        normals->push_back(0);
    } else if (direction.getZ() == 0 && !clockWiseDir) {
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(1);
    } else if (direction.getZ() == 0) {
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(-1);
    }

    for(int i=0; i<grid; i++) {

        vertices.push_back(base.getX());
        vertices.push_back(base.getY());
        vertices.push_back(base.getZ());
        textCoord->push_back(i);
        textCoord->push_back(grid);

        if (direction.getZ() == 0) {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
            textCoord->push_back(i+1);
            textCoord->push_back(grid);
        }
        else {
            vertices.push_back(base.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
            textCoord->push_back(i+1);
            textCoord->push_back(grid);
        }

        for (int j = 0; j < grid; j++) {

            if (direction.getY() == 0) {
                vertices.push_back(base.getX() + step * direction.getX());
                vertices.push_back(base.getY());
                vertices.push_back(base.getZ());
                textCoord->push_back(i);
                textCoord->push_back(grid-j-1);
            }
            else {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
                textCoord->push_back(i);
                textCoord->push_back(grid-j-1);
            }

            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY() + step * direction.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
            textCoord->push_back(i+1);
            textCoord->push_back(grid-j-1);

            if (!clockWiseDir) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 1);

                indexes->push_back(*index);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 3);

                textCoordIndexes->push_back(*textCoordIndex);
                textCoordIndexes->push_back(*textCoordIndex+ 3);
                textCoordIndexes->push_back(*textCoordIndex+ 1);

                textCoordIndexes->push_back(*textCoordIndex);
                textCoordIndexes->push_back(*textCoordIndex + 2);
                textCoordIndexes->push_back(*textCoordIndex + 3);

            } else {
                indexes->push_back(*index);
                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 3);

                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 2);

                textCoordIndexes->push_back(*textCoordIndex);
                textCoordIndexes->push_back(*textCoordIndex+ 1);
                textCoordIndexes->push_back(*textCoordIndex+ 3);

                textCoordIndexes->push_back(*textCoordIndex);
                textCoordIndexes->push_back(*textCoordIndex + 3);
                textCoordIndexes->push_back(*textCoordIndex + 2);
            }

            normalsIndexes->push_back(*normalIndex);
            normalsIndexes->push_back(*normalIndex);
            normalsIndexes->push_back(*normalIndex);

            normalsIndexes->push_back(*normalIndex);
            normalsIndexes->push_back(*normalIndex);
            normalsIndexes->push_back(*normalIndex);


            // Move the base point
            if (direction.getY() == 0) {
                base.setX(base.getX() + step * direction.getX());
            }
            else {
                base.setY(base.getY() + step * direction.getY());
            }

            (*index) += 2;
            (*textCoordIndex) += 2;
        }
        if (direction.getX() == 0) {
            base.setZ(base.getZ() + step * direction.getZ());
            base.setY(initial.getY());
        } else if (direction.getY() == 0) {
            base.setZ(base.getZ() + step * direction.getZ());
            base.setX(initial.getX());
        } else if (direction.getZ() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setY(initial.getY());
        }

        (*index) += 2;
        (*textCoordIndex) += 2;
    }

    (*normalIndex)++;

    return vertices;
}