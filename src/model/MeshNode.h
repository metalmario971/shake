/**
*  @file MeshNode.h
*  @date March 26, 2017
*  @author MetalMario971
*/
#pragma once
#ifndef __MeshNode_1490572482630426783_H__
#define __MeshNode_1490572482630426783_H__

#include "../gfx/GfxHeader.h"
#include "../model/ModelHeader.h"
#include "../model/SceneNode.h"

namespace BR2 {
/**
*  @class MeshNode
*  @brief Basically this is MeshNode*. We carete this from tnhse meshspec class..
*/
class MeshNode : public SceneNode {
public:
  static std::shared_ptr<MeshNode> create(string_t name, bool pickable, std::shared_ptr<MeshSpec> ps, std::shared_ptr<ModelNode> mn);
  static std::shared_ptr<MeshNode> create(string_t name, bool pickable, std::shared_ptr<MeshSpec> pd);

  MeshNode(string_t name, bool pickable, std::shared_ptr<MeshSpec> ps, std::shared_ptr<ModelNode> mn);
  MeshNode(string_t name, bool pickable, std::shared_ptr<MeshSpec> pd);
  virtual ~MeshNode() override;

  virtual void init() override;
  virtual void update(float delta, std::map<Hash32, std::shared_ptr<Animator>>& pAnimator) override;

  virtual void drawDeferred(RenderParams& rp) override;
  virtual void drawTransparent(RenderParams& rp) override;
  virtual void drawForward(RenderParams& rp) override;
  virtual void drawShadow(RenderParams& rp) override;
  virtual void calcBoundBox(Box3f& __out_ pBox, const vec3& obPos, float extra_pad) override;

  std::vector<std::shared_ptr<BoneNode>>& getBoneNodesOrdered() { return _vecBoneNodesOrdered; }//Bone nodes ordered by, for each armature, Armature ID, then Bone ID 
  void printDataToStdout();
  void computeAndDispatchSkin();
  //void dispatchSkinCompute();

  std::shared_ptr<Material> getMaterial() { return _pMaterial; }
  std::shared_ptr<MeshSpec> getMeshSpec();
  uint32_t getPickId() { return _iPickId; }
  //std::shared_ptr<VaoDataGeneric> getVaoData() {return _pVaoData;}

protected:
  uint32_t _bPickable = false;
  uint32_t _iPickId = PICK_ID_NOT_SET; //Pick Id must be separate from NodeId because non-node things can be picked.
  std::shared_ptr<ModelNode> _pModelNode = nullptr;
  std::shared_ptr<Material> _pMaterial = nullptr;
  std::shared_ptr<ShaderStorageBuffer> _pArmJoints = nullptr; //Skin 12/9/2017
  std::vector<std::shared_ptr<BoneNode>> _vecBoneNodesOrdered;//Bone nodes ordered by, for each armature, Armature ID, then Bone ID 
  std::vector<std::shared_ptr<ArmatureNode>> _vecArmaturesOrdered;//Bone nodes ordered by, for each armature, Armature ID, then Bone ID 
 // std::shared_ptr<GpuComputeSync> _pSkinCompute = nullptr;
  //std::shared_ptr<VaoDataGeneric> _pVaoData = nullptr;

  virtual void afterAddedToScene(std::shared_ptr<Scene> scene) override;

  //If we wanted to be picky we could make a "modifier" class to mimic 3D editors
  void createSkin();
  void computeSkinFrame();
  void copyJointsToGpu();
  void orderBoneNodesForGpu();
  void getMeshLocalMatrix(mat4& __out_ matMesh);
  void bindSkin(std::shared_ptr<ShaderBase> shader);
  void showNoMaterialError();
  void draw(RenderParams& rp, bool bTransparent);
};

}//ns Game



#endif
