#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include "../GameObject/cGameObject.h"
#include "../ModelStuff/cMesh.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

class cPhysics
{
public:
	cPhysics();

	// Alias to a type "existing type" "new type name"
	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	struct Sphere
	{
		glm::vec3 c;
		float r;
	};	

	struct sPhysicsTriangle
	{
		glm::vec3 verts[3];
		glm::vec3 normal;
	};

	struct sCollisionInfo
	{
		glm::vec3 closestPoint;
		glm::vec3 directionOfApproach;
		float penetrationDistance;
		glm::vec3 adjustmentVector;
		unsigned int Object1_ID;		// Shpere
		unsigned int Object2_ID;		// Sphere or TestTriangle
	};
	
	// This "moves" the objects based on the inegration step
	void IntegrationStep( std::vector<cGameObject*>& vec_pGameObjects,
					      float deltaTime );

	void TestForCollisions( std::vector<cGameObject*>& vec_pGameObjects, std::vector<glm::vec3>& todraw);

	// Returns all the triangles and the closest points
	void GetClosestTriangleToPoint( Point pointXYZ, cMesh &mesh, glm::vec3 &closestPoint, sPhysicsTriangle &closestTriangle );


	// *********************************************************************************
	// FOR THE PATTERNS AND FRAMEWORKS MID-TERM
	// 
	// This is like the method above, but doesn't test for the y axis, which gives you the 
	// TRIANGLE (not the point) that that point is over (or under). 
	// 
	void GetClosestTriangleToPoint_FRAMEWORKS_AND_PATTERNS(Point pointXYZ, cMesh& mesh, sPhysicsTriangle& closestTriangle);
	// *********************************************************************************


	// Will return the closest triangles that are within the range "distanceRange".
	// This can be used as a "closest triangles to sphere"
	void GetClosestTrianglesToSphere( cGameObject &testSphere, float distanceRange, cMesh &mesh, std::vector<sPhysicsTriangle> &vecClosestTriangles );


	// Taken from Ericson's book:
	Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);
	int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p);

	void setGravity( glm::vec3 newGravityValue );
	glm::vec3 getGravity(void);

	// Takes a mesh in "model space" and converts it into "world space"
	void CalculateTransformedMesh( cMesh &originalMesh, glm::mat4 matWorld, cMesh &mesh_transformedInWorld );

	float ScalarTriple(Vector u, Vector v, Vector w);
	bool IntersectLineTriangle(Point p, Point q, 
							   Point a, Point b, Point c,
							   float& u, float& v, float& w,
							   Point& collisionPoint);

	static glm::vec3 RK6(glm::vec3 val, glm::vec3 change, float delta);
private:

	// Does collision test and returns collision information
	// Returns true if collision, and will load collisionInfo struct
	bool DoSphereSphereCollisionTest( cGameObject* pA, cGameObject *pB, sCollisionInfo &collisionInfo );
	bool DoShphereMeshCollisionTest( cGameObject* pA, cGameObject* pB, sCollisionInfo &collisionInfo );


	glm::vec3  m_Gravity;

};

#endif
