#pragma once

// FORWARD BEGIN
class Camera;
class PipelineState;
class Viewport;
// FORWARD END

class UpdateEventArgs : public EventArgs
{
	typedef EventArgs base;
public:
	UpdateEventArgs
    (
        const Object& caller, 
        float DeltaTime, 
        float TotalTime, 
        uint64_t FrameCounter
    )
		: base(caller)
		, ElapsedTime(DeltaTime)
		, TotalTime(TotalTime)
        , FrameCounter(FrameCounter)
	{}

	float                                           ElapsedTime;
	float                                           TotalTime;
    int64_t                                         FrameCounter;
};
typedef Delegate<UpdateEventArgs> UpdateEvent;



class RenderEventArgs : public EventArgs
{
	typedef EventArgs base;
public:
	RenderEventArgs
    (
        const Object& Caller, 
        float DeltaTime, 
        float TotalTime, 
        uint64_t FrameCounter,
        const Camera* Camera,
        const PipelineState* PipelineState,
        const Object* Node,
        const Viewport* Viewport
    )
		: base(Caller)
		, ElapsedTime(DeltaTime)
		, TotalTime(TotalTime)
		, FrameCounter(FrameCounter)

        , Camera(Camera)
        , PipelineState(PipelineState)
        , Node(Node)
        , Viewport(Viewport)
	{}

	float                                           ElapsedTime;
	float                                           TotalTime;
	int64_t                                         FrameCounter;

    const Camera*                                   Camera;
    const PipelineState*                            PipelineState;
    const Object*                                   Node;
    const Viewport*                                 Viewport;
};
typedef Delegate<RenderEventArgs> RenderEvent;
