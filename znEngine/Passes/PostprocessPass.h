#pragma once

#include "BasePass.h"

class Texture;

class PostprocessPass : public BasePass
{
public:
    typedef BasePass base;

    PostprocessPass( std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline, const glm::mat4& projectionMatrix, std::shared_ptr<Texture> texture );

    // Render the pass. This should only be called by the RenderTechnique.
    virtual void Render( Render3DEventArgs& e );

    virtual bool Visit( SceneNode3D& node );

protected:

private:
    glm::mat4 m_ProjectionMatrix;
    std::shared_ptr<Texture> m_Texture;
};