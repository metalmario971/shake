#include "../base/Logger.h"
#include "../base/BinaryFile.h"
#include "../base/ApplicationPackage.h"
#include "../base/FileSystem.h"
#include "../base/Img32.h"
#include "../gfx/CameraNode.h"
#include "../gfx/RenderPipe.h"
#include "../gfx/TexCache.h"
#include "../gfx/LightNode.h"
#include "../gfx/ShadowBox.h"
#include "../gfx/Texture2DSpec.h"
#include "../model/Model.h"
#include "../model/ModelCache.h"
#include "../model/MeshSpec.h"
#include "../model/Material.h"
#include "../model/MbiFile.h"
#include "../world/PhysicsWorld.h"

namespace BR2 {
MbiFile::MbiFile() {
  _pFile = std::make_shared<BinaryFile>(c_strMbiVersion);
}
MbiFile::~MbiFile() {
  _pFile = nullptr;
  //DEL_MEM(_pFile);
}
void MbiFile::parseErr(string_t str, bool bDebugBreak, bool bFatal) {
  string_t strhead = Stz "Error: '" + _fileLoc + "': line " + _pFile->pos() + "\r\n  ";
  str = strhead + str;
  //Throw this if you wnt to have an error in your file.
  if (bFatal) {
    BRThrowException(str);
  }
  else {
    BRLogError(str);
    if (bDebugBreak) {
      Gu::debugBreak();
    }
  }
}
void MbiFile::postLoad() {

  //Compute Bone Boxes
  //Debug: we'll do this every time becasue FUCK!s
  BRLogInfo("  Making Bone Boxes..");
  for (std::shared_ptr<ModelSpec> ms : _vecModels) {
    ms->cacheMeshBones();
  }

}
bool MbiFile::loadAndParse(string_t file) {
  _fileLoc = file;
  std::shared_ptr<BinaryFile> fb = std::make_shared<BinaryFile>(c_strMbiVersion);
  Gu::getPackage()->getFile(file, fb, false);

  //    return false;

  fb->rewind();

  //header
  int8_t h0, h1, h2, h3;
  fb->readByte(h0);
  fb->readByte(h1);
  fb->readByte(h2);
  fb->readByte(h3);

  if (h0 != 'M' || h1 != 'B' || h2 != 'I' || h3 != 'H') {
    BRLogError("Invalid file header for MBI1 file.");
    Gu::debugBreak();
    return false;
  }

  //version
  if (!fb->readVersion()) {
    return false;
  }

  //models
  int32_t nModels;
  fb->readInt32(nModels);
  for (int32_t iModel = 0; iModel < nModels; ++iModel) {
    std::shared_ptr<ModelSpec> ms = std::make_shared<ModelSpec>();
    ms->deserialize(fb);
    _vecModels.push_back(ms);

    Gu::getModelCache()->addSpec(ms);
  }


  //Read textures
  BRLogInfo("  Loading textures..");
  std::map<Hash32, std::shared_ptr<Texture2DSpec>> texs;
  int32_t nTexs;
  fb->readInt32(nTexs);
  Img32 img;
  for (int32_t iTex = 0; iTex < nTexs; ++iTex) {
    Hash32 hTex;
    fb->readUint32(hTex);

    std::shared_ptr<Texture2DSpec> pTex = std::make_shared<Texture2DSpec>("<unset>",Gu::getCoreContext());
    pTex->deserialize(fb);
    if (Gu::getTexCache()->add(pTex->getLocation(), pTex, false) == false) {
      string_t loc = pTex->getLocation();
      //DEL_MEM(pTex);
      pTex = nullptr;
      pTex = Gu::getTexCache()->getOrLoad(TexFile(loc));
    }
    texs.insert(std::make_pair(hTex, pTex));
  }
  //Resolve textures.
  BRLogInfo("  Resolving textures..");
  for (std::shared_ptr<ModelSpec> ms : _vecModels) {
    for (std::shared_ptr<MeshSpec> mesh : ms->getMeshes()) {
      if (mesh->getMaterial() != nullptr) {
        for (std::pair<TextureChannel::e, std::shared_ptr<TextureSlot>> p : mesh->getMaterial()->getTextureSlots()) {
          std::map<Hash32, std::shared_ptr<Texture2DSpec>>::iterator it = texs.find(p.second->_iTexFileHashed);
          if (it != texs.end()) {
            p.second->_pTex = it->second;
          }
          else {
            BRLogError("Failed to find packed texture map for MBI mesh material.");
          }


        }
      }
    }
  }

  //Footer
  fb->readByte(h0);
  fb->readByte(h1);
  fb->readByte(h2);
  fb->readByte(h3);
  //if (false)
  //{
  //    for (std::shared_ptr<ModelSpec> ms : _vecModels) {
  //        for (std::shared_ptr<MeshSpec> me : ms->getMeshes()) {
  //            me->debugPrintDataToStdOut();
  //        }
  //    }

  //}

  if (h0 != 'M' || h1 != 'B' || h2 != 'I' || h3 != 'F') {
    parseErr("Invalid file footer for MBI1 file.", true, false);
    return false;
  }

  postLoad();

  return true;
}
void MbiFile::save(string_t file) {
  _fileLoc = file;
  std::shared_ptr<BinaryFile> fb = std::make_shared<BinaryFile>(c_strMbiVersion);
  fb->rewind();

  //header
  fb->writeByte('M');
  fb->writeByte('B');
  fb->writeByte('I');
  fb->writeByte('H');

  if (file.find("skele") != std::string::npos) {
    int n = 0;
    n++;
  }

  //version
  fb->writeVersion();

  //models
  fb->writeInt32((int32_t)_vecModels.size());
  for (std::shared_ptr<ModelSpec> ms : _vecModels) {
    ms->serialize(fb);
  }

  //Collect textures
  std::map<Hash32, std::shared_ptr<Texture2DSpec>> texs;
  for (std::shared_ptr<ModelSpec> ms : _vecModels) {
    for (std::shared_ptr<MeshSpec> mesh : ms->getMeshes()) {
      if (mesh->getMaterial() != nullptr) {
        for (std::pair<TextureChannel::e, std::shared_ptr<TextureSlot>> p : mesh->getMaterial()->getTextureSlots()) {
          if (p.second->_iTexFileHashed != 0) {
            texs.insert(std::make_pair(p.second->_iTexFileHashed, p.second->_pTex));
          }
        }
      }
    }
  }

  //Write textures
  fb->writeInt32(std::move((int32_t)texs.size()));
  Img32 img;
  for (std::pair<Hash32, std::shared_ptr<Texture2DSpec>> p : texs) {
    fb->writeUint32(std::move(p.first));
    p.second->serialize(fb);
  }

  //Footer
  fb->writeByte('M');
  fb->writeByte('B');
  fb->writeByte('I');
  fb->writeByte('F');

  string_t dir = FileSystem::getPathFromPath(file);
  FileSystem::createDirectoryRecursive(dir);
  fb->writeToDisk(file);

}

}//ns Game
