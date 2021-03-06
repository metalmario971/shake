#include "../base/Logger.h"
#include "../base/Hash.h"
#include "../base/GLContext.h"
#include "../gfx/ShaderAttribute.h"
#include "../gfx/ShaderBase.h"


namespace BR2 {
ShaderAttribute::ShaderAttribute(std::shared_ptr<ShaderBase> pShaderBase, int32_t attribIndex) :
  _iGLLocation(NoAttribLocationFound)
  , _strName("") {
  parseAttribute(pShaderBase, attribIndex);
}
/**
*    @fn assignLocation
*    @brief Returns a nonempty string if there was an error.
*/
void ShaderAttribute::parseAttribute(std::shared_ptr<ShaderBase> pShaderBase, int32_t attribIndex) {
  string_t err;
  static const int NBUFSIZ = 512;
  char buf[NBUFSIZ];
  GLsizei buflen;
  _strError = "";

  memset(buf, 0, 512);


  std::dynamic_pointer_cast<GLContext>(Gu::getCoreContext())->glGetActiveAttrib(
    pShaderBase->getGlId()
    , attribIndex
    , NBUFSIZ
    , &buflen
    , &_iGLAttribSize
    , &_eGLAttribType
    , buf
  );

  AssertOrThrow2(buflen < NBUFSIZ);

  _strName = string_t(buf);
  _iNameHashed = STRHASH(_strName);

  if (isOpenGLBuiltInAttrib(_strName)) {
    _bIsBuiltInAttrib = true;
  }
  else {
    _iGLLocation = std::dynamic_pointer_cast<GLContext>(Gu::getCoreContext())->glGetAttribLocation(pShaderBase->getGlId(), getName().c_str());

    _eUserType = parseUserType(_strName);

    if (_iGLLocation == NoAttribLocationFound) {
      _strError += Stz "  Error [GLSL] Attrib " + getName() + " was not found.  \r\n  \
                             1) It may have been optimized out.\r\n  \
                             2) you may have forgotten to set it in addAttr()\r\n \
                             3) the attribute may not be in the shader, so you need to remove the addAttr(). \r\n";

      BRLogWarn(err);
      Gu::debugBreak();
      // Don't throw but just exit gracefully so we can accumulate errors.
    }
  }
}
bool ShaderAttribute::isOpenGLBuiltInAttrib(string_t strName) {
  if (StringUtil::equals(strName, "gl_VertexID")) { return true; }
  else if (StringUtil::equals(strName, "gl_InstanceID")) { return true; }
  else if (StringUtil::equals(strName, "gl_DrawID")) { return true; }
  else if (StringUtil::equals(strName, "gl_BaseVertex")) { return true; }
  else if (StringUtil::equals(strName, "gl_BaseInstance")) { return true; }
  return false;
}
VertexUserType::e ShaderAttribute::parseUserType(string_t& name) {
  VertexUserType::e ret = VertexUserType::e::None;

  //Validate some errors
  string_t err = "";
  if (_strName.length() < 4) {
    err += Stz "  Invalid attribute identifier: '" + _strName + "'\r\n";
  }
  else if (_strName[0] != '_') {
    err += Stz "  Invalid attribute identifier pattern: '" + _strName + "'\r\n";
  }
  else {
    string_t strValue = _strName.substr(1, 4);
    string_t strComponent = strValue.substr(0, 1);
    string_t strVectorSize = strValue.substr(1, 1);
    string_t strIndex = strValue.substr(2, 2);

    //validate component size.
    if (_eGLAttribType == GL_FLOAT_VEC2 && !(strVectorSize == "2")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_FLOAT_VEC3 && !(strVectorSize == "3")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_FLOAT_VEC4 && !(strVectorSize == "4")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_INT_VEC2 && !(strVectorSize == "2")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_INT_VEC3 && !(strVectorSize == "3")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_INT_VEC4 && !(strVectorSize == "4")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_BOOL_VEC2 && !(strVectorSize == "2")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_BOOL_VEC3 && !(strVectorSize == "3")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_BOOL_VEC4 && !(strVectorSize == "4")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_FLOAT_MAT2 && !(strVectorSize == "2")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_FLOAT_MAT3 && !(strVectorSize == "3")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }
    else if (_eGLAttribType == GL_FLOAT_MAT4 && !(strVectorSize == "4")) { err += Stz "  Vertex component for '" + strValue + "' is invalid.\r\n"; }

    if (StringUtil::isEmpty(strVectorSize)) {
      err += Stz "  Could not match the given gl enumeration type for vertex attribute '" + _strName + "'\r\n";
    }

  }

  if (StringUtil::isNotEmpty(err)) {
    BRLogError("Attribute '" + _strName + "'parse error: \r\n" + err);
    Gu::debugBreak();
  }
  else {
    //**In the old system we had a more generic approach to this, but here we just hard code it.
    if (StringUtil::equals(name, "_v201")) {
      ret = VertexUserType::e::v2_01;
    }
    else if (StringUtil::equals(name, "_v301")) {
      ret = VertexUserType::e::v3_01;
    }
    else if (StringUtil::equals(name, "_v401")) {
      ret = VertexUserType::e::v4_01;
    }
    else if (StringUtil::equals(name, "_v402")) {
      ret = VertexUserType::e::v4_02;
    }
    else if (StringUtil::equals(name, "_v403")) {
      ret = VertexUserType::e::v4_03;
    }
    else if (StringUtil::equals(name, "_n301")) {
      ret = VertexUserType::e::n3_01;
    }
    else if (StringUtil::equals(name, "_c301")) {
      ret = VertexUserType::e::c3_01;
    }
    else if (StringUtil::equals(name, "_c401")) {
      ret = VertexUserType::e::c4_01;
    }
    else if (StringUtil::equals(name, "_x201")) {
      ret = VertexUserType::e::x2_01;
    }
    else if (StringUtil::equals(name, "_i201")) {
      ret = VertexUserType::e::i2_01;
    }
    else if (StringUtil::equals(name, "_u201")) {
      ret = VertexUserType::e::u2_01;
    }
    else {
      //Wer're going to hit this in the beginning because we can have a lot of different attrib types.
      BRLogInfo("  Unrecognized vertex attribute '" + name + "'.");
      Gu::debugBreak();
    }

  }


  return ret;
}







}//ns Game
