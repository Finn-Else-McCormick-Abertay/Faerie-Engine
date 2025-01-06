#pragma once

#include <cxx.h>
#include <faerie_rust_bindings.h>
#include <vector>

namespace bridge {
    unsigned int CreateEntity();

    // This is so scuffed but I just need to get some kind of api in before the deadline so it'll have to do

    void AddTransform(unsigned int entity);
    void AddHierarchy(unsigned int entity);
    void AddCamera(unsigned int entity);

    float GetPositionX(unsigned int id);
    float GetPositionY(unsigned int id);
    float GetPositionZ(unsigned int id);
    void SetPosition(unsigned int id, float x, float y, float z);

    void HierarchyAddChild(unsigned int entity, unsigned int child);
    rust::Vec<unsigned int> HierarchyGetChildren(unsigned int id);
    rust::Vec<unsigned int> HierarchyGetDescendants(unsigned int id);

    void CameraSetActive(unsigned int id);

    void InstantiateModel(unsigned int id, rust::Str modelPath);
    void AddMaterial(unsigned int id, rust::Str materialName);
    
    void CreateMaterial(rust::Str materialName, rust::Str vertPath, rust::Str fragPath);
    void LoadModel(rust::Str modelPath);
}