#include "Gizmos.h"
#include "gl_core_4_4.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

namespace aie {

Gizmos* Gizmos::sm_singleton = nullptr;

Gizmos::Gizmos(unsigned int maxLines, unsigned int maxTris,
			   unsigned int max2DLines, unsigned int max2DTris)
	: m_maxLines(maxLines),
	m_lineCount(0),
	m_lines(new GizmoLine[maxLines]),
	m_maxTris(maxTris),
	m_triCount(0),
	m_tris(new GizmoTri[maxTris]),
	m_transparentTriCount(0),
	m_transparentTris(new GizmoTri[maxTris]),
	m_max2DLines(max2DLines),
	m_2DlineCount(0),
	m_2Dlines(new GizmoLine[max2DLines]),
	m_max2DTris(max2DTris),
	m_2DtriCount(0),
	m_2Dtris(new GizmoTri[max2DTris]) {

	// create shaders
	const char* vsSource = "#version 150\n \
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 150\n \
					 in vec4 vColour; \
                     out vec4 FragColor; \
					 void main()	{ FragColor = vColour; }";
    
    
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, (const char**)&vsSource, 0);
	glCompileShader(vs);

	glShaderSource(fs, 1, (const char**)&fsSource, 0);
	glCompileShader(fs);

	m_shader = glCreateProgram();
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);
	glBindAttribLocation(m_shader, 0, "Position");
	glBindAttribLocation(m_shader, 1, "Colour");
	glLinkProgram(m_shader);
    
	int success = GL_FALSE;
    glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength + 1];
        
		glGetProgramInfoLog(m_shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link Gizmo shader program!\n%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
    
    // create VBOs
	glGenBuffers( 1, &m_lineVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxLines * sizeof(GizmoLine), m_lines, GL_DYNAMIC_DRAW);

	glGenBuffers( 1, &m_triVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * sizeof(GizmoTri), m_tris, GL_DYNAMIC_DRAW);

	glGenBuffers( 1, &m_transparentTriVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_transparentTriVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxTris * sizeof(GizmoTri), m_transparentTris, GL_DYNAMIC_DRAW);

	glGenBuffers( 1, &m_2DlineVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_2DlineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_max2DLines * sizeof(GizmoLine), m_2Dlines, GL_DYNAMIC_DRAW);

	glGenBuffers( 1, &m_2DtriVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_2DtriVBO);
	glBufferData(GL_ARRAY_BUFFER, m_max2DTris * sizeof(GizmoTri), m_2Dtris, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), (void*)16);

	glGenVertexArrays(1, &m_triVAO);
	glBindVertexArray(m_triVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_triVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), (void*)16);

	glGenVertexArrays(1, &m_transparentTriVAO);
	glBindVertexArray(m_transparentTriVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_transparentTriVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), (void*)16);

	glGenVertexArrays(1, &m_2DlineVAO);
	glBindVertexArray(m_2DlineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_2DlineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), (void*)16);

	glGenVertexArrays(1, &m_2DtriVAO);
	glBindVertexArray(m_2DtriVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_2DtriVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), (void*)16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Gizmos::~Gizmos() {
	delete[] m_lines;
	delete[] m_tris;
	delete[] m_transparentTris;
	glDeleteBuffers( 1, &m_lineVBO );
	glDeleteBuffers( 1, &m_triVBO );
	glDeleteBuffers( 1, &m_transparentTriVBO );
	glDeleteVertexArrays( 1, &m_lineVAO );
	glDeleteVertexArrays( 1, &m_triVAO );
	glDeleteVertexArrays( 1, &m_transparentTriVAO );
	delete[] m_2Dlines;
	delete[] m_2Dtris;
	glDeleteBuffers( 1, &m_2DlineVBO );
	glDeleteBuffers( 1, &m_2DtriVBO );
	glDeleteVertexArrays( 1, &m_2DlineVAO );
	glDeleteVertexArrays( 1, &m_2DtriVAO );
	glDeleteProgram(m_shader);
}

void Gizmos::create(unsigned int maxLines, unsigned int maxTris,
					unsigned int max2DLines, unsigned int max2DTris) {
	if (sm_singleton == nullptr)
		sm_singleton = new Gizmos(maxLines,maxTris,max2DLines,max2DTris);
}

void Gizmos::destroy() {
	delete sm_singleton;
	sm_singleton = nullptr;
}

void Gizmos::clear() {
	sm_singleton->m_lineCount = 0;
	sm_singleton->m_triCount = 0;
	sm_singleton->m_transparentTriCount = 0;
	sm_singleton->m_2DlineCount = 0;
	sm_singleton->m_2DtriCount = 0;
}

// Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
// at the transform's translation. Optional scale available.
void Gizmos::addTransform(const glm::mat4& transform, float scale) {

	glm::vec4 vXAxis = transform[3] + transform[0] * scale;
	glm::vec4 vYAxis = transform[3] + transform[1] * scale;
	glm::vec4 vZAxis = transform[3] + transform[2] * scale;

	glm::vec4 vRed(1,0,0,1);
	glm::vec4 vGreen(0,1,0,1);
	glm::vec4 vBlue(0,0,1,1);

	addLine(glm::vec3(transform[3]), glm::vec3(vXAxis), vRed, vRed);
	addLine(glm::vec3(transform[3]), glm::vec3(vYAxis), vGreen, vGreen);
	addLine(glm::vec3(transform[3]), glm::vec3(vZAxis), vBlue, vBlue);
}

void Gizmos::addAABB(const glm::vec3& center, 
	const glm::vec3& rvExtents, 
	const glm::vec4& colour, 
	const glm::mat4* transform) {

	glm::vec3 vVerts[8];
	glm::vec3 c = center;
	glm::vec3 vX(rvExtents.x, 0, 0);
	glm::vec3 vY(0, rvExtents.y, 0);
	glm::vec3 vZ(0, 0, rvExtents.z);

	if (transform != nullptr) {
		vX = glm::vec3((*transform * glm::vec4(vX,0)));
		vY = glm::vec3((*transform * glm::vec4(vY,0)));
		vZ = glm::vec3((*transform * glm::vec4(vZ,0)));
		c = glm::vec3((*transform)[3]) + c;
	}

	// top verts
	vVerts[0] = c - vX - vZ - vY;
	vVerts[1] = c - vX + vZ - vY;
	vVerts[2] = c + vX + vZ - vY;
	vVerts[3] = c + vX - vZ - vY;

	// bottom verts
	vVerts[4] = c - vX - vZ + vY;
	vVerts[5] = c - vX + vZ + vY;
	vVerts[6] = c + vX + vZ + vY;
	vVerts[7] = c + vX - vZ + vY;

	addLine(vVerts[0], vVerts[1], colour, colour);
	addLine(vVerts[1], vVerts[2], colour, colour);
	addLine(vVerts[2], vVerts[3], colour, colour);
	addLine(vVerts[3], vVerts[0], colour, colour);

	addLine(vVerts[4], vVerts[5], colour, colour);
	addLine(vVerts[5], vVerts[6], colour, colour);
	addLine(vVerts[6], vVerts[7], colour, colour);
	addLine(vVerts[7], vVerts[4], colour, colour);

	addLine(vVerts[0], vVerts[4], colour, colour);
	addLine(vVerts[1], vVerts[5], colour, colour);
	addLine(vVerts[2], vVerts[6], colour, colour);
	addLine(vVerts[3], vVerts[7], colour, colour);
}

void Gizmos::addAABBFilled(const glm::vec3& center, 
	const glm::vec3& rvExtents, 
	const glm::vec4& fillColour, 
	const glm::mat4* transform) {

	glm::vec3 vVerts[8];
	glm::vec3 tempCenter = center;
	glm::vec3 vX(rvExtents.x, 0, 0);
	glm::vec3 vY(0, rvExtents.y, 0);
	glm::vec3 vZ(0, 0, rvExtents.z);

	if (transform != nullptr) {
		vX = glm::vec3((*transform * glm::vec4(vX, 0)));
		vY = glm::vec3((*transform * glm::vec4(vY, 0)));
		vZ = glm::vec3((*transform * glm::vec4(vZ, 0)));
		tempCenter = glm::vec3((*transform)[3]) + tempCenter;
	}

	// top verts
	vVerts[0] = tempCenter - vX - vZ - vY;
	vVerts[1] = tempCenter - vX + vZ - vY;
	vVerts[2] = tempCenter + vX + vZ - vY;
	vVerts[3] = tempCenter + vX - vZ - vY;

	// bottom verts
	vVerts[4] = tempCenter - vX - vZ + vY;
	vVerts[5] = tempCenter - vX + vZ + vY;
	vVerts[6] = tempCenter + vX + vZ + vY;
	vVerts[7] = tempCenter + vX - vZ + vY;

	glm::vec4 vWhite(1,1,1,1);

	addLine(vVerts[0], vVerts[1], vWhite, vWhite);
	addLine(vVerts[1], vVerts[2], vWhite, vWhite);
	addLine(vVerts[2], vVerts[3], vWhite, vWhite);
	addLine(vVerts[3], vVerts[0], vWhite, vWhite);

	addLine(vVerts[4], vVerts[5], vWhite, vWhite);
	addLine(vVerts[5], vVerts[6], vWhite, vWhite);
	addLine(vVerts[6], vVerts[7], vWhite, vWhite);
	addLine(vVerts[7], vVerts[4], vWhite, vWhite);

	addLine(vVerts[0], vVerts[4], vWhite, vWhite);
	addLine(vVerts[1], vVerts[5], vWhite, vWhite);
	addLine(vVerts[2], vVerts[6], vWhite, vWhite);
	addLine(vVerts[3], vVerts[7], vWhite, vWhite);

	// top
	addTri(vVerts[2], vVerts[1], vVerts[0], fillColour);
	addTri(vVerts[3], vVerts[2], vVerts[0], fillColour);

	// bottom
	addTri(vVerts[5], vVerts[6], vVerts[4], fillColour);
	addTri(vVerts[6], vVerts[7], vVerts[4], fillColour);

	// front
	addTri(vVerts[4], vVerts[3], vVerts[0], fillColour);
	addTri(vVerts[7], vVerts[3], vVerts[4], fillColour);

	// back
	addTri(vVerts[1], vVerts[2], vVerts[5], fillColour);
	addTri(vVerts[2], vVerts[6], vVerts[5], fillColour);

	// left
	addTri(vVerts[0], vVerts[1], vVerts[4], fillColour);
	addTri(vVerts[1], vVerts[5], vVerts[4], fillColour);

	// right
	addTri(vVerts[2], vVerts[3], vVerts[7], fillColour);
	addTri(vVerts[6], vVerts[2], vVerts[7], fillColour);
}

void Gizmos::addCylinderFilled(const glm::vec3& center, float radius, float fHalfLength,
	unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform) {

	glm::vec4 white(1,1,1,1);

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float segmentSize = (2 * glm::pi<float>()) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec3 v0top(0,fHalfLength,0);
		glm::vec3 v1top( sinf( i * segmentSize ) * radius, fHalfLength, cosf( i * segmentSize ) * radius);
		glm::vec3 v2top( sinf( (i+1) * segmentSize ) * radius, fHalfLength, cosf( (i+1) * segmentSize ) * radius);
		glm::vec3 v0bottom(0,-fHalfLength,0);
		glm::vec3 v1bottom( sinf( i * segmentSize ) * radius, -fHalfLength, cosf( i * segmentSize ) * radius);
		glm::vec3 v2bottom( sinf( (i+1) * segmentSize ) * radius, -fHalfLength, cosf( (i+1) * segmentSize ) * radius);

		if (transform != nullptr) {
			v0top = glm::vec3((*transform * glm::vec4(v0top, 0)));
			v1top = glm::vec3((*transform * glm::vec4(v1top, 0)));
			v2top = glm::vec3((*transform * glm::vec4(v2top, 0)));
			v0bottom = glm::vec3((*transform * glm::vec4(v0bottom, 0)));
			v1bottom = glm::vec3((*transform * glm::vec4(v1bottom, 0)));
			v2bottom = glm::vec3((*transform * glm::vec4(v2bottom, 0)));
		}

		// triangles
		addTri(tempCenter + v0top, tempCenter + v1top, tempCenter + v2top, fillColour);
		addTri(tempCenter + v0bottom, tempCenter + v2bottom, tempCenter + v1bottom, fillColour);
		addTri(tempCenter + v2top, tempCenter + v1top, tempCenter + v1bottom, fillColour);
		addTri(tempCenter + v1bottom, tempCenter + v2bottom, tempCenter + v2top, fillColour);

		// lines
		addLine(tempCenter + v1top, tempCenter + v2top, white, white);
		addLine(tempCenter + v1top, tempCenter + v1bottom, white, white);
		addLine(tempCenter + v1bottom, tempCenter + v2bottom, white, white);
	}
}

void Gizmos::addRing(const glm::vec3& center, float innerRadius, float outerRadius,
	unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform) {

	glm::vec4 vSolid = fillColour;
	vSolid.w = 1;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float fSegmentSize = (2 * glm::pi<float>()) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec3 v1outer( sinf( i * fSegmentSize ) * outerRadius, 0, cosf( i * fSegmentSize ) * outerRadius );
		glm::vec3 v2outer( sinf( (i+1) * fSegmentSize ) * outerRadius, 0, cosf( (i+1) * fSegmentSize ) * outerRadius );
		glm::vec3 v1inner( sinf( i * fSegmentSize ) * innerRadius, 0, cosf( i * fSegmentSize ) * innerRadius );
		glm::vec3 v2inner( sinf( (i+1) * fSegmentSize ) * innerRadius, 0, cosf( (i+1) * fSegmentSize ) * innerRadius );

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
			v1inner = glm::vec3((*transform * glm::vec4(v1inner, 0)));
			v2inner = glm::vec3((*transform * glm::vec4(v2inner, 0)));
		}

		if (fillColour.w != 0) {
			addTri(tempCenter + v2outer, tempCenter + v1outer, tempCenter + v1inner, fillColour);
			addTri(tempCenter + v1inner, tempCenter + v2inner, tempCenter + v2outer, fillColour);

			addTri(tempCenter + v1inner, tempCenter + v1outer, tempCenter + v2outer, fillColour);
			addTri(tempCenter + v2outer, tempCenter + v2inner, tempCenter + v1inner, fillColour);
		}
		else {
			// line
			addLine(tempCenter + v1inner, tempCenter + v2inner, vSolid, vSolid);
			addLine(tempCenter + v1outer, tempCenter + v2outer, vSolid, vSolid);
		}
	}
}

void Gizmos::addDisk(const glm::vec3& center, float radius,
	unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform) {

	glm::vec4 vSolid = fillColour;
	vSolid.w = 1;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float fSegmentSize = (2 * glm::pi<float>()) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec3 v1outer( sinf( i * fSegmentSize ) * radius, 0, cosf( i * fSegmentSize ) * radius );
		glm::vec3 v2outer( sinf( (i+1) * fSegmentSize ) * radius, 0, cosf( (i+1) * fSegmentSize ) * radius );

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
		}

		if (fillColour.w != 0) {
			addTri(tempCenter, tempCenter + v1outer, tempCenter + v2outer, fillColour);
			addTri(tempCenter + v2outer, tempCenter + v1outer, tempCenter, fillColour);
		}
		else {
			// line
			addLine(tempCenter + v1outer, tempCenter + v2outer, vSolid, vSolid);
		}
	}
}

void Gizmos::addArc(const glm::vec3& center, float rotation,
	float radius, float arcHalfAngle,
	unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform) {

	glm::vec4 vSolid = fillColour;
	vSolid.w = 1;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float fSegmentSize = (2 * arcHalfAngle) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec3 v1outer( sinf( i * fSegmentSize - arcHalfAngle + rotation ) * radius, 0, cosf( i * fSegmentSize - arcHalfAngle + rotation ) * radius);
		glm::vec3 v2outer( sinf( (i+1) * fSegmentSize - arcHalfAngle + rotation ) * radius, 0, cosf( (i+1) * fSegmentSize - arcHalfAngle + rotation ) * radius);

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
		}

		if (fillColour.w != 0) {
			addTri(tempCenter, tempCenter + v1outer, tempCenter + v2outer, fillColour);
			addTri(tempCenter + v2outer, tempCenter + v1outer, tempCenter, fillColour);
		}
		else {
			// line
			addLine(tempCenter + v1outer, tempCenter + v2outer, vSolid, vSolid);
		}
	}

	// edge lines
	if (fillColour.w == 0) {
		glm::vec3 v1outer( sinf( -arcHalfAngle + rotation ) * radius, 0, cosf( -arcHalfAngle + rotation ) * radius );
		glm::vec3 v2outer( sinf( arcHalfAngle + rotation ) * radius, 0, cosf( arcHalfAngle + rotation ) * radius );

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
		}

		addLine(tempCenter, tempCenter + v1outer, vSolid, vSolid);
		addLine(tempCenter, tempCenter + v2outer, vSolid, vSolid);
	}
}

void Gizmos::addArcRing(const glm::vec3& center, float rotation, 
	float innerRadius, float outerRadius, float arcHalfAngle,
	unsigned int segments, const glm::vec4& fillColour, const glm::mat4* transform) {

	glm::vec4 vSolid = fillColour;
	vSolid.w = 1;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;

	float fSegmentSize = (2 * arcHalfAngle) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec3 v1outer( sinf( i * fSegmentSize - arcHalfAngle + rotation ) * outerRadius, 0, cosf( i * fSegmentSize - arcHalfAngle + rotation ) * outerRadius );
		glm::vec3 v2outer( sinf( (i+1) * fSegmentSize - arcHalfAngle + rotation ) * outerRadius, 0, cosf( (i+1) * fSegmentSize - arcHalfAngle + rotation ) * outerRadius );
		glm::vec3 v1inner( sinf( i * fSegmentSize - arcHalfAngle + rotation  ) * innerRadius, 0, cosf( i * fSegmentSize - arcHalfAngle + rotation  ) * innerRadius );
		glm::vec3 v2inner( sinf( (i+1) * fSegmentSize - arcHalfAngle + rotation  ) * innerRadius, 0, cosf( (i+1) * fSegmentSize - arcHalfAngle + rotation  ) * innerRadius );

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
			v1inner = glm::vec3((*transform * glm::vec4(v1inner, 0)));
			v2inner = glm::vec3((*transform * glm::vec4(v2inner, 0)));
		}

		if (fillColour.w != 0) {
			addTri(tempCenter + v2outer, tempCenter + v1outer, tempCenter + v1inner, fillColour);
			addTri(tempCenter + v1inner, tempCenter + v2inner, tempCenter + v2outer, fillColour);

			addTri(tempCenter + v1inner, tempCenter + v1outer, tempCenter + v2outer, fillColour);
			addTri(tempCenter + v2outer, tempCenter + v2inner, tempCenter + v1inner, fillColour);
		}
		else {
			// line
			addLine(tempCenter + v1inner, tempCenter + v2inner, vSolid, vSolid);
			addLine(tempCenter + v1outer, tempCenter + v2outer, vSolid, vSolid);
		}
	}

	// edge lines
	if (fillColour.w == 0) {
		glm::vec3 v1outer( sinf( -arcHalfAngle + rotation ) * outerRadius, 0, cosf( -arcHalfAngle + rotation ) * outerRadius );
		glm::vec3 v2outer( sinf( arcHalfAngle + rotation ) * outerRadius, 0, cosf( arcHalfAngle + rotation ) * outerRadius );
		glm::vec3 v1inner( sinf( -arcHalfAngle + rotation  ) * innerRadius, 0, cosf( -arcHalfAngle + rotation  ) * innerRadius );
		glm::vec3 v2inner( sinf( arcHalfAngle + rotation  ) * innerRadius, 0, cosf( arcHalfAngle + rotation  ) * innerRadius );

		if (transform != nullptr) {
			v1outer = glm::vec3((*transform * glm::vec4(v1outer, 0)));
			v2outer = glm::vec3((*transform * glm::vec4(v2outer, 0)));
			v1inner = glm::vec3((*transform * glm::vec4(v1inner, 0)));
			v2inner = glm::vec3((*transform * glm::vec4(v2inner, 0)));
		}

		addLine(tempCenter + v1inner, tempCenter + v1outer, vSolid, vSolid);
		addLine(tempCenter + v2inner, tempCenter + v2outer, vSolid, vSolid);
	}
}

void Gizmos::addSphere(const glm::vec3& center, float radius, int rows, int columns, const glm::vec4& fillColour, 
								const glm::mat4* transform, float longMin, float longMax, 
								float latMin, float latMax) {

	float inverseRadius = 1 / radius;

	// invert these first as the multiply is slightly quicker
	float invColumns = 1.0f / columns;
	float invRows = 1.0f / rows;

	float DEG2RAD = glm::pi<float>() / 180;

	glm::vec3 tempCenter = transform != nullptr ? glm::vec3((*transform)[3]) + center : center;
	
	//Lets put everything in radians first
	float latitiudinalRange = (latMax - latMin) * DEG2RAD;
	float longitudinalRange = (longMax - longMin) * DEG2RAD;

	// for each row of the mesh
	glm::vec3* v4Array = new glm::vec3[rows*columns + columns];

	for (int row = 0; row <= rows; ++row) {
		// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
		float ratioAroundXAxis = float(row) * invRows;
		float radiansAboutXAxis  = ratioAroundXAxis * latitiudinalRange + (latMin * DEG2RAD);
		float y  =  radius * sin(radiansAboutXAxis);
		float z  =  radius * cos(radiansAboutXAxis);
		
		for ( int col = 0; col <= columns; ++col ) {
			float ratioAroundYAxis   = float(col) * invColumns;
			float theta = ratioAroundYAxis * longitudinalRange + (longMin * DEG2RAD);
			glm::vec3 v4Point( -z * sinf(theta), y, -z * cosf(theta) );
			glm::vec3 v4Normal( inverseRadius * v4Point.x, inverseRadius * v4Point.y, inverseRadius * v4Point.z);

			if (transform != nullptr) {
				v4Point = glm::vec3((*transform * glm::vec4(v4Point, 0)));
				v4Normal = glm::vec3((*transform * glm::vec4(v4Normal, 0)));
			}

			int index = row * columns + (col % columns);
			v4Array[index] = v4Point;
		}
	}
	
	for (int face = 0; face < (rows)*(columns); ++face ) {
		int iNextFace = face + 1;		
		
		if( iNextFace % columns == 0 )
			iNextFace = iNextFace - (columns);

		addLine(tempCenter + v4Array[face], tempCenter + v4Array[face+columns], glm::vec4(1.f,1.f,1.f,1.f), glm::vec4(1.f,1.f,1.f,1.f));
		
		if( face % columns == 0 && longitudinalRange < (glm::pi<float>() * 2))
			continue;
		addLine(tempCenter + v4Array[iNextFace+columns], tempCenter + v4Array[face+columns], glm::vec4(1.f,1.f,1.f,1.f), glm::vec4(1.f,1.f,1.f,1.f));

		addTri(tempCenter + v4Array[iNextFace+columns], tempCenter + v4Array[face], tempCenter + v4Array[iNextFace], fillColour);
		addTri(tempCenter + v4Array[iNextFace+columns], tempCenter + v4Array[face+columns], tempCenter + v4Array[face], fillColour);
	}

	delete[] v4Array;	
}

void Gizmos::addCapsule(const glm::vec3& center, float height, float radius,
						int rows, int cols, const glm::vec4& fillColour, const glm::mat4* rotation) {

	float sphereCenters = (height * 0.5f) - radius;
	glm::vec4 top = glm::vec4(0, sphereCenters, 0, 0);
	glm::vec4 bottom = glm::vec4(0, -sphereCenters, 0, 0);
	glm::vec4 white(1);

	if (rotation) {
		top = (*rotation) * top + (*rotation)[3];
		bottom = (*rotation) * bottom + (*rotation)[3];
	}

	glm::vec3 topCenter = center + glm::vec3(top);
	glm::vec3 bottomCenter = center + glm::vec3(bottom);

	float inverseRadius = 1 / radius;

	// invert these first as the multiply is slightly quicker
	float invColumns = 1.0f / float(cols);
	float invRows = 1.0f / float(rows);

	float DEG2RAD = glm::pi<float>() / 180;
	float latMin = -90, latMax = 90, longMin = 0.f, longMax = 360;

	// lets put everything in radians first
	float latitiudinalRange = (latMax - latMin) * DEG2RAD;
	float longitudinalRange = (longMax - longMin) * DEG2RAD;

	// for each row of the mesh
	glm::vec3* v4Array = new glm::vec3[rows*cols + cols];

	for (int row = 0; row <= (rows); ++row) {
		// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
		float ratioAroundXAxis = float(row) * invRows;
		float radiansAboutXAxis = ratioAroundXAxis * latitiudinalRange + (latMin * DEG2RAD);
		float y = radius * sin(radiansAboutXAxis);
		float z = radius * cos(radiansAboutXAxis);

		for (int col = 0; col <= cols; ++col) {
			float ratioAroundYAxis = float(col) * invColumns;
			float theta = ratioAroundYAxis * longitudinalRange + (longMin * DEG2RAD);
			glm::vec3 v4Point(-z * sinf(theta), y, -z * cosf(theta));
			glm::vec3 v4Normal(inverseRadius * v4Point.x, inverseRadius * v4Point.y, inverseRadius * v4Point.z);

			if (rotation != nullptr) {
				v4Point = glm::vec3((*rotation * glm::vec4(v4Point, 0)));
				v4Normal = glm::vec3((*rotation * glm::vec4(v4Normal, 0)));
			}

			int index = row * cols + (col % cols);
			v4Array[index] = v4Point;
		}
	}

	// create a sphere that is split in two
	for (int face = 0; face < (rows)*(cols); ++face) {
		int iNextFace = face + 1;

		if (iNextFace % cols == 0)
			iNextFace = iNextFace - (cols);

		glm::vec3 tempCenter = topCenter;
		if (face < (rows / 2)*cols)
			tempCenter = bottomCenter;

		addLine(tempCenter + v4Array[face], tempCenter + v4Array[face + cols], white, white);

		if (face % cols == 0 && longitudinalRange < (glm::pi<float>() * 2))
			continue;

		addLine(tempCenter + v4Array[iNextFace + cols], tempCenter + v4Array[face + cols], white, white);
		addTri(tempCenter + v4Array[iNextFace + cols], tempCenter + v4Array[face], tempCenter + v4Array[iNextFace], fillColour);
		addTri(tempCenter + v4Array[iNextFace + cols], tempCenter + v4Array[face + cols], tempCenter + v4Array[face], fillColour);
	}

	delete[] v4Array;

	for (int i = 0; i < cols; ++i) {
		float x = (float)i / (float)cols;
		float x1 = (float)(i+1) / (float)cols;
		x *= 2.0f * glm::pi<float>();
		x1 *= 2.0f * glm::pi<float>();

		glm::vec3 pos = glm::vec3(cosf(x), 0, sinf(x)) * radius;
		glm::vec3 pos1 = glm::vec3(cosf(x1), 0, sinf(x1)) * radius;

		if (rotation) {
			pos = glm::vec3((*rotation) * glm::vec4(pos,0));
			pos1 = glm::vec3((*rotation) * glm::vec4(pos1,0));
		}

		addTri(topCenter + pos1, bottomCenter + pos1, bottomCenter + pos, fillColour);
		addTri(topCenter + pos1, bottomCenter + pos, topCenter + pos, fillColour);

		addLine(topCenter + pos, topCenter + pos1, white, white);
		addLine(bottomCenter + pos, bottomCenter + pos1, white, white);
		addLine(topCenter + pos, bottomCenter + pos, white, white);
	}
}

void Gizmos::addHermiteSpline(const glm::vec3& start, const glm::vec3& end,
	const glm::vec3& tangentStart, const glm::vec3& tangentEnd, unsigned int segments, const glm::vec4& colour) {

	segments = segments > 1 ? segments : 1;

	glm::vec3 prev = start;

	for ( unsigned int i = 1 ; i <= segments ; ++i ) {
		float s = i / (float)segments;

		float s2 = s * s;
		float s3 = s2 * s;
		float h1 = (2.0f * s3) - (3.0f * s2) + 1.0f;
		float h2 = (-2.0f * s3) + (3.0f * s2);
		float h3 =  s3- (2.0f * s2) + s;
		float h4 = s3 - s2;
		glm::vec3 p = (start * h1) + (end * h2) + (tangentStart * h3) + (tangentEnd * h4);

		addLine(prev,p,colour,colour);
		prev = p;
	}
}

void Gizmos::addLine(const glm::vec3& v0,  const glm::vec3& v1, const glm::vec4& colour) {
	addLine(v0,v1,colour,colour);
}

void Gizmos::addLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& colour0, const glm::vec4& colour1) {

	if (sm_singleton != nullptr &&
		sm_singleton->m_lineCount < sm_singleton->m_maxLines) {
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.x = v0.x;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.y = v0.y;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.z = v0.z;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.w = 1;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.r = colour0.r;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.g = colour0.g;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.b = colour0.b;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v0.a = colour0.a;

		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.x = v1.x;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.y = v1.y;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.z = v1.z;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.w = 1;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.r = colour1.r;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.g = colour1.g;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.b = colour1.b;
		sm_singleton->m_lines[sm_singleton->m_lineCount].v1.a = colour1.a;

		sm_singleton->m_lineCount++;
	}
}

void Gizmos::addTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& colour) {
	if (sm_singleton != nullptr) {
		if (colour.w == 1) {
			if (sm_singleton->m_triCount < sm_singleton->m_maxTris) {
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.x = v0.x;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.y = v0.y;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.z = v0.z;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.w = 1;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.x = v1.x;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.y = v1.y;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.z = v1.z;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.w = 1;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.x = v2.x;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.y = v2.y;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.z = v2.z;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.w = 1;

				sm_singleton->m_tris[sm_singleton->m_triCount].v0.r = colour.r;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.g = colour.g;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.b = colour.b;
				sm_singleton->m_tris[sm_singleton->m_triCount].v0.a = colour.a;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.r = colour.r;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.g = colour.g;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.b = colour.b;
				sm_singleton->m_tris[sm_singleton->m_triCount].v1.a = colour.a;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.r = colour.r;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.g = colour.g;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.b = colour.b;
				sm_singleton->m_tris[sm_singleton->m_triCount].v2.a = colour.a;

				sm_singleton->m_triCount++;
			}
		}
		else {
			if (sm_singleton->m_transparentTriCount < sm_singleton->m_maxTris) {
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.x = v0.x;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.y = v0.y;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.z = v0.z;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.w = 1;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.x = v1.x;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.y = v1.y;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.z = v1.z;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.w = 1;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.x = v2.x;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.y = v2.y;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.z = v2.z;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.w = 1;

				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.r = colour.r;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.g = colour.g;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.b = colour.b;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v0.a = colour.a;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.r = colour.r;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.g = colour.g;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.b = colour.b;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v1.a = colour.a;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.r = colour.r;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.g = colour.g;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.b = colour.b;
				sm_singleton->m_transparentTris[sm_singleton->m_transparentTriCount].v2.a = colour.a;

				sm_singleton->m_transparentTriCount++;
			}
		}
	}
}

void Gizmos::add2DAABB(const glm::vec2& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform /*= nullptr*/) {	
	glm::vec2 verts[4];
	glm::vec2 vX(extents.x, 0);
	glm::vec2 vY(0, extents.y);

	if (transform != nullptr) {
		vX = glm::vec2((*transform * glm::vec4(vX,0,0)));
		vY = glm::vec2((*transform * glm::vec4(vY,0,0)));
	}

	verts[0] = center - vX - vY;
	verts[1] = center + vX - vY;
	verts[2] = center - vX + vY;
	verts[3] = center + vX + vY;

	add2DLine(verts[0], verts[1], colour, colour);
	add2DLine(verts[1], verts[3], colour, colour);
	add2DLine(verts[2], verts[3], colour, colour);
	add2DLine(verts[2], verts[0], colour, colour);
}

void Gizmos::add2DAABBFilled(const glm::vec2& center, const glm::vec2& extents, const glm::vec4& colour, const glm::mat4* transform /*= nullptr*/) {	
	glm::vec2 verts[4];
	glm::vec2 vX(extents.x, 0);
	glm::vec2 vY(0, extents.y);

	if (transform != nullptr) {
		vX = glm::vec2((*transform * glm::vec4(vX,0,0)));
		vY = glm::vec2((*transform * glm::vec4(vY,0,0)));
	}

	verts[0] = center - vX - vY;
	verts[1] = center + vX - vY;
	verts[2] = center + vX + vY;
	verts[3] = center - vX + vY;
	
	add2DTri(verts[0], verts[1], verts[2], colour);
	add2DTri(verts[0], verts[2], verts[3], colour);
}

void Gizmos::add2DCircle(const glm::vec2& center, float radius, unsigned int segments, const glm::vec4& colour, const glm::mat4* transform /*= nullptr*/) {
	glm::vec4 solidColour = colour;
	solidColour.w = 1;

	float segmentSize = (2 * glm::pi<float>()) / segments;

	for ( unsigned int i = 0 ; i < segments ; ++i ) {
		glm::vec2 v1outer( sinf( i * segmentSize ) * radius, cosf( i * segmentSize ) * radius );
		glm::vec2 v2outer( sinf( (i+1) * segmentSize ) * radius, cosf( (i+1) * segmentSize ) * radius );

		if (transform != nullptr) {
			v1outer = glm::vec2((*transform * glm::vec4(v1outer,0,0)));
			v2outer = glm::vec2((*transform * glm::vec4(v2outer,0,0)));
		}

		if (colour.w != 0) {
			add2DTri(center, center + v1outer, center + v2outer, colour);
			add2DTri(center + v2outer, center + v1outer, center, colour);
		}
		else {
			// line
			add2DLine(center + v1outer, center + v2outer, solidColour, solidColour);
		}
	}
}

void Gizmos::add2DLine(const glm::vec2& rv0,  const glm::vec2& rv1, const glm::vec4& colour) {
	add2DLine(rv0,rv1,colour,colour);
}

void Gizmos::add2DLine(const glm::vec2& rv0, const glm::vec2& rv1, const glm::vec4& colour0, const glm::vec4& colour1) {
	if (sm_singleton != nullptr &&
		sm_singleton->m_2DlineCount < sm_singleton->m_max2DLines) {
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.x = rv0.x;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.y = rv0.y;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.z = 1;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.w = 1;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.r = colour0.r;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.g = colour0.g;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.b = colour0.b;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v0.a = colour0.a;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.x = rv1.x;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.y = rv1.y;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.z = 1;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.w = 1;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.r = colour1.r;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.g = colour1.g;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.b = colour1.b;
		sm_singleton->m_2Dlines[sm_singleton->m_2DlineCount].v1.a = colour1.a;

		sm_singleton->m_2DlineCount++;
	}
}

void Gizmos::add2DTri(const glm::vec2& rv0, const glm::vec2& rv1, const glm::vec2& rv2, const glm::vec4& colour) {
	add2DTri(rv0, rv1, rv2, colour, colour, colour);
}

void Gizmos::add2DTri(const glm::vec2& rv0, const glm::vec2& rv1, const glm::vec2& rv2, const glm::vec4& colour0, const glm::vec4& colour1, const glm::vec4& colour2) {
	if (sm_singleton != nullptr) {
		if (sm_singleton->m_2DtriCount < sm_singleton->m_max2DTris) {
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.x = rv0.x;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.y = rv0.y;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.z = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.w = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.x = rv1.x;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.y = rv1.y;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.z = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.w = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.x = rv2.x;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.y = rv2.y;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.z = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.w = 1;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.r = colour0.r;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.g = colour0.g;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.b = colour0.b;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v0.a = colour0.a;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.r = colour1.r;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.g = colour1.g;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.b = colour1.b;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v1.a = colour1.a;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.r = colour2.r;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.g = colour2.g;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.b = colour2.b;
			sm_singleton->m_2Dtris[sm_singleton->m_2DtriCount].v2.a = colour2.a;

			sm_singleton->m_2DtriCount++;
		}
	}
}

void Gizmos::draw(const glm::mat4& projection, const glm::mat4& view) {
	draw(projection * view);
}

void Gizmos::draw(const glm::mat4& projectionView) {
	if ( sm_singleton != nullptr && 
		(sm_singleton->m_lineCount > 0 || 
		 sm_singleton->m_triCount > 0 || 
		 sm_singleton->m_transparentTriCount > 0)) {
		int shader = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

		glUseProgram(sm_singleton->m_shader);
		
		unsigned int projectionViewUniform = glGetUniformLocation(sm_singleton->m_shader,"ProjectionView");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionView));

		if (sm_singleton->m_lineCount > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_lineVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_lineCount * sizeof(GizmoLine), sm_singleton->m_lines);

			glBindVertexArray(sm_singleton->m_lineVAO);
			glDrawArrays(GL_LINES, 0, sm_singleton->m_lineCount * 2);
		}

		if (sm_singleton->m_triCount > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_triVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_triCount * sizeof(GizmoTri), sm_singleton->m_tris);

			glBindVertexArray(sm_singleton->m_triVAO);
			glDrawArrays(GL_TRIANGLES, 0, sm_singleton->m_triCount * 3);
		}
		
		if (sm_singleton->m_transparentTriCount > 0) {
			// not ideal to store these, but Gizmos must work stand-alone
			GLboolean blendEnabled = glIsEnabled(GL_BLEND);
			GLboolean depthMask = GL_TRUE;
			glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
			int src, dst;
			glGetIntegerv(GL_BLEND_SRC, &src);
			glGetIntegerv(GL_BLEND_DST, &dst);
			
			// setup blend states
			if (blendEnabled == GL_FALSE)
				glEnable(GL_BLEND);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_transparentTriVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_transparentTriCount * sizeof(GizmoTri), sm_singleton->m_transparentTris);

			glBindVertexArray(sm_singleton->m_transparentTriVAO);
			glDrawArrays(GL_TRIANGLES, 0, sm_singleton->m_transparentTriCount * 3);

			// reset state
			glDepthMask(depthMask);
			glBlendFunc(src, dst);
			if (blendEnabled == GL_FALSE)
				glDisable(GL_BLEND);
		}

		glUseProgram(shader);
	}
}

void Gizmos::draw2D(float screenWidth, float screenHeight) {
	draw2D(glm::ortho(0.f, screenWidth, 0.f, screenHeight));
}

void Gizmos::draw2D(const glm::mat4& projection) {
	if ( sm_singleton != nullptr && 
		(sm_singleton->m_2DlineCount > 0 || 
		 sm_singleton->m_2DtriCount > 0)) {
		int shader = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

		glUseProgram(sm_singleton->m_shader);
		
		unsigned int projectionViewUniform = glGetUniformLocation(sm_singleton->m_shader,"ProjectionView");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projection));

		if (sm_singleton->m_2DlineCount > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_2DlineVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_2DlineCount * sizeof(GizmoLine), sm_singleton->m_2Dlines);

			glBindVertexArray(sm_singleton->m_2DlineVAO);
			glDrawArrays(GL_LINES, 0, sm_singleton->m_2DlineCount * 2);
		}

		if (sm_singleton->m_2DtriCount > 0) {
			GLboolean blendEnabled = glIsEnabled(GL_BLEND);

			GLboolean depthMask = GL_TRUE;
			glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

			int src, dst;
			glGetIntegerv(GL_BLEND_SRC, &src);
			glGetIntegerv(GL_BLEND_DST, &dst);

			if (blendEnabled == GL_FALSE)
				glEnable(GL_BLEND);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDepthMask(GL_FALSE);

			glBindBuffer(GL_ARRAY_BUFFER, sm_singleton->m_2DtriVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sm_singleton->m_2DtriCount * sizeof(GizmoTri), sm_singleton->m_2Dtris);

			glBindVertexArray(sm_singleton->m_2DtriVAO);
			glDrawArrays(GL_TRIANGLES, 0, sm_singleton->m_2DtriCount * 3);

			glDepthMask(depthMask);

			glBlendFunc(src, dst);

			if (blendEnabled == GL_FALSE)
				glDisable(GL_BLEND);
		}

		glUseProgram(shader);
	}
}

} // namespace aie