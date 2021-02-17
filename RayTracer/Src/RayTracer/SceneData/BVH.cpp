#include "BVH.h"
#include "../Primitive/RayTracerPrimitive.h"
#include "../../Core/Math.h"
#include <vector>
#include <stack>

unsigned int ExpandBits(unsigned int v)
{
	v = (v * 0x00010001u) & 0xFF0000FFu;
	v = (v * 0x00000101u) & 0x0F00F00Fu;
	v = (v * 0x00000011u) & 0xC30C30C3u;
	v = (v * 0x00000005u) & 0x49249249u;
	return v;
}

unsigned int Morton3D(double x, double y, double z)
{
	x = Math::Min(Math::Max(x * 1024.0, 0.0), 1023.0);
	y = Math::Min(Math::Max(y * 1024.0, 0.0), 1023.0);
	z = Math::Min(Math::Max(z * 1024.0, 0.0), 1023.0);
	unsigned int xx = ExpandBits((unsigned int)x);
	unsigned int yy = ExpandBits((unsigned int)y);
	unsigned int zz = ExpandBits((unsigned int)z);
	return xx * 4 + yy * 2 + zz;
}

RayTracer::BVH::BVH()
{
	m_root = nullptr;
}

RayTracer::BVH::~BVH()
{
	if (m_root)
		delete m_root;
	m_root = nullptr;
}

bool RayTracer::BVH::Tracing(const Ray& ray, RayTracingResult& result)
{
	bool raycast = false;
	if (m_root == nullptr)
		return raycast;
	if (m_root->IsLeaf())
	{
		PrimitiveBase* primitive = m_root->GetPrimitive();
		if (primitive != nullptr)
			return primitive->Tracing(ray, result);

		return raycast;
	}

	std::stack<BVHNode*> nodes;
	Bounds rootBounds = m_root->GetBounds();
	if (rootBounds.Raycast(ray))
	{
		nodes.push(m_root);
	}

	while (nodes.size() > 0)
	{
		BVHNode* node = nodes.top();
		nodes.pop();
		if (node->IsLeaf() == false)
		{
			BVHNode* nodeLeftChild = node->GetLeftNode();
			BVHNode* nodeRightChild = node->GetRightNode();
			double hitleftdistance = 0.0, hitrightdistance = 0.0;
			bool hitleft = nodeLeftChild != nullptr ? nodeLeftChild->GetBounds().Raycast(ray, hitleftdistance) : false;
			bool hitright = nodeRightChild != nullptr ? nodeRightChild->GetBounds().Raycast(ray, hitrightdistance) : false;
			if (hitleft && hitright)
			{
				if (hitleftdistance < hitrightdistance)
				{
					nodes.push(nodeRightChild);
					nodes.push(nodeLeftChild);
				}
				else
				{
					nodes.push(nodeLeftChild);
					nodes.push(nodeRightChild);
				}
			}
			else if (hitleft)
				nodes.push(nodeLeftChild);
			else if (hitright)
				nodes.push(nodeRightChild);
		}
		else
		{
			PrimitiveBase* nodePrimitive = node->GetPrimitive();
			if (nodePrimitive != nullptr)
			{
				raycast = nodePrimitive->Tracing(ray, result) || raycast;
			}
		}
	}
	return raycast;
}

void RayTracer::BVH::BuildSceneData(const Bounds& bounds)
{
	if (m_primitivesForBuild.size() == 0)
	{
		m_root = nullptr;
		return;
	}
	std::vector<unsigned int> mortons;
	for (int i = 0; i < m_primitivesForBuild.size(); i++)
	{
		Bounds primitiveBounds;
		m_primitivesForBuild[i]->GetBounds(primitiveBounds);

		Vector3 center = primitiveBounds.center;
		Vector3 boundsMin = bounds.GetMin();
		Vector3 boundsSize = bounds.GetSize();
		double x = (center.x - boundsMin.x) / boundsSize.x;
		double y = (center.y - boundsMin.y) / boundsSize.y;
		double z = (center.z - boundsMin.z) / boundsSize.z;

		unsigned int morton = Morton3D(x, y, z);
		mortons.push_back(morton);
	}

	Sort(mortons);

	m_root = GenerateHierarchy(m_primitivesForBuild, mortons, 0, mortons.size() - 1);
}

RayTracer::BVHNode* RayTracer::BVH::GenerateHierarchy(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int first, int last)
{
	if (first == last)
		return new BVHNode(primitives[first]);

	//查找分割平面位置
	int split = FindSplit(mortons, first, last);

	//将几何体列表根据分割平面分割成左右子树
	BVHNode* child1 = GenerateHierarchy(primitives, mortons, first, split);
	BVHNode* child2 = GenerateHierarchy(primitives, mortons, split + 1, last);

	return new BVHNode(child1, child2);
}

int RayTracer::BVH::FindSplit(std::vector<unsigned int>& mortons, int first, int last)
{
	unsigned int firstCode = mortons[first];
	unsigned int lastCode = mortons[last];

	if (firstCode == lastCode)
		return (first + last) >> 1;

	int commonPrefix = CountLeadingZeros(firstCode ^ lastCode);

	int split = first;
	int step = last - first;

	do
	{
		step = (step + 1) >> 1;
		int newSplit = split + step;

		if (newSplit < last)
		{
			unsigned int splitCode = mortons[newSplit];
			int splitPrefix = CountLeadingZeros(firstCode ^ splitCode);
			if (splitPrefix > commonPrefix)
				split = newSplit;
		}
	} while (step > 1);

	return split;
}

int RayTracer::BVH::CountLeadingZeros(unsigned int i)
{
	int ret = 0;
	unsigned int temp = ~i;

	while ((temp & 0x80000000) > 0)
	{
		temp <<= 1;
		ret++;
	}
	return ret;
}

void RayTracer::BVH::Sort(std::vector<unsigned int>& mortons)
{
	QuickSort(m_primitivesForBuild, mortons, 0, mortons.size() - 1);
}

void RayTracer::BVH::QuickSort(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int low, int high)
{
	int pivot;
	if (low < high)
	{
		pivot = Partition(primitives, mortons, low, high);

		QuickSort(primitives, mortons, low, pivot - 1);
		QuickSort(primitives, mortons, pivot + 1, high);
	}
}

int RayTracer::BVH::Partition(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int low, int high)
{
	unsigned int pivotkey = mortons[low];
	while (low < high)
	{
		while (low < high && mortons[high] >= pivotkey)
			high--;
		Swap(primitives, mortons, low, high);
		while (low < high && mortons[low] <= pivotkey)
			low++;
		Swap(primitives, mortons, low, high);
	}
	return low;
}

void RayTracer::BVH::Swap(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int a, int b)
{
	auto tempData = primitives[a];
	auto tempMorton = mortons[a];
	primitives[a] = primitives[b];
	primitives[b] = tempData;
	mortons[a] = mortons[b];
	mortons[b] = tempMorton;
}

RayTracer::BVHNode::BVHNode(PrimitiveBase* primitive)
{
	m_primitive = primitive;
	m_isLeaf = true;
	primitive->GetBounds(m_bounds);
	m_left = nullptr;
	m_right = nullptr;
}

RayTracer::BVHNode::BVHNode(BVHNode* leftChild, BVHNode* rightChild)
{
	m_primitive = nullptr;
	m_isLeaf = false;
	m_left = leftChild;
	m_right = rightChild;

	Vector3 max = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
	Vector3 min = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
	if (leftChild != nullptr)
	{
		Bounds leftBounds = leftChild->GetBounds();
		Vector3 leftBoundsMin = leftBounds.GetMin();
		Vector3 leftBoundsMax = leftBounds.GetMax();

		max = Vector3::Max(max, leftBoundsMax);
		min = Vector3::Min(min, leftBoundsMin);
	}
	if (rightChild != nullptr)
	{
		Bounds rightBounds = rightChild->GetBounds();
		Vector3 rightBoundsMin = rightBounds.GetMin();
		Vector3 rightBoundsMax = rightBounds.GetMax();

		max = Vector3::Max(max, rightBoundsMax);
		min = Vector3::Min(min, rightBoundsMin);
	}

	Vector3 si = max - min;
	Vector3 ct = min + si * 0.5;

	if (si.x <= 0)
		si.x = 0.1;
	if (si.y <= 0)
		si.y = 0.1;
	if (si.z <= 0)
		si.z = 0.1;

	m_bounds = Bounds(ct, si);
}

RayTracer::BVHNode::~BVHNode()
{
}
