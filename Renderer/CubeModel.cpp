#include "stdafx.h"

#include "CubeModel.h"
#include <glm/gtc/matrix_transform.hpp>


namespace {
	inline void changeOrderWithNormal(const glm::vec3& n, glm::vec3& p) {
		if (n == glm::vec3(1.0f, 0.0f, 0.0f) || n == glm::vec3(-1.0f, 0.0f, 0.0f)) {
			// l,r
			p = glm::vec3(p.z, p.y, p.x);
		}
		else if (n == glm::vec3(0.0f, 1.0f, 0.0f) || n == glm::vec3(0.0f, -1.0f, 0.0f)) {
			// u,d
			p = glm::vec3(p.x, p.z, p.y);
		}
	}

	inline bool areVectorsEqual(const glm::vec3& one, const glm::vec3& other) {
		if (fabs(one.x - other.x) < 0.00001
			&& fabs(one.y - other.y) < 0.00001
			&& fabs(one.z - other.z) < 0.00001)
		{
			return true;
		}
		return false;
	}

	void calculateCubePositions(std::vector<float>& vertices, glm::vec3& position, const glm::vec3& normal) {
		const float t = 0.99;
		if (areVectorsEqual(normal,glm::vec3(0.0f, 0.0f, 1.0f)) || areVectorsEqual(normal, glm::vec3(0.0f, 0.0f, -1.0f))) {
			vertices.push_back(position.x - t); vertices.push_back(position.y - t); vertices.push_back(position.z);
			vertices.push_back(position.x - t); vertices.push_back(position.y + t); vertices.push_back(position.z);
			vertices.push_back(position.x + t); vertices.push_back(position.y + t); vertices.push_back(position.z);
			vertices.push_back(position.x + t); vertices.push_back(position.y - t); vertices.push_back(position.z);
			vertices.push_back(position.x - t); vertices.push_back(position.y - t); vertices.push_back(position.z);
			vertices.push_back(position.x + t); vertices.push_back(position.y + t); vertices.push_back(position.z);
		}
		else if (areVectorsEqual(normal, glm::vec3(0.0f, 1.0f, 0.0f)) || areVectorsEqual(normal, glm::vec3(0.0f, -1.0f, 0.0f))) {
			vertices.push_back(position.x - t); vertices.push_back(position.y); vertices.push_back(position.z - t);
			vertices.push_back(position.x + t); vertices.push_back(position.y); vertices.push_back(position.z - t);
			vertices.push_back(position.x + t); vertices.push_back(position.y); vertices.push_back(position.z + t);
			vertices.push_back(position.x - t); vertices.push_back(position.y); vertices.push_back(position.z + t);
			vertices.push_back(position.x - t); vertices.push_back(position.y); vertices.push_back(position.z - t);
			vertices.push_back(position.x + t); vertices.push_back(position.y); vertices.push_back(position.z + t);
		}
		else if (areVectorsEqual(normal, glm::vec3(1.0f,0.0f,0.0f)) || areVectorsEqual(normal, glm::vec3(-1.0f,0.0f,0.0f))) {
			vertices.push_back(position.x); vertices.push_back(position.y - t); vertices.push_back(position.z - t);
			vertices.push_back(position.x); vertices.push_back(position.y - t); vertices.push_back(position.z + t);
			vertices.push_back(position.x); vertices.push_back(position.y + t); vertices.push_back(position.z + t);
			vertices.push_back(position.x); vertices.push_back(position.y + t); vertices.push_back(position.z - t);
			vertices.push_back(position.x); vertices.push_back(position.y - t); vertices.push_back(position.z - t);
			vertices.push_back(position.x); vertices.push_back(position.y + t); vertices.push_back(position.z + t);
		}
	}

	inline void calculateCubeColor(std::vector<float>& colors, const glm::vec3& color) {
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
		colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);
	}
	
	std::vector<CubeSquare *> createCubeSquaresOneSide(const glm::vec3& n, const glm::vec3& c, glm::vec3& p) {
		std::vector<CubeSquare *> css = std::vector<CubeSquare *>();

		for (int i = 0; i < 9; i++) {
			css.push_back(new CubeSquare(n, c, p));

			changeOrderWithNormal(n, p);
			if (i == 2 || i == 5) {
				p.y -= 4.0f;
				p.x += 2.0f;
			}
			else {
				p.y += 2.0f;
			}
			changeOrderWithNormal(n, p);
		}

		return css;
	}

	template <typename T> inline void addToVector(std::vector<T>& base, const std::vector<T>& add) {
		for (std::vector<T>::const_iterator iter = add.begin(); iter != add.end(); iter++) {
			base.push_back(*iter);
		}
	}

	template <typename T> inline std::vector<T> combineInVector(const std::vector<T>& vec1, const std::vector<T>& vec2, 
		const std::vector<T>& vec3, const std::vector<T>& vec4, const std::vector<T>& vec5, const std::vector<T>& vec6)
	{
		std::vector<T> base = std::vector<T>();

		addToVector(base, vec1);
		addToVector(base, vec2);
		addToVector(base, vec3);
		addToVector(base, vec4);
		addToVector(base, vec5);
		addToVector(base, vec6);

		return base;
	}

	std::vector<CubePiece *> createPiecesFromSquares(const std::vector<CubeSquare *>& base) 
	{
		std::vector<CubePiece *> allPieces = std::vector<CubePiece *>();

		// top face
		allPieces.push_back(new CubePiece(base.at(0), base.at(38), base.at(20)));
		allPieces.push_back(new CubePiece(base.at(1), base.at(23)));
		allPieces.push_back(new CubePiece(base.at(2), base.at(26), base.at(47)));
		allPieces.push_back(new CubePiece(base.at(3), base.at(41)));
		allPieces.push_back(new CubePiece(base.at(4)));
		allPieces.push_back(new CubePiece(base.at(5), base.at(50)));
		allPieces.push_back(new CubePiece(base.at(6), base.at(44), base.at(29)));
		allPieces.push_back(new CubePiece(base.at(7), base.at(32)));
		allPieces.push_back(new CubePiece(base.at(8), base.at(35), base.at(53)));
		allPieces.push_back(new CubePiece(base.at(9), base.at(36), base.at(18)));
		allPieces.push_back(new CubePiece(base.at(10), base.at(21)));
		allPieces.push_back(new CubePiece(base.at(11), base.at(24), base.at(45)));
		allPieces.push_back(new CubePiece(base.at(12), base.at(39)));
		allPieces.push_back(new CubePiece(base.at(13)));
		allPieces.push_back(new CubePiece(base.at(14), base.at(48)));
		allPieces.push_back(new CubePiece(base.at(15), base.at(42), base.at(27)));
		allPieces.push_back(new CubePiece(base.at(16), base.at(30)));
		allPieces.push_back(new CubePiece(base.at(17), base.at(33), base.at(51)));
		allPieces.push_back(new CubePiece(base.at(19), base.at(37)));
		allPieces.push_back(new CubePiece(base.at(22)));
		allPieces.push_back(new CubePiece(base.at(25), base.at(46)));
		allPieces.push_back(new CubePiece(base.at(28), base.at(43)));
		allPieces.push_back(new CubePiece(base.at(31)));
		allPieces.push_back(new CubePiece(base.at(34), base.at(52)));
		allPieces.push_back(new CubePiece(base.at(40)));
		allPieces.push_back(new CubePiece(base.at(49)));

		return allPieces;
	}

	template <typename T> void safeDeleteVector(std::vector<T *> deletable) {
		for (std::vector<T *>::iterator iter = deletable.begin(); iter != deletable.end(); iter++) {
			T * item = *iter;
			if (item != nullptr) delete item;
		}
	}

}

CubeSquare::CubeSquare(glm::vec3 faceNormal, glm::vec3 faceColor, glm::vec3 facePosition)
{
	normal = faceNormal;
	color = faceColor;
	position = facePosition;
}

CubePiece::CubePiece(CubeSquare * piece)
	: _piece1(piece), _piece2(nullptr), _piece3(nullptr)
{
}

CubePiece::CubePiece(CubeSquare * piece1, CubeSquare * piece2)
	: _piece1(piece1), _piece2(piece2), _piece3(nullptr)
{
}

CubePiece::CubePiece(CubeSquare * piece1, CubeSquare * piece2, CubeSquare * piece3)
	: _piece1(piece1), _piece2(piece2), _piece3(piece3)
{
}

void
CubePiece::rotate(const glm::mat4& rot)
{
	_piece1->position = glm::vec3(rot * glm::vec4(_piece1->position, 1.0f));
	_piece1->normal = glm::normalize(glm::vec3(rot * glm::vec4(_piece1->normal, 1.0f)));

	if (_piece2 != nullptr) {
		_piece2->position = glm::vec3(rot * glm::vec4(_piece2->position, 1.0f));
		_piece2->normal = glm::normalize(glm::vec3(rot * glm::vec4(_piece2->normal, 1.0f)));
	}

	if (_piece3 != nullptr) {
		_piece3->position = glm::vec3(rot * glm::vec4(_piece3->position, 1.0f));
		_piece3->normal = glm::normalize(glm::vec3(rot * glm::vec4(_piece3->normal, 1.0f)));
	}
}

bool 
CubePiece::hasNormal(const glm::vec3& n) const
{
	if (areVectorsEqual(_piece1->normal,n))
		return true;
	else if (_piece2 != nullptr && areVectorsEqual(_piece2->normal, n))
		return true;
	else if (_piece3 != nullptr && areVectorsEqual(_piece3->normal, n))
		return true;

	return false;
}

CubeModel::CubeModel() : _cubeSquares(), _cubePieces(), _allPoints(), _allColors(), modelMatrix()
{
	// create pieces 26 (9,9,8)
	// create faces 9
	// model matrix

	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -25.0f));
	modelMatrix = glm::rotate(modelMatrix, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	createModel();
	updateAllPointsAndColors();
}

CubeModel::~CubeModel()
{
	safeDeleteVector(_cubeSquares);
	safeDeleteVector(_cubePieces);
}

std::vector<float>
CubeModel::getPositions() const
{
	return _allPoints;
}

std::vector<float>
CubeModel::getColors() const
{
	return _allColors;
}

int
CubeModel::getTriangleCount() const
{
	return _cubeSquares.size() * 6;
}

void
CubeModel::createModel()
{
	// starting points
	// 
	// top		-  1,  6,  1
	// bottom	-  1,  0,  1 
	// left		-  0,  1,  1
	// right	-  6,  1,  1
	// front	-  1,  1,  0
	// back		-  1,  1,  6
	//	
	// transpose by -3,-3,-3 so that 0,0,0 is the origin

	std::vector<CubeSquare *> topFace = createCubeSquaresOneSide(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 3.0f, -2.0f));
	std::vector<CubeSquare *> botFace = createCubeSquaresOneSide(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(-2.0f, -3.0f, -2.0f));
	std::vector<CubeSquare *> lefFace = createCubeSquaresOneSide(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-3.0f, -2.0f, -2.0f));
	std::vector<CubeSquare *> rigFace = createCubeSquaresOneSide(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0f, -2.0f, -2.0f));
	std::vector<CubeSquare *> froFace = createCubeSquaresOneSide(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(-2.0f, -2.0f, -3.0f));
	std::vector<CubeSquare *> bacFace = createCubeSquaresOneSide(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.0f, -2.0f, 3.0f));

	_cubeSquares = combineInVector(topFace, botFace, lefFace, rigFace, froFace, bacFace);
	
	_cubePieces = createPiecesFromSquares(_cubeSquares);
}

void
CubeModel::updateAllPointsAndColors()
{
	_allPoints.clear();
	_allColors.clear();

	for (std::vector<CubeSquare *>::const_iterator iter = _cubeSquares.begin(); iter != _cubeSquares.end(); iter++) {
		CubeSquare * cs = *iter;
		calculateCubePositions(_allPoints, cs->position, cs->normal);
		calculateCubeColor(_allColors, cs->color);
	}
}

void
CubeModel::rotate(ROTATE rotation)
{
	switch (rotation) {
	case ROTATE::X:
		rotateAll(0, true);
		break;
	case ROTATE::Y:
		rotateAll(1, true);
		break;
	case ROTATE::Z:
		rotateAll(2, true);
		break;
	case ROTATE::L:
		rotateWithNormal(glm::vec3(-1.0f, 0.0f, 0.0f), true);
		break;
	case ROTATE::Lp:
		rotateWithNormal(glm::vec3(-1.0f, 0.0f, 0.0f), false);
		break;

	case ROTATE::R:
		rotateWithNormal(glm::vec3(1.0f, 0.0f, 0.0f), true);
		break;
	case ROTATE::Rp:
		rotateWithNormal(glm::vec3(1.0f, 0.0f, 0.0f), false);
		break;

	case ROTATE::U:
		rotateWithNormal(glm::vec3(0.0f, 1.0f, 0.0f), true);
		break;
	case ROTATE::Up:
		rotateWithNormal(glm::vec3(0.0f, 1.0f, 0.0f), false);
		break;
	case ROTATE::D:
		rotateWithNormal(glm::vec3(0.0f, -1.0f, 0.0f), true);
		break;
	case ROTATE::Dp:
		rotateWithNormal(glm::vec3(0.0f, -1.0f, 0.0f), false);
		break;
	case ROTATE::F:
		rotateWithNormal(glm::vec3(0.0f, 0.0f, -1.0f), true);
		break;
	case ROTATE::Fp:
		rotateWithNormal(glm::vec3(0.0f, 0.0f, -1.0f), false);
		break;
	case ROTATE::B:
		rotateWithNormal(glm::vec3(0.0f, 0.0f, 1.0f), true);
		break;
	case ROTATE::Bp:
		rotateWithNormal(glm::vec3(0.0f, 0.0f, 1.0f), false);
		break;
	case ROTATE::M:
		break;
	case ROTATE::Mp:
		break;
	case ROTATE::E:
		break;
	case ROTATE::Ep:
		break;
	case ROTATE::S:
		break;
	case ROTATE::Sp:
		break;

	}
	updateAllPointsAndColors();
}

void
CubeModel::rotateWithNormal(const glm::vec3& normal, bool clockwise)
{
	float angle = clockwise ? -90.0f : 90.0f;

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, normal);

	for (std::vector<CubePiece *>::iterator iter = _cubePieces.begin(); iter != _cubePieces.end(); iter++) {
		CubePiece * cp = *iter;
		
		if (cp->hasNormal(normal))
			cp->rotate(rot);
	}

}

void 
CubeModel::rotateAll(int axis, bool clockwise) 
{
	float angle = clockwise ? -90.0f : 90.0f;

	glm::mat4 rot;
	if (axis == 0)
		rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	else if (axis == 1)
		rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	else 
		rot = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(1.0f, 0.0f, 0.0f));

	for (std::vector<CubePiece *>::iterator iter = _cubePieces.begin(); iter != _cubePieces.end(); iter++) {
		CubePiece * cp = *iter;
		cp->rotate(rot);
	}

}
