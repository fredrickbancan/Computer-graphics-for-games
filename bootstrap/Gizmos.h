#pragma once

#include <glm/fwd.hpp>

namespace aie {

// a singleton class for rendering immediate-mode 3-D primitives
class Gizmos {
public:

	static void		create(unsigned int maxLines, unsigned int maxTris,
						   unsigned int max2DLines, unsigned int max2DTris);
	static void		destroy();

	// removes all Gizmos
	static void		clear();

	// draws current Gizmo buffers, either using a combined (projection * view) matrix, or separate matrices
	static void		draw(const glm::mat4& projectionView);
	static void		draw(const glm::mat4& projection, const glm::mat4& view);
	
	// the projection matrix here should ideally be orthographic with a near of -1 and far of 1
	static void		draw2D(const glm::mat4& projection);
	static void		draw2D(float screenWidth, float screenHeight);

	// adds a single debug line
	static void		addLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour);

	// adds a single debug line
	static void		addLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour0, const glm::vec4& colour1);

	// adds a triangle
	static void		addTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour);

	// adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
	// at the transform's translation. Optional scale available
	static void		addTransform(const glm::mat4& transform, float scale = 1.0f);
	
	// adds a wireframe Axis-Aligned Bounding-Box with optional transform for rotation/translation
	static void		addAABB(const glm::vec3& center, const glm::vec3& extents, 
							const glm::vec4& colour, const glm::mat4* transform = nullptr);

	// adds an Axis-Aligned Bounding-Box with optional transform for rotation
	static void		addAABBFilled(const glm::vec3& center, const glm::vec3& extents, 
								  const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds a cylinder aligned to the Y-axis with optional transform for rotation
	static void		addCylinderFilled(const glm::vec3& center, float radius, float halfLength,
									  unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds a double-sided hollow ring in the XZ axis with optional transform for rotation.
	// if fillColour.w == 0 then only an outer and inner line is drawn
	static void		addRing(const glm::vec3& center, float innerRadius, float outerRadius,
							unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds a double-sided disk in the XZ axis with optional transform for rotation.
	// if fillColour.w == 0 then only an outer line is drawn
	static void		addDisk(const glm::vec3& center, float radius,
							unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds an arc, around the Y-axis
	// if fillColour.w == 0 then only an outer line is drawn
	static void		addArc(const glm::vec3& center, float rotation, float radius, float halfAngle,
						   unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds an arc, around the Y-axis, starting at the inner radius and extending to the outer radius
	// if fillColour.w == 0 then only an outer line is drawn
	static void		addArcRing(const glm::vec3& center, float rotation, 
							   float innerRadius, float outerRadius, float arcHalfAngle,
							   unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform = nullptr);

	// adds a Sphere at a given position, with a given number of rows, and columns, radius and a max and min long and latitude
	static void		addSphere(const glm::vec3& center, float radius, int rows, int columns, const glm::vec4& fillColour, 
							  const glm::mat4* transform = nullptr, float longMin = 0.f, float longMax = 360, 
							  float latMin = -90, float latMax = 90 );

	// adds a capsule with a set height and radius
	static void		addCapsule(const glm::vec3& center, float height, float radius,
							   int rows, int cols, const glm::vec4& fillColour, const glm::mat4* rotation = nullptr);

	// adds a single Hermite spline curve
	static void		addHermiteSpline(const glm::vec3& start, const glm::vec3& end,
									 const glm::vec3& tangentStart, const glm::vec3& tangentEnd, unsigned int segments, const glm::vec4& colour);

	// 2-dimensional gizmos
	static void		add2DLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& colour);
	static void		add2DLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& colour0, const glm::vec4& colour1);
	static void		add2DTri(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2, const glm::vec4& colour);
	static void		add2DTri(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2, const glm::vec4& colour0, const glm::vec4& colour1, const glm::vec4& colour2);
	static void		add2DAABB(const glm::vec2& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr);	
	static void		add2DAABBFilled(const glm::vec2& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform = nullptr);	
	static void		add2DCircle(const glm::vec2& center, float radius, unsigned int segments, const glm::vec4& colour, const glm::mat4* transform = nullptr);
	
private:

	Gizmos(unsigned int maxLines, unsigned int maxTris,
		   unsigned int max2DLines, unsigned int max2DTris);
	~Gizmos();

	struct GizmoVertex {
		float x, y, z, w;
		float r, g, b, a;
	};

	struct GizmoLine {
		GizmoVertex v0;
		GizmoVertex v1;
	};

	struct GizmoTri {
		GizmoVertex v0;
		GizmoVertex v1;
		GizmoVertex v2;
	};

	unsigned int	m_shader;

	// line data
	unsigned int	m_maxLines;
	unsigned int	m_lineCount;
	GizmoLine*		m_lines;

	unsigned int	m_lineVAO;
	unsigned int 	m_lineVBO;

	// triangle data
	unsigned int	m_maxTris;
	unsigned int	m_triCount;
	GizmoTri*		m_tris;

	unsigned int	m_triVAO;
	unsigned int 	m_triVBO;
	
	unsigned int	m_transparentTriCount;
	GizmoTri*		m_transparentTris;

	unsigned int	m_transparentTriVAO;
	unsigned int 	m_transparentTriVBO;
	
	// 2D line data
	unsigned int	m_max2DLines;
	unsigned int	m_2DlineCount;
	GizmoLine*		m_2Dlines;

	unsigned int	m_2DlineVAO;
	unsigned int 	m_2DlineVBO;

	// 2D triangle data
	unsigned int	m_max2DTris;
	unsigned int	m_2DtriCount;
	GizmoTri*		m_2Dtris;

	unsigned int	m_2DtriVAO;
	unsigned int 	m_2DtriVBO;

	static Gizmos*	sm_singleton;
};

} // namespace aie