#ifndef CUBEMODEL_H
#define CUBEMODEL_H

#include <glm/glm.hpp>
#include <vector>

enum RENDERER ROTATE {
	L, Lp,
	R, Rp,
	U, Up,
	D, Dp,
	F, Fp,
	B, Bp,
	M, Mp,
	E, Ep,
	S, Sp,
	X, Xp,
	Y, Yp,
	Z, Zp
};

struct RENDERER CubeSquare {
	CubeSquare(glm::vec3 faceNormal, glm::vec3 faceColor, glm::vec3 facePosition);

	glm::vec3 normal;
	glm::vec3 color;
	glm::vec3 position;
};

class RENDERER CubePiece {
public:
	CubePiece(CubeSquare * piece);
	CubePiece(CubeSquare * piece1, CubeSquare * piece2);
	CubePiece(CubeSquare * piece1, CubeSquare * piece2, CubeSquare * piece3);

	void rotate(const glm::mat4& rot);
	bool hasNormal(const glm::vec3& n) const;

private:
	CubeSquare * _piece1;
	CubeSquare * _piece2;
	CubeSquare * _piece3;
};

class RENDERER CubeModel {
public:
	CubeModel();
	~CubeModel();

	void rotate(ROTATE rotation);

	// helper functions for rendergin
	std::vector<float> getPositions() const;
	std::vector<float> getColors() const;
	int getTriangleCount() const;

	glm::mat4 modelMatrix;


private:
	std::vector<CubeSquare *> _cubeSquares;
	std::vector<CubePiece *> _cubePieces;

	std::vector<float> _allPoints;
	std::vector<float> _allColors;

	void createModel();
	void updateAllPointsAndColors();
	void rotateWithNormal(const glm::vec3& normal, bool clockwise);
	void rotateAll(int axis, bool clockwise);
};

#endif // CUBEMODEL_H