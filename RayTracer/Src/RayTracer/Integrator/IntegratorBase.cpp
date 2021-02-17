#include "IntegratorBase.h"
#include <thread>

//RayTracer::IntegratorBase::IntegratorBase()
//{
//	unsigned int maxThread = std::thread::hardware_concurrency();
//	m_threadCount = maxThread;
//}

RayTracer::IntegratorBase::IntegratorBase(RayTracingPreviewRenderer* previewRenderer)
{
	unsigned int maxThread = std::thread::hardware_concurrency();
	m_threadCount = maxThread;
	m_previewRenderer = previewRenderer;
}
