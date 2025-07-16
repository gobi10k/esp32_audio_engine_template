// ModulationEngine.h
#ifndef MODULATION_ENGINE_H
#define MODULATION_ENGINE_H

#include <vector>
#include <cmath>

// Forward declarations
class ModulationSource;

struct ModulationRoute {
    ModulationSource* source;
    float depth;
    float* targetParam;
};

class ModulationEngine {
public:
    void addSource(ModulationSource* src);
    void addRoute(ModulationSource* src, float* targetParam, float depth);
    void update(float dt);

private:
    std::vector<ModulationSource*> sources;
    std::vector<ModulationRoute> routes;
};

class ModulationSource {
public:
    virtual ~ModulationSource() {}
    virtual void update(float dt) = 0;
    virtual float getValue() const = 0;
};

// --- Implementation ---

void ModulationEngine::addSource(ModulationSource* src) {
    sources.push_back(src);
}

void ModulationEngine::addRoute(ModulationSource* src, float* targetParam, float depth) {
    routes.push_back({src, depth, targetParam});
}

void ModulationEngine::update(float dt) {
    // 1. Backup original parameter values
    std::vector<float> originalValues;
    for (auto& route : routes) {
        if (route.targetParam) {
            originalValues.push_back(*route.targetParam);
        }
    }

    // 2. Reset parameters to their base values (1.0 for multiplicative)
    for (auto& route : routes) {
        if (route.targetParam) {
            *route.targetParam = 1.0f; // Base value for multiplication
        }
    }

    // 3. Update all modulation sources
    for (auto* src : sources) {
        src->update(dt);
    }

    // 4. Apply each modulation route multiplicatively
    size_t i = 0;
    for (auto& route : routes) {
        if (route.source && route.targetParam) {
            *route.targetParam *= (route.source->getValue() * route.depth);
            i++;
        }
    }

    // 5. Restore original values multiplied by modulation
    i = 0;
    for (auto& route : routes) {
        if (route.targetParam) {
            *route.targetParam *= originalValues[i++];
        }
    }
}

#endif // MODULATION_ENGINE_H
