#include <GL/glew.h>


struct PolygonModeConfig {
    GLenum face = GL_FRONT_AND_BACK;
    GLenum mode = GL_FILL;

    bool operator!=(const PolygonModeConfig& other) const {
        return (face != other.face) || 
        (mode != other.mode);
    }
};

struct PolygonSmoothingConfig {
    bool enabled = true;
    GLenum hint = GL_FASTEST;

    bool operator!=(const PolygonSmoothingConfig& other) const {
        return (enabled != other.enabled) || 
        (hint != other.hint);
    }
};

struct DepthConfig {
    bool enabled = true;
    GLenum function = GL_LESS;

    bool operator!=(const DepthConfig& other) const {
        return (enabled != other.enabled) || 
        (function != other.function);
    }
};

struct BlendingConfig {
    bool enabled = true;
    GLenum sourceFactor = GL_SRC_ALPHA;
    GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
    GLenum equation = GL_FUNC_ADD;

    bool operator!=(const BlendingConfig& other) const {
        return (enabled != other.enabled) || 
        (sourceFactor != other.sourceFactor) || 
        (destinationFactor != other.destinationFactor) || 
        (equation != other.equation);
    }
};

struct CullingConfig {
    bool enabled = true;
    GLenum face = GL_BACK;
    GLenum windingOrder = GL_CCW;

    bool operator!=(const CullingConfig& other) const {
        return (enabled != other.enabled) || 
        (face != other.face) || 
        (windingOrder != other.windingOrder);
    }
};


struct RenderConfig {
    PolygonModeConfig polygonMode;
    PolygonSmoothingConfig polygonSmoothing;
    DepthConfig depth;
    BlendingConfig blending;
    CullingConfig culling;

    bool operator!=(const RenderConfig& other) const {
        return (polygonMode != other.polygonMode) || 
        (polygonSmoothing != other.polygonSmoothing) || 
        (depth != other.depth) || 
        (blending != other.blending) || 
        (culling != other.culling);
    }
};