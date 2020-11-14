//
// Created by vsensu on 2020/9/27.
//

#include "Camera.h"

// ===============================================
//
//           Frustum/ Frustum Culling
//
// ===============================================
enum Planes {
	Near,
	Far,
	Left,
	Right,
	Top,
	Bottom,
};

float Plane::distanceToPoint(const glm::vec3& point) const
{
	return glm::dot(point, normal) + distanceToOrigin;
}

void ViewFrustum::update(const glm::mat4& mat) noexcept
{
	// Left
	m_planes[Planes::Left].normal.x = mat[0][3] + mat[0][0];
	m_planes[Planes::Left].normal.y = mat[1][3] + mat[1][0];
	m_planes[Planes::Left].normal.z = mat[2][3] + mat[2][0];
	m_planes[Planes::Left].distanceToOrigin = mat[3][3] + mat[3][0];

	// Right
	m_planes[Planes::Right].normal.x = mat[0][3] - mat[0][0];
	m_planes[Planes::Right].normal.y = mat[1][3] - mat[1][0];
	m_planes[Planes::Right].normal.z = mat[2][3] - mat[2][0];
	m_planes[Planes::Right].distanceToOrigin = mat[3][3] - mat[3][0];

	// Bottom
	m_planes[Planes::Bottom].normal.x = mat[0][3] + mat[0][1];
	m_planes[Planes::Bottom].normal.y = mat[1][3] + mat[1][1];
	m_planes[Planes::Bottom].normal.z = mat[2][3] + mat[2][1];
	m_planes[Planes::Bottom].distanceToOrigin = mat[3][3] + mat[3][1];

	// Top
	m_planes[Planes::Top].normal.x = mat[0][3] - mat[0][1];
	m_planes[Planes::Top].normal.y = mat[1][3] - mat[1][1];
	m_planes[Planes::Top].normal.z = mat[2][3] - mat[2][1];
	m_planes[Planes::Top].distanceToOrigin = mat[3][3] - mat[3][1];

	// Near
	m_planes[Planes::Near].normal.x = mat[0][3] + mat[0][2];
	m_planes[Planes::Near].normal.y = mat[1][3] + mat[1][2];
	m_planes[Planes::Near].normal.z = mat[2][3] + mat[2][2];
	m_planes[Planes::Near].distanceToOrigin = mat[3][3] + mat[3][2];

	// Far
	m_planes[Planes::Far].normal.x = mat[0][3] - mat[0][2];
	m_planes[Planes::Far].normal.y = mat[1][3] - mat[1][2];
	m_planes[Planes::Far].normal.z = mat[2][3] - mat[2][2];
	m_planes[Planes::Far].distanceToOrigin = mat[3][3] - mat[3][2];

	for (auto& plane : m_planes) {
		float length = glm::length(plane.normal);
		plane.normal /= length;
		plane.distanceToOrigin /= length;
	}
}
