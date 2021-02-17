#pragma once
#include "SceneDataBase.h"
#include <vector>

namespace RayTracer
{
	class PrimitiveBase;

	class BVHNode
	{
	public:
		BVHNode(PrimitiveBase* primitive);
		BVHNode(BVHNode* leftChild, BVHNode* rightChild);
		~BVHNode();

	public:
		bool IsLeaf() const { return m_isLeaf; }
		PrimitiveBase* GetPrimitive() const { return m_primitive; }
		BVHNode* GetLeftNode() const { return m_left; }
		BVHNode* GetRightNode() const { return m_right; }
		Bounds GetBounds() const { return m_bounds; }

	private:
		bool m_isLeaf;
		BVHNode* m_left;
		BVHNode* m_right;
		PrimitiveBase* m_primitive;
		Bounds m_bounds;
	};

	class BVH : public SceneDataBase
	{
	public:
		BVH();
		virtual ~BVH() override;

		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		
	protected:
		virtual void BuildSceneData(const Bounds& bounds) override;

	private:
		BVHNode* GenerateHierarchy(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int first, int last);
		int FindSplit(std::vector<unsigned int>& mortons, int first, int last);
		int CountLeadingZeros(unsigned int i);
		void Sort(std::vector<unsigned int>& mortons);
		void QuickSort(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int low, int high);
		int Partition(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int low, int high);
		void Swap(std::vector<PrimitiveBase*> primitives, std::vector<unsigned int>& mortons, int a, int b);

	private:
		BVHNode* m_root;
	};
};