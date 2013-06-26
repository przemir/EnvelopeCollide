#include "GameMeshExecutor.h"
#include "Engine.h"

extern Engine& en;

template <>
EnvelopeCollide::Mesh GameMeshExecutor<SolidGame>::get(const SolidGame& data, int i) const
{
    /// Gets vertex coordinates for current moment
    /// (coordinates changes in each frames so it is important to get coordinates for current moment).
    EnvelopeCollide::Mesh source;
    if(data.type == SolidGame::TYPE_GENERIC)
    {
        source.type = EnvelopeCollide::TRIANGLE;
        source.r = 0.0f;
        const Model::ModelData& md = en.module.get(data.id)->model.getModelData();
        /// 'getCurrentPoint' returns transformed, current coordinates.
        for(int k = 0; k < source.pCount(); ++k)
            source.p[k] = en.module.get(data.id)->getCurrentPoint(md.triangles[data.nr*3+k]);
    }
    else if(data.type == SolidGame::TYPE_HARD)
    {
        source = en.module.get(data.id)->getCurrentHardShape(data.nr);
    }
    else // if(data.type == SolidGame::TYPE_SOFT)
    {
        source = en.module.get(data.id)->getCurrentSoftShape(data.nr);
    }
    return source;
}

template <>
int GameMeshExecutor<SolidGame>::count(const SolidGame& data) const
{
    return 1;
}
