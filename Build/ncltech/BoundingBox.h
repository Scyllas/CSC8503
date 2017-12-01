/******************************************************************************
Class: BoundingBox
Implements:
Author:
Pieran Marris <p.marris@newcastle.ac.uk>
Description:
A very basic way of representing an Axis Algined Bounding Box (AABB), that represents a
bounding cuboid that encompasses a set of given points. The box is always orientated in the same
way, such that the x,y,z of the maximum/minimum dimensions are always algined with the x,y,z axes of the world.

This is currently used to help assist with the shadow mapping, though could be extended to help assist
with graphics frustum culling by using both AABB and bounding sphere's inside SceneNode/Object.

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Matrix4.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>

struct BoundingBox
{
	Vector3 _min;
	Vector3 _max;
	Vector3 _center;


	//Initialize _min to max possible value and vice versa to force the first value incorporated to always be used for both min and max points.
	BoundingBox()
		: _min(FLT_MAX, FLT_MAX, FLT_MAX)
		, _max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
	{
		Center();
	}
	BoundingBox(Vector3 min, Vector3 max)
		: _min(min)
		, _max(max)
	{
		Center();
	}

	//Expand the boundingbox to fit a given point. 
	//  If no points have been set yet, both _min and _max will equal the point provided.
	void ExpandToFit(const Vector3& point)
	{
		_min.x = min(_min.x, point.x);
		_min.y = min(_min.y, point.y);
		_min.z = min(_min.z, point.z);
		_max.x = max(_max.x, point.x);
		_max.y = max(_max.y, point.y);
		_max.z = max(_max.z, point.z);
	}

	//Transform the given AABB (Axis Aligned Bounding Box) and returns a new AABB that encapsulates the new rotated bounding box.
	BoundingBox Transform(const Matrix4& mtx)
	{
		BoundingBox bb;
		bb.ExpandToFit(mtx * Vector3(_min.x, _min.y, _min.z));
		bb.ExpandToFit(mtx * Vector3(_max.x, _min.y, _min.z));
		bb.ExpandToFit(mtx * Vector3(_min.x, _max.y, _min.z));
		bb.ExpandToFit(mtx * Vector3(_max.x, _max.y, _min.z));

		bb.ExpandToFit(mtx * Vector3(_min.x, _min.y, _max.z));
		bb.ExpandToFit(mtx * Vector3(_max.x, _min.y, _max.z));
		bb.ExpandToFit(mtx * Vector3(_min.x, _max.y, _max.z));
		bb.ExpandToFit(mtx * Vector3(_max.x, _max.y, _max.z));
		return bb;
	}

	Vector3 Center() {

		float x = (this->_min.x + this->_min.x) / 2.0f;
		float y = (this->_min.y + this->_min.y) / 2.0f;
		float z = (this->_min.z + this->_min.z) / 2.0f;

		return Vector3(x, y, z);
	}

	BoundingBox DebugDraw() {
		NCLDebug::DrawThickLine(_min, Vector3(_min.x, _max.y, _max.z), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		NCLDebug::DrawThickLine(_min, Vector3(_max.x, _max.y, _min.z), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		NCLDebug::DrawThickLine(_min, Vector3(_max.x, _min.y, _max.z), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));

		NCLDebug::DrawThickLine(Vector3(_min.x, _min.y, _max.z), _max, 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		NCLDebug::DrawThickLine(Vector3(_min.x, _max.y, _min.z), _max, 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		NCLDebug::DrawThickLine(Vector3(_max.x, _min.y, _min.z), _max, 1.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));


	}
};