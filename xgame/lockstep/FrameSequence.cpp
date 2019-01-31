
#include "FrameSequence.h"
#include "Frame.h"

#include <assert.h>

FrameSequence::~FrameSequence()
{
	for (auto it : m_Frames)
	{
		delete it.second;
	}
	m_Frames.clear();
}

bool FrameSequence::AddFrame(Frame* pFrame)
{
	auto it = m_Frames.insert(std::make_pair(pFrame->GetFrameID(), pFrame));
	assert(it.second && "frame id already existed");
	return it.second;
}

Frame* FrameSequence::GetFrame(int32_t nFrameID)
{
	auto it = m_Frames.find(nFrameID);
	if (it != m_Frames.end())
	{
		return it->second;
	}
	return nullptr;
}

uint32_t FrameSequence::GetFrameSize()
{
	return m_Frames.size();
}
