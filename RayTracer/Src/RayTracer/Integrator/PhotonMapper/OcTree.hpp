#pragma once
#include "../../../Core/Bounds.h"
#include <vector>

namespace RayTracer
{
	template<class T>
	struct SearchResult
	{
	public:
		SearchResult(const T& data, double distance2) : data(data), distance2(distance2)
		{
		}
		T data;
		double distance2;
	};

	template<class T>
	class OcTreeNode
	{
	public:
		OcTreeNode(const Bounds& bounds, bool isLeaf);
		virtual ~OcTreeNode();

		void Add(T data, int depth, int maxDepth);
		void SearchByRadius(std::vector<SearchResult<T>>& results, const Vector3& position, float radius, int maxCount);
		bool IsLeaf() const { return m_isLeaf; }

	private:
		int GetChildIndex(const Vector3& position);
		void GetChildBounds(const Vector3& position, Bounds& bounds);

	private:
		Bounds m_bounds;
		OcTreeNode* m_childs[8];
		std::vector<T> m_datas;
		bool m_isLeaf;
	};

	template<class T>
	class OcTree
	{
	public:
		OcTree(const Bounds& bounds, int maxDepth = 11);
		~OcTree();

		void Add(T data);
		void SearchByRadius(std::vector<SearchResult<T>>& results, const Vector3& position, float radius, int maxCount);

	private:
		int m_maxDepth;
		OcTreeNode<T>* m_root;
	};
}

template<class T>
RayTracer::OcTreeNode<T>::OcTreeNode(const Bounds& bounds, bool isLeaf)
{
	m_bounds = bounds;
	m_isLeaf = isLeaf;
	for (int i = 0; i < 8; i++)
	{
		m_childs[i] = nullptr;
	}
}

template<class T>
RayTracer::OcTreeNode<T>::~OcTreeNode()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_childs[i] != nullptr)
			delete m_childs[i];
	}
}

template<class T>
void RayTracer::OcTreeNode<T>::Add(T data, int depth, int maxDepth)
{
	Vector3 pos = data.position;
	if (m_bounds.Contains(pos) == false)
		return;

	if (m_isLeaf)
	{
		m_datas.push_back(data);
		return;
	}
	
	
	
		int nodeIndex = GetChildIndex(pos);
		if (m_childs[nodeIndex] == nullptr)
		{
			Bounds childBounds;
			GetChildBounds(pos, childBounds);
			if (depth == maxDepth)
				m_childs[nodeIndex] = new RayTracer::OcTreeNode<T>(childBounds, true);
			else
				m_childs[nodeIndex] = new RayTracer::OcTreeNode<T>(childBounds, false);
		}
		m_childs[nodeIndex]->Add(data, depth + 1, maxDepth);
	
}

template<class T>
void RayTracer::OcTreeNode<T>::SearchByRadius(std::vector<SearchResult<T>>& results, const Vector3& position, float radius, int maxCount)
{
	double dis2 = m_bounds.GetDistance2(position);
	double radius2 = radius * radius;

	if (dis2 > radius2)
	{
		return;
	}
	if (results.size() >= maxCount)
		return;
	if (m_isLeaf)
	{
		for (int i = 0; i < m_datas.size(); i++)
		{
			Vector3 pos = m_datas[i].position;
			double dis2 = Vector3::Distance2(pos, position);
			if (dis2 < radius2)
			{
				results.emplace_back(m_datas[i], dis2);
				if (results.size() >= maxCount)
					return;
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_childs[i] != nullptr)
			{
				m_childs[i]->SearchByRadius(results, position, radius, maxCount);
				if (results.size() >= maxCount)
					return;
			}
		}
	}
}

template<class T>
int RayTracer::OcTreeNode<T>::GetChildIndex(const Vector3& position)
{
	if (position.x < m_bounds.center.x && position.y < m_bounds.center.y && position.z < m_bounds.center.z)
		return 0;
	if (position.x >= m_bounds.center.x && position.y < m_bounds.center.y && position.z < m_bounds.center.z)
		return 1;
	if (position.x < m_bounds.center.x && position.y < m_bounds.center.y && position.z >= m_bounds.center.z)
		return 2;
	if (position.x >= m_bounds.center.x && position.y < m_bounds.center.y && position.z >= m_bounds.center.z)
		return 3;
	if (position.x < m_bounds.center.x && position.y >= m_bounds.center.y && position.z < m_bounds.center.z)
		return 4;
	if (position.x >= m_bounds.center.x && position.y >= m_bounds.center.y && position.z < m_bounds.center.z)
		return 5;
	if (position.x < m_bounds.center.x && position.y >= m_bounds.center.y && position.z >= m_bounds.center.z)
		return 6;
	return 7;
}

template<class T>
void RayTracer::OcTreeNode<T>::GetChildBounds(const Vector3& position, Bounds& bounds)
{
	bounds.halfSize = Vector3(m_bounds.halfSize.x, m_bounds.halfSize.y, m_bounds.halfSize.z) * 0.5f;
	if (position.x < m_bounds.center.x && position.y < m_bounds.center.y && position.z < m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(-m_bounds.halfSize.x, -m_bounds.halfSize.y, -m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x >= m_bounds.center.x && position.y < m_bounds.center.y && position.z < m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(m_bounds.halfSize.x, -m_bounds.halfSize.y, -m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x < m_bounds.center.x && position.y < m_bounds.center.y && position.z >= m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(-m_bounds.halfSize.x, -m_bounds.halfSize.y, m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x >= m_bounds.center.x && position.y < m_bounds.center.y && position.z >= m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(m_bounds.halfSize.x, -m_bounds.halfSize.y, m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x < m_bounds.center.x && position.y >= m_bounds.center.y && position.z < m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(-m_bounds.halfSize.x, m_bounds.halfSize.y, -m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x >= m_bounds.center.x && position.y >= m_bounds.center.y && position.z < m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(m_bounds.halfSize.x, m_bounds.halfSize.y, -m_bounds.halfSize.z) * 0.5f;
		return;
	}
	if (position.x < m_bounds.center.x && position.y >= m_bounds.center.y && position.z >= m_bounds.center.z)
	{
		bounds.center = m_bounds.center + Vector3(-m_bounds.halfSize.x, m_bounds.halfSize.y, m_bounds.halfSize.z) * 0.5f;
		return;
	}
	bounds.center = m_bounds.center + Vector3(m_bounds.halfSize.x, m_bounds.halfSize.y, m_bounds.halfSize.z) * 0.5f;
}

template<class T>
RayTracer::OcTree<T>::OcTree(const Bounds& bounds, int maxDepth)
{
	m_root = new OcTreeNode<T>(bounds, false);
	m_maxDepth = maxDepth;
}

template<class T>
RayTracer::OcTree<T>::~OcTree()
{
	delete m_root;
}

template<class T>
void RayTracer::OcTree<T>::Add(T data)
{
	m_root->Add(data, 0, m_maxDepth);
}

template<class T>
void RayTracer::OcTree<T>::SearchByRadius(std::vector<SearchResult<T>>& results, const Vector3& position, float radius, int maxCount)
{
	if (m_root == nullptr)
		return;
	m_root->SearchByRadius(results, position, radius, maxCount);
}
