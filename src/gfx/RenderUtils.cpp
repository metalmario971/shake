#include "../base/Base.h"
#include "../base/Img32.h"

#include "../gfx/FrustumBase.h"
#include "../gfx/RenderUtils.h"
#include "../gfx/ShaderBase.h"
#include "../gfx/ShaderMaker.h"
#include "../gfx/ShaderUniform.h"
#include "../model/UtilMeshGrid.h"
#include "../model/UtilMeshAxis.h"
#include "../model/UtilMeshSphere.h"
#include "../model/UtilMeshBox.h"
#include "../model/UtilMeshPickBox.h"
#include "../model/UtilMeshInline.h"

namespace Game {

void RenderUtils::setLineWidth(float width)
{
    //This is deprecated. so, idk.
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->setLineWidth(width);
    //This is deprecated..?
   // Gd::setLineWidth(width);
}
void RenderUtils::resetRenderState()
{
    RenderUtils::debugGetRenderState();
    {
        Gu::getShaderMaker()->shaderBound(nullptr);

        //glUseProgram(NULL);//DO NOT CALL - we must maintain consistency on the gpu driver
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, NULL);

        int iMaxTextures = 0;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&iMaxTextures);
        for (int iTex = 0; iTex < iMaxTextures; iTex++)
        {
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glActiveTexture(GL_TEXTURE0 + iTex);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //  VaoData::debugDisableAllAttribArrays();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindVertexArray(0);
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, 0);
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glActiveTexture(GL_TEXTURE0);

        //Note: Client textures are deprecated
      //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      //  glDisableClientState(GL_COLOR_ARRAY);
      //  glDisableClientState(GL_NORMAL_ARRAY);
      //  glDisableClientState(GL_VERTEX_ARRAY);
      //  glDisableClientState(GL_INDEX_ARRAY);
      //  glClientActiveTexture(GL_TEXTURE0);
        glDisable(GL_LIGHTING);

    }
    RenderUtils::debugGetRenderState();

}

#pragma region Debug Draw
//UNSAFE
void RenderUtils::renderTexturedQuadAttrib(float size) {
    int attr_v = 0;
    int attr_c = 1;
    int attr_n = 2;
    int attr_t = 3;
    //UNSAFE
    static const float s = 100;
    static const GLfloat _vertexes[] = {
        0,0,0,
        0,0,s,
        s,0,0,
        s,0,s
    };
    static const GLfloat _colors[] = {
        1,1,1,1,
        1,1,1,1,
        1,1,1,1,
        1,1,1,1
    };
    static const GLfloat _normals[] = {
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0
    };
    static const GLfloat _tcoords[] = {
        0,1,
        0,0,
        1,1,
        1,0
    };
    static const GLushort _indexes[] = {
        0,1,2,
        2,1,3,
    };

    static const int _nIndexes = 6;

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glActiveTexture(GL_TEXTURE0);

    GLuint bdVerts,
        bdColors,
        bdTextures,
        bdNormals,
        bdIndexes,
        vaoIndexes;

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenVertexArrays(1, (GLuint*)&vaoIndexes);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindVertexArray(vaoIndexes);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenBuffers(1, (GLuint*)&bdVerts);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenBuffers(1, (GLuint*)&bdColors);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenBuffers(1, (GLuint*)&bdTextures);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenBuffers(1, (GLuint*)&bdNormals);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGenBuffers(1, (GLuint*)&bdIndexes);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdVerts);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBufferData(GL_ARRAY_BUFFER,
        4 * sizeof(GLfloat) * 3,
        _vertexes,
        GL_STATIC_DRAW);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdColors);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBufferData(GL_ARRAY_BUFFER,
        4 * sizeof(GLfloat) * 4,
        _colors,
        GL_STATIC_DRAW);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdNormals);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBufferData(GL_ARRAY_BUFFER,
        4 * sizeof(GLfloat) * 3,
        _normals,
        GL_STATIC_DRAW);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdTextures);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBufferData(GL_ARRAY_BUFFER,
        4 * sizeof(GLfloat) * 2,
        _tcoords,
        GL_STATIC_DRAW);

    //Hmm...
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bdIndexes);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        6 * sizeof(GL_UNSIGNED_SHORT),
        _indexes,
        GL_STATIC_DRAW);


    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,NULL);
    //glBindBuffer(GL_ARRAY_BUFFER,NULL);

    // MUST BIND BUFFER DATA TO USE VERTEX ATTRIBS
    // MUST USE VBOS WITH VERTEX ATTRIBS


    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glEnableVertexAttribArray(attr_v);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdVerts);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glVertexAttribPointer(attr_v,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat) * 3,
        NULL);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glEnableVertexAttribArray(attr_c);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdColors);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glVertexAttribPointer(attr_c,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat) * 4,
        NULL);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glEnableVertexAttribArray(attr_n);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdNormals);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glVertexAttribPointer(attr_n,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat) * 3,
        NULL);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glEnableVertexAttribArray(attr_t);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, bdTextures);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glVertexAttribPointer(attr_t,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat) * 2,
        NULL);
    // CheckGpuErrorsDbg();

         //std::cout<<_vertexes<<std::endl;
         //std::cout<<_colors<<std::endl;
         //std::cout<<_normals<<std::endl;
         //std::cout<<_tcoords<<std::endl;
         //std::cout<<_indexes<<std::endl;

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bdIndexes);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindVertexArray(vaoIndexes);
    glDrawElements(GL_TRIANGLES, _nIndexes, GL_UNSIGNED_SHORT, NULL);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindBuffer(GL_ARRAY_BUFFER, NULL);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteBuffers(1, (GLuint*)&bdVerts);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteBuffers(1, (GLuint*)&bdColors);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteBuffers(1, (GLuint*)&bdTextures);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteBuffers(1, (GLuint*)&bdNormals);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteBuffers(1, (GLuint*)&bdIndexes);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glDeleteVertexArrays(1, (GLuint*)&vaoIndexes);
}
void RenderUtils::drawAxisShader(float scale, float lineWidth, mat4& transform)
{
    UtilMeshAxis* ax = new UtilMeshAxis(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), scale, lineWidth, transform);
    ax->init();
    ax->draw();
    delete ax;
}
//void RenderUtils::drawWireSphereShader(float fRadius, vec3& vOffset, vec4& vColor, int32_t nSlices, int32_t nStacks)
//{
//    UtilMeshSphere* ax = new UtilMeshSphere(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), fRadius, vOffset, vColor, nSlices, nStacks);
//    ax->init();
//    ax->draw();
//    delete ax;
//}
void RenderUtils::drawWireSphereShader(float fRadius, vec4& vColor, int32_t nSlices, int32_t nStacks, mat4* pMatrix)
{
    UtilMeshSphere* ax = new UtilMeshSphere(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), fRadius, vec3(0, 0, 0), vColor, nSlices, nStacks);
    ax->init();
    if (pMatrix != nullptr) {
        ax->setModelMatrix(*pMatrix);
    }
    ax->draw();
    delete ax;
}
void RenderUtils::drawWireBoxShader(Box3f* box, vec3& vOffset, vec4& vColor)
{
    UtilMeshBox* ax = new UtilMeshBox(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), box, vOffset, vColor);
    ax->init();
    ax->draw();
    delete ax;
}
void RenderUtils::drawSolidBoxShaded(Box3f* box, vec3& vOffset, vec4& vColor)
{
    UtilMeshBox* ax = new UtilMeshBox(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), box, vOffset, vColor);
    ax->setWireFrame(false);
    ax->init();
    ax->draw();
    delete ax;
}
//void RenderUtils::drawPickBox(Box3f* box, uint32_t uiColorId)
//{
//    UtilMeshPickBox* ax = new UtilMeshPickBox(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), box, uiColorId);
//    ax->init();
//    ax->draw();
//    delete ax;
//}
//void RenderUtils::inlineBegin(GLenum type, UtilMeshInline& __out_ mesh)
//{
//    //mesh = new UtilMeshInline();
//    mesh.begin(type);
//}
//void RenderUtils::inlineVec3(vec3& v, UtilMeshInline& __out_ mesh)
//{
//    mesh.v3f1(v);
//}
//void RenderUtils::inlineEnd(UtilMeshInline& __out_ mesh)
//{
//    mesh.end();
//}
void RenderUtils::drawGridShader(float r, float g, float b, int32_t nSlices, float fSliceWidth, vec3& center, std::shared_ptr<ShaderBase> pShader)
{
    UtilMeshGrid* pGrid = new UtilMeshGrid(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()), r, g, b, nSlices, fSliceWidth, center);
    pGrid->init();
    pGrid->draw();
    delete pGrid;
}
void RenderUtils::drawFrustumShader(std::shared_ptr<FrustumBase> pf, vec4& avColor)
{
    setLineWidth(3.0f);

    UtilMeshInline mi(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()));
    Color4f c4 = avColor;
    mi.begin(GL_LINES);
    {
        mi.vt2(v_v3c4(pf->PointAt(fpt_ntl), c4), v_v3c4(pf->PointAt(fpt_ftl), c4));// Edges
        mi.vt2(v_v3c4(pf->PointAt(fpt_nbr), c4), v_v3c4(pf->PointAt(fpt_fbr), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_nbl), c4), v_v3c4(pf->PointAt(fpt_fbl), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ntr), c4), v_v3c4(pf->PointAt(fpt_ftr), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ftl), c4), v_v3c4(pf->PointAt(fpt_ftr), c4));// Far Quad
        mi.vt2(v_v3c4(pf->PointAt(fpt_fbl), c4), v_v3c4(pf->PointAt(fpt_fbr), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ftl), c4), v_v3c4(pf->PointAt(fpt_fbl), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ftr), c4), v_v3c4(pf->PointAt(fpt_fbr), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ntl), c4), v_v3c4(pf->PointAt(fpt_ntr), c4));// Near Quad
        mi.vt2(v_v3c4(pf->PointAt(fpt_nbl), c4), v_v3c4(pf->PointAt(fpt_nbr), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ntl), c4), v_v3c4(pf->PointAt(fpt_nbl), c4));
        mi.vt2(v_v3c4(pf->PointAt(fpt_ntr), c4), v_v3c4(pf->PointAt(fpt_nbr), c4));
    }
    mi.end();
    setLineWidth(1.0f);

}
#pragma endregion




#pragma region Debug Output


void appendLine(t_string& st, char* c) {
    st += c;
    st += "\r\n";
}
void appendLine(t_string& st, t_string& c) {
    st += c;
    st += "\r\n";
}


t_string RenderUtils::debugGetRenderState(bool bForceRun, bool bPrintToStdout, bool bSaveFramebufferTexture)
{
    //This method is called in frames to drag down the debug arrow
    // and we skip it unless we force it to run.
    //Do not comment
    if (!bForceRun) {
        return "";//Do not comment
    }
    t_string strState = "";

    // Gd::verifyRenderThread();//We must be in render thread

    appendLine(strState, "===================================================================");
    appendLine(strState, "  RENDERING SYSTEM STATE                                            ");
    appendLine(strState, "===================================================================");

    GLint iBlend;
    glGetIntegerv(GL_BLEND, (GLint*)&iBlend);
    appendLine(strState, Stz " Blending:" + (iBlend ? "ENABLED" : "DISABLED"));

    GLint iCullFace;
    glGetIntegerv(GL_CULL_FACE, (GLint*)&iCullFace);
    appendLine(strState, Stz " Culling:" + (iCullFace ? "ENABLED" : "DISABLED"));

    GLint iDepthTest;
    glGetIntegerv(GL_DEPTH_TEST, (GLint*)&iDepthTest);
    appendLine(strState, Stz " Depth Test:" + (iDepthTest ? "ENABLED" : "DISABLED"));


    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    RenderUtils::debugGetLegacyViewAndMatrixStack(strState);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    RenderUtils::debugGetBufferState(strState);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    //RenderUtils::debugGetAttribState(); // This is redundant with vertexarraystate
    //    CheckGpuErrorsDbg();
    RenderUtils::debugGetTextureState(strState);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    RenderUtils::debugGetVertexArrayState(strState);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    RenderUtils::debugGetFramebufferAttachmentState(strState);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    if (bPrintToStdout) {
        Gu::print(strState);
    }
    if (bSaveFramebufferTexture) {
        t_string fname = FileSystem::getScreenshotFilename();
        saveFramebufferAsPng(std::move(fname));
    }

    return strState;
}
void RenderUtils::debugGetLegacyViewAndMatrixStack(t_string& strState)
{
    GLint iScissorBox[4];
    GLint iViewportBox[4];
    appendLine(strState, "**Scissor and Legcay info**");
    //View Bounds (Legacy)
    glGetIntegerv(GL_SCISSOR_BOX, (GLint*)iScissorBox);
    glGetIntegerv(GL_VIEWPORT, (GLint*)iViewportBox);
    appendLine(strState, Stz "Scissor: " + iScissorBox[0] + "," + iScissorBox[1] + "," + iScissorBox[2] + "," + iScissorBox[3]);
    appendLine(strState, Stz "Viewport: " + iViewportBox[0] + "," + iViewportBox[1] + "," + iViewportBox[2] + "," + iViewportBox[3]);
    //TODO: legacy matrix array state.
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
}
void RenderUtils::debugGetBufferState(t_string& strState)
{
    appendLine(strState, "--------------------------------------");
    appendLine(strState, "--SHADER STATE");

    GLint iBoundBuffer;
    GLint iCurrentProgram;
    GLint iElementArrayBufferBinding;
    GLint iSsboBinding;    //shader storage
    GLint iUniformBufferBinding;
    GLint iVertexArrayBinding;

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &iBoundBuffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &iElementArrayBufferBinding);
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &iSsboBinding);
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &iUniformBufferBinding);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &iVertexArrayBinding);
    glGetIntegerv(GL_CURRENT_PROGRAM, &iCurrentProgram);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    appendLine(strState, Stz "Bound Vertex Array Buffer Id (VBO):" + iBoundBuffer);
    appendLine(strState, Stz "Bound Element Array Buffer Id (IBO):" + iElementArrayBufferBinding);
    appendLine(strState, Stz "Bound Shader Storage Buffer Id (SSBO):" + iSsboBinding);
    appendLine(strState, Stz "Bound Uniform Buffer Object Id:" + iUniformBufferBinding);
    appendLine(strState, Stz "Bound Vertex Array Object Id:" + iVertexArrayBinding);
    appendLine(strState, Stz "Bound Shader Program Id:" + iCurrentProgram);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    if (Gu::getShaderMaker()->getBound() != nullptr) {
        appendLine(strState, Stz "Bound Shader Name:" +
            Gu::getShaderMaker()->getBound()->getProgramName());
        RenderUtils::debugPrintActiveUniforms(Gu::getShaderMaker()->getBound()->getGlId(), strState);
        // appendLine(strState, Gu::getShaderMaker()->getBound()->debugGetUniformValues());

    }
    else {
        appendLine(strState, "Bound Shader Name: None");
    }


}
void RenderUtils::debugPrintActiveUniforms(int iGlProgramId, t_string& strState)
{
    GLint nUniforms;
    t_string uniformName;
    char name[256];
    GLint name_len = -1;
    GLint iArraySize = -1;
    GLenum uniformType = GL_ZERO;
    GLint nActiveUniformBlocks;
    GLint nMaxUniformLocations;
    GLint nMaxComponentsVert;
    GLint nMaxComponentsFrag;
    GLint nMaxComponentsComp;

    // - Get the number of uniforms
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetProgramiv(iGlProgramId, GL_ACTIVE_UNIFORMS, &nUniforms);
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetProgramiv(iGlProgramId, GL_ACTIVE_UNIFORM_BLOCKS, (GLint*)&nActiveUniformBlocks);
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, (GLint*)&nMaxUniformLocations);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, (GLint*)&nMaxComponentsVert);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, (GLint*)&nMaxComponentsFrag);
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, (GLint*)&nMaxComponentsComp);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    appendLine(strState, Stz "# Active Uniforms: " + nUniforms);
    appendLine(strState, Stz "# Active Uniform Blocks: " + nActiveUniformBlocks);
    appendLine(strState, Stz "# Max Uniform Locations: " + nMaxUniformLocations);
    appendLine(strState, Stz "# Max Uniform Components Vertex: " + nMaxComponentsVert);
    appendLine(strState, Stz "# Max Uniform Components Fragment: " + nMaxComponentsFrag);
    appendLine(strState, Stz "# Max Uniform Components Compute: " + nMaxComponentsComp);

    appendLine(strState, "--------------------------------------");
    appendLine(strState, "-- Active Uniform Data: ");

    //Get all uniform names and types into a list.
    for (int32_t i = 0; i < nUniforms; ++i)
    {
        //Get name an d type
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniform(iGlProgramId, (GLuint)i, 256, &name_len, &iArraySize, &uniformType, (char*)name);
        name[name_len] = 0;
        uniformName = t_string(name);

        //get location
        GLint glLocation = std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetUniformLocation((GLuint)iGlProgramId, (GLchar*)uniformName.c_str());

        appendLine(strState, Stz "  Name: " + uniformName);
        appendLine(strState, Stz "  Type: " + RenderUtils::openGlTypeToString(uniformType));
        appendLine(strState, Stz "  Location: " + glLocation);
        appendLine(strState, Stz "  Array Size: " + iArraySize);

        // Uniform Block Data.
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

        GLuint iCurrentBlockIdx;
        iCurrentBlockIdx = std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetUniformBlockIndex(iGlProgramId, uniformName.c_str());

        if (iCurrentBlockIdx != GL_INVALID_INDEX)
        {
            int iBlockBinding;
            int iBlockDataSize;
            int iBlockNameLength;
            int iBlockActiveUniforms;
            int iBlockActiveUniformIndices;

            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniformBlockiv(iGlProgramId, iCurrentBlockIdx, GL_UNIFORM_BLOCK_BINDING, (GLint*)&iBlockBinding);
            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniformBlockiv(iGlProgramId, iCurrentBlockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&iBlockDataSize);
            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniformBlockiv(iGlProgramId, iCurrentBlockIdx, GL_UNIFORM_BLOCK_NAME_LENGTH, (GLint*)&iBlockNameLength);
            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniformBlockiv(iGlProgramId, iCurrentBlockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, (GLint*)&iBlockActiveUniforms);
            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetActiveUniformBlockiv(iGlProgramId, iCurrentBlockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)&iBlockActiveUniformIndices);
            std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

            appendLine(strState, Stz "  Block Index:" + iCurrentBlockIdx);
            appendLine(strState, Stz "  Block Binding:" + iBlockBinding);
            appendLine(strState, Stz "  Block Data Size:" + iBlockDataSize);
            appendLine(strState, Stz "  Block Name Length:" + iBlockNameLength);
            appendLine(strState, Stz "  Block Active Uniforms:" + iBlockActiveUniforms);
            appendLine(strState, Stz "  Block Active Uniform Indices:" + iBlockActiveUniformIndices);
        }

        //Data
        if (Gu::getShaderMaker()->getBound() != nullptr) {

            std::shared_ptr<ShaderUniform> uf = Gu::getShaderMaker()->getBound()->getUniformByName(uniformName);
            if (uf != nullptr) {
                appendLine(strState, ("  Buffer Data:"));
                if (uf->hasBeenSet() == false) {
                    appendLine(strState, ("  not set."));
                }
                else {
                    appendLine(strState, (uf->debugGetUniformValueAsString()));
                }
            }
            else {
                appendLine(strState, Stz "Uniform " + uniformName + " was not found.  It may be a uniform buffer.");
            }
        }
        appendLine(strState, (""));
    }
}
t_string RenderUtils::openGlTypeToString(GLenum type)
{
    switch (type) {
    case GL_UNSIGNED_INT: return ("GL_UNSIGNED_INT        "); break;
    case GL_UNSIGNED_INT_VEC2: return ("GL_UNSIGNED_INT_VEC2 "); break;
    case GL_UNSIGNED_INT_VEC3: return ("GL_UNSIGNED_INT_VEC3 "); break;
    case GL_UNSIGNED_INT_VEC4: return ("GL_UNSIGNED_INT_VEC4 "); break;
    case GL_FLOAT: return ("GL_FLOAT                "); break;
    case GL_FLOAT_VEC2: return ("GL_FLOAT_VEC2         "); break;
    case GL_FLOAT_VEC3: return ("GL_FLOAT_VEC3         "); break;
    case GL_FLOAT_VEC4: return ("GL_FLOAT_VEC4         "); break;
    case GL_INT: return ("GL_INT                "); break;
    case GL_INT_VEC2: return ("GL_INT_VEC2             "); break;
    case GL_INT_VEC3: return ("GL_INT_VEC3             "); break;
    case GL_INT_VEC4: return ("GL_INT_VEC4             "); break;
    case GL_BOOL: return ("GL_BOOL                 "); break;
    case GL_BOOL_VEC2: return ("GL_BOOL_VEC2         "); break;
    case GL_BOOL_VEC3: return ("GL_BOOL_VEC3         "); break;
    case GL_BOOL_VEC4: return ("GL_BOOL_VEC4         "); break;
    case GL_FLOAT_MAT2: return ("GL_FLOAT_MAT2         "); break;
    case GL_FLOAT_MAT3: return ("GL_FLOAT_MAT3         "); break;
    case GL_FLOAT_MAT4: return ("GL_FLOAT_MAT4         "); break;
    case GL_SAMPLER_1D: return ("GL_SAMPLER_1D         "); break;
    case GL_SAMPLER_2D: return ("GL_SAMPLER_2D         "); break;
    case GL_SAMPLER_3D: return ("GL_SAMPLER_3D         "); break;
    case GL_SAMPLER_CUBE: return ("GL_SAMPLER_CUBE         "); break;
    case GL_SAMPLER_1D_SHADOW: return ("GL_SAMPLER_1D_SHADOW "); break;
    case GL_SAMPLER_2D_SHADOW: return ("GL_SAMPLER_2D_SHADOW "); break;
    case GL_2_BYTES: return ("GL_2_BYTES            "); break;
    case GL_3_BYTES: return ("GL_3_BYTES            "); break;
    case GL_4_BYTES: return ("GL_4_BYTES            "); break;
    case GL_UNSIGNED_SHORT: return ("GL_UNSIGNED_SHORT    "); break;
    case GL_SHORT: return ("GL_SHORT                "); break;
    case GL_DOUBLE: return ("GL_DOUBLE            "); break;
    }
    return Stz "Undefined " + (int)type;
}
void RenderUtils::debugGetAttribState(t_string& strState)
{
    //// - print bound attributes
    //int iMaxAttribs;
    //int iBoundAttrib;
    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&iMaxAttribs);
    //std::cout<<"Attribs: max count = "<<iMaxAttribs<<std::endl;
    //for(int xx=0; xx<iMaxAttribs; ++xx)
    //{
    //    glGetIntegerv(GL_VERTEX_ATTRIB_ARRAY0_NV+xx,&iBoundAttrib);
    //    std::cout<<"attrib "<<xx<<": "<<iBoundAttrib<<std::endl;
    //}
}
void RenderUtils::debugGetTextureState(t_string& strState)
{
    appendLine(strState, ("----------------------------------"));
    appendLine(strState, ("--TEXTURE STATE"));
    GLint iActiveTexture;
    GLint iTextureBindingBuffer;
    GLint iTextureBinding;    //Texture ID
    GLint iMaxVertexTextureUnits;
    GLint iMaxTextureUnits;
    GLint iMaxCombinedTextureUnits;

    glGetIntegerv(GL_ACTIVE_TEXTURE, &iActiveTexture);//Texture ID    0x84C0 is TEXTURE0
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &iTextureBinding);//Texture ID    
    glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &iTextureBindingBuffer);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &iMaxVertexTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &iMaxTextureUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &iMaxCombinedTextureUnits);


    appendLine(strState, Stz  "Active Texture ID: " + iTextureBinding);
    appendLine(strState, Stz  "Active Texture Buffer ID: " + iTextureBindingBuffer);
    appendLine(strState, Stz  "Current active texture: " + "GL_TEXTURE" + (iActiveTexture - 0x84C0));
    appendLine(strState, Stz  "Max Texture Units: " + iMaxTextureUnits);
    appendLine(strState, Stz  "Max Vertex Texture Units: " + iMaxVertexTextureUnits);
    appendLine(strState, Stz  "Max Combined Texture Units: " + iMaxCombinedTextureUnits);
    appendLine(strState, Stz  "Below are the bound textures Per Texture Channel:");
    // - Get bound texture units.
    for (int i = 0; i < iMaxVertexTextureUnits; ++i)
    {
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glActiveTexture(GL_TEXTURE0 + i);
        appendLine(strState, Stz "  Channel " + i);
        glGetIntegerv(GL_TEXTURE_BINDING_1D, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz "     1D: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz "     1D_ARRAY: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     2D: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     2D_ARRAY: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     2D_MULTISAMPLE: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     2D_MULTISAMPLE_ARRAY: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_3D, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     3D: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     BUFFER: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     CUBE_MAP: " + (int)iTextureBinding);
        iTextureBinding = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &iTextureBinding); if (iTextureBinding > 0) appendLine(strState, Stz"     RECTANGLE: " + (int)iTextureBinding);
    }
}
void RenderUtils::debugGetFramebufferAttachmentState(t_string& strState)
{
    appendLine(strState, "-----------------------------------");
    appendLine(strState, "--FRAMEBUFFERS");
    GLenum eDrawBuffer;
    GLint iDrawFramebufferBinding;//name of fb beijmg drawn to
    GLint iReadFramebufferBinding;//name of fb beijmg drawn to
    GLint iRenderbufferBinding;
    GLenum eReadBuffer;
    GLint iSamplerBinding;    //!Texture sampler (should be 2d??)
    GLint maxColorAttachments = 0;
    GLint boundFramebuffer;
    //Reference enums
    //#define GL_FRONT 0x0404
    //#define GL_BACK 0x0405

    // Framebuffers
    glGetIntegerv(GL_DRAW_BUFFER, (GLint*)&eDrawBuffer);//0x08CE0 is the COLOR ATTACHMENT 1, 0x0405 is the default BACK buffer.
    glGetIntegerv(GL_READ_BUFFER, (GLint*)&eReadBuffer);//Default: GL_BACK
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &iDrawFramebufferBinding);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &iReadFramebufferBinding);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &iRenderbufferBinding);
    glGetIntegerv(GL_SAMPLER_BINDING, &iSamplerBinding);
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer);

    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    appendLine(strState, Stz " # Max Color Attachments: " + maxColorAttachments);
    appendLine(strState, Stz " Current Bound Framebuffer: " + boundFramebuffer);
    appendLine(strState, Stz " Current Draw Framebuffer (glDrawBuffer): " + iDrawFramebufferBinding);
    appendLine(strState, Stz " Current Read Framebuffer (glReadBuffer): " + iReadFramebufferBinding);
    if (iDrawFramebufferBinding != iReadFramebufferBinding) {
        appendLine(strState, "   NOTE: Draw and Read framebuffers are bound different!");
    }
    appendLine(strState, Stz " Current RenderBuffer Binding: " + iRenderbufferBinding);
    appendLine(strState, Stz " Read Buffer Enum: " + eReadBuffer);
    appendLine(strState, Stz " Current Sampler Binding: " + iSamplerBinding);

    if (boundFramebuffer == 0) {
        return;
    }

    // Print details about hte bound buffer.
    for (int i = 0; i < maxColorAttachments; ++i)
    {
        RenderUtils::debugPrintFBOAttachment(strState, GL_COLOR_ATTACHMENT0 + i);
    }
    RenderUtils::debugPrintFBOAttachment(strState, GL_DEPTH_ATTACHMENT);
    RenderUtils::debugPrintFBOAttachment(strState, GL_STENCIL_ATTACHMENT);

}
void RenderUtils::debugPrintFBOAttachment(t_string& strState, GLenum attachment) {

    GLint attachmentName = 0;
    GLint attachmentType = 0;
    GLint mipmapLevel = 0;

    t_string strAttachment;
    if (attachment == GL_DEPTH_ATTACHMENT) {
        strAttachment = ("GL_DEPTH_ATTACHMENT");
    }
    else if (attachment == GL_STENCIL_ATTACHMENT) {
        strAttachment = ("GL_STENCIL_ATTACHMENT");
    }
    else if (attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT15) {
        strAttachment = Stz "GL_COLOR_ATTACHMENT" + (attachment - GL_COLOR_ATTACHMENT0);
    }

    appendLine(strState, Stz "  Attachment: " + strAttachment);

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachmentType);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    if (attachmentType == GL_NONE)
    {
        appendLine(strState, Stz "    Type: " + "GL_NONE");
    }
    else if (attachmentType == GL_RENDERBUFFER)
    {
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attachmentName);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        appendLine(strState, Stz "    Type: " + "GL_RENDERBUFFER");
        appendLine(strState, Stz "    Name: " + attachmentName);

    }
    else if (attachmentType == GL_TEXTURE)
    {
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attachmentName);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, &mipmapLevel);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        appendLine(strState, Stz "    Type: " + "GL_TEXTURE");
        appendLine(strState, Stz "    Name: " + attachmentName);
        appendLine(strState, Stz "    Mipmap Level: " + mipmapLevel);
    }
}
void RenderUtils::debugGetVertexArrayState(t_string& strState)
{
    appendLine(strState, ("----------------------------------------"));
    appendLine(strState, ("--VERTEX ARRAY STATE"));
    int nMaxAttribs;
    GLint iVertexArrayBinding;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nMaxAttribs);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &iVertexArrayBinding);

    appendLine(strState, Stz "# Bound Vertex Array Id (VAO):" + iVertexArrayBinding);
    appendLine(strState, Stz "# Max Allowed Atribs:" + nMaxAttribs);
    appendLine(strState, Stz("---------------------------------------"));
    appendLine(strState, Stz("--Active Vertex Attribs: "));

    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    // - Disable all arrays by default.
    for (int iAttrib = 0; iAttrib < nMaxAttribs; ++iAttrib) {
        //TODO:
        size_t iArrayBufferBinding = 0;
        GLint iArrayEnabled = 0;
        GLint iAttribArraySize = 0;
        GLenum iAttribArrayType = 0;
        GLint iAttribArrayStride = 0;
        GLint iAttribArrayInteger = 0;
        GLfloat fCurAttrib[4];
        GLint iCurAttrib[4];
        GLuint uiCurAttrib[4];
        GLint iAttribArrayNormalized;
        //GLint iAttribArrayDivisor;
        memset(fCurAttrib, 0, sizeof(GLfloat) * 4);
        memset(iCurAttrib, 0, sizeof(GLint) * 4);
        memset(uiCurAttrib, 0, sizeof(GLuint) * 4);

        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&iArrayBufferBinding);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_ENABLED, (GLint*)&iArrayEnabled);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_SIZE, (GLint*)&iAttribArraySize);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*)&iAttribArrayType);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_STRIDE, (GLint*)&iAttribArrayStride);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_INTEGER, (GLint*)&iAttribArrayInteger);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

        std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, (GLint*)&iAttribArrayNormalized);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        //glGetVertexAttribiv(iAttrib, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, (GLint*)&iAttribArrayDivisor);
        //CheckGpuErrorsDbg();

        appendLine(strState, Stz "Attrib " + iAttrib + "  Enabled:" + (iArrayEnabled ? "Y" : "N"));

        if (!iArrayEnabled) {
            continue;
        }

        appendLine(strState, Stz "  ArrayBufferBinding:" + iArrayBufferBinding);
        appendLine(strState, Stz "  Size:" + iAttribArraySize);
        appendLine(strState, Stz "  Stride:" + iAttribArrayStride);
        appendLine(strState, Stz "  Is Integer:" + (iAttribArrayInteger ? "Y" : "N"));
        appendLine(strState, Stz "  Normalized:" + (iAttribArrayNormalized ? "Y" : "N"));
        appendLine(strState, Stz "  Type:" + RenderUtils::openGlTypeToString(iAttribArrayType));

        if (iAttrib != 0) {
            //Generic vertex attribute 0 is unique in that it has no current state,
            //so an error will be generated if index is 0. The initial value for all
            //other generic vertex attributes is (0,0,0,1).
            switch (iAttribArrayType)
            {
            case GL_INT:
                std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribIiv(iAttrib, GL_CURRENT_VERTEX_ATTRIB, (GLint*)&iCurAttrib);
                std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
                appendLine(strState, Stz "  Cur Value: " + iCurAttrib[0] + "," + iCurAttrib[1] + "," + iCurAttrib[2] + "," + iCurAttrib[3]);
                break;
            case GL_UNSIGNED_INT:
                std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribIuiv(iAttrib, GL_CURRENT_VERTEX_ATTRIB, (GLuint*)&uiCurAttrib);
                std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
                appendLine(strState, Stz "  Cur Value: " + uiCurAttrib[0] + "," + uiCurAttrib[1] + "," + uiCurAttrib[2] + "," + uiCurAttrib[3]);
                break;
            case GL_FLOAT:
                std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glGetVertexAttribfv(iAttrib, GL_CURRENT_VERTEX_ATTRIB, (GLfloat*)&iCurAttrib);
                std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
                appendLine(strState, Stz "  Cur Value: " + fCurAttrib[0] + "," + fCurAttrib[1] + "," + fCurAttrib[2] + "," + fCurAttrib[3]);
                break;
            default:
                appendLine(strState, "  TODO:Cur Value: ");
                break;
            };
        }
        //This reads the attrib values such as float, int etc.
        //GLint iCurrentVertexAttrib;
        //glGetVertexAttribiv(iAttrib, GL_CURRENT_VERTEX_ATTRIB, (GLint*)&iCurrentVertexAttrib);
    }
}
#pragma endregion


void RenderUtils::saveTexture(t_string&& strLoc, GLuint iGLTexId, GLenum eTexTarget, int iCubeMapSide) {
    std::shared_ptr<Img32> bi = std::make_shared<Img32>();
    if (RenderUtils::getTextureDataFromGpu(bi, iGLTexId, eTexTarget, iCubeMapSide) == true) {
        //the GL tex image must be flipped to show upriht/
        bi->flipV();
        Gu::saveImage(strLoc, bi);
        Gu::checkErrorsRt();
    }
}
void RenderUtils::saveFramebufferAsPng(t_string&& strLoc, GLuint iFBOId) {
    GLint iFbBindingLast;
    GLint iFbWidth;
    GLint iFbHeight;

    //Save current FBO
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &iFbBindingLast);

    if (iFBOId == 0) {
        iFBOId = iFbBindingLast;
    }

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindFramebuffer(GL_FRAMEBUFFER, iFBOId);
    {
        //glGetIntegerv(GL_FRAMEBUFFER_DEFAULT_WIDTH, &iFbWidth);
        //glGetIntegerv(GL_FRAMEBUFFER_DEFAULT_HEIGHT, &iFbHeight);
        GLint iViewportBox[4];
        glGetIntegerv(GL_VIEWPORT, (GLint*)iViewportBox);
        iFbWidth = iViewportBox[2];
        iFbHeight = iViewportBox[3];

        //Sanity Values
        AssertOrThrow2(iFbWidth >= 0 && iFbWidth < MemSize::MEMSZ_GIG2);
        AssertOrThrow2(iFbHeight >= 0 && iFbHeight < MemSize::MEMSZ_GIG2);

        std::shared_ptr<Img32> bi = std::make_shared<Img32>(iFbWidth, iFbHeight);
        glReadPixels(0, 0, iFbWidth, iFbHeight, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bi->getData()->ptr());
        bi->flipV(); //the GL tex image must be flipped to show uprih
        Gu::saveImage(strLoc, bi);
    }
    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glBindFramebuffer(GL_FRAMEBUFFER, iFbBindingLast);
}
//void RenderUtils::createDepthTexture(GLuint& __out_ texId, int w, int h, GLenum depthSize){
//    //This will query the device to make sure the depth format is supported.
//    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
//
//    glGenTextures(1, &texId);
//    glBindTexture(GL_TEXTURE_2D, texId);
//    //THe following parameters are for depth textures only
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);//GL_NONE
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
//
//    //**This might fail on phones - change to depth component 24
//    glTexImage2D(GL_TEXTURE_2D, 0, depthSize, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//}
void RenderUtils::createDepthTexture(GLuint* __out_ texId, int32_t w, int32_t h, bool bMsaaEnabled, int32_t nMsaaSamples, GLenum eRequestedDepth) {
    //This will query the device to make sure the depth format is supported.
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    GLenum texTarget;

    if (bMsaaEnabled) {
        texTarget = GL_TEXTURE_2D_MULTISAMPLE;
    }
    else {
        texTarget = GL_TEXTURE_2D;
    }

    glGenTextures(1, texId);
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    glBindTexture(texTarget, *texId);
    //THe following parameters are for depth textures only
    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    if (bMsaaEnabled == false) {
        //For some reason you can't use this with multisample.
        glTexParameteri(texTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);//GL_NONE
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        glTexParameteri(texTarget, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        glTexParameterf(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        glTexParameterf(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
        glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();
    }

    getCompatibleDepthComponent(eRequestedDepth, [&](GLenum eDepth) {
        if (bMsaaEnabled) {
            std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nMsaaSamples, eDepth, w, h, GL_TRUE);
            Gu::checkErrorsRt();
        }
        else {
            glTexImage2D(texTarget, 0, eDepth, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
            Gu::checkErrorsRt();
        }
    });

    Gu::checkErrorsRt();
}

GLenum RenderUtils::getSupportedDepthSize() {
    int32_t depth = Gu::getGraphicsContext()->getSupportedDepthSize();
    //If we don't support requested depth, change it.
    if (depth == 16) {
        return GL_DEPTH_COMPONENT16;
    }
    else if (depth == 24) {
        return GL_DEPTH_COMPONENT24;
    }
    else if (depth == 32) {
        return GL_DEPTH_COMPONENT32F;
        //There is also a 32F component, but .. What/s the difference?
        //eDepthSize = GL_DEPTH_COMPONENT32F;
    }
    else {
        BroThrowException("[222039] Unsupported depth component size " + depth);
    }
}
void RenderUtils::getCompatibleDepthComponent(GLenum eRequestedDepth, std::function<void(GLenum)> func) {
    int32_t depth = Gu::getGraphicsContext()->getSupportedDepthSize();
    //Shortcut lambda to create something that asks for a GL_DEPTH_COMPONENT enum.
    GLenum eDepthSize;
    if (eRequestedDepth == GL_DEPTH_COMPONENT32F && depth < 32) {
        eDepthSize = getSupportedDepthSize();
    }
    else if (eRequestedDepth == GL_DEPTH_COMPONENT32 && depth < 32) {
        eDepthSize = getSupportedDepthSize();
    }
    else if (eRequestedDepth == GL_DEPTH_COMPONENT24 && depth < 24) {
        eDepthSize = getSupportedDepthSize();
    }
    else {
        eDepthSize = eRequestedDepth;
    }

    std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext())->chkErrRt();

    //**This might fail on phones - change to depth component 24
    func(eDepthSize);
    while (true) {
        if (glGetError() != GL_NO_ERROR) {

            if (eDepthSize == GL_DEPTH_COMPONENT32F) {
                BroLogWarn("32 bit floating point depth buffer not supported. Attempting another format.");
                eDepthSize = GL_DEPTH_COMPONENT32;
            }
            else if (eDepthSize == GL_DEPTH_COMPONENT32) {
                BroLogWarn("32 bit depth buffer not supported. Attempting another format.");
                eDepthSize = GL_DEPTH_COMPONENT24;
            }
            else if (eDepthSize == GL_DEPTH_COMPONENT24) {
                BroLogWarn("24 bit floating point depth buffer not supported. Attempting another format.");
                eDepthSize = GL_DEPTH_COMPONENT16;
            }
            else {
                BroThrowException("Creating Depth Texture, No valid depth buffer format could be obtained.");
            }
            //Attempt to use a 24 bit depth buffer
            func(eDepthSize);
        }
        else {
            break;
        }
    }

}


bool RenderUtils::getTextureBindingForTextureId(GLint testTex, GLenum& __out_ target, GLenum& __out_ binding) {
    // GLenum target;
    GLint currentTex;

    glGetIntegerv(binding = GL_TEXTURE_BINDING_1D, &currentTex);
    if (currentTex == testTex) {
        target = GL_TEXTURE_1D;
        return true;
    }
    glGetIntegerv(binding = GL_TEXTURE_BINDING_2D, &currentTex);
    if (currentTex == testTex) {
        target = GL_TEXTURE_2D;
        return true;
    }
    glGetIntegerv(binding = GL_TEXTURE_BINDING_3D, &currentTex);
    if (currentTex == testTex) {
        target = GL_TEXTURE_3D;
        return true;
    }
    glGetIntegerv(binding = GL_TEXTURE_BINDING_CUBE_MAP, &currentTex);
    if (currentTex == testTex) {
        target = GL_TEXTURE_CUBE_MAP;
        return true;
    }
    return false;
}
GLenum RenderUtils::getTexBindingForTexTarget(GLenum eTarget) {
    if (eTarget == GL_TEXTURE_1D) return GL_TEXTURE_BINDING_1D;
    else if (eTarget == GL_TEXTURE_2D) return GL_TEXTURE_BINDING_2D;
    else if (eTarget == GL_TEXTURE_3D) return GL_TEXTURE_BINDING_1D;
    else if (eTarget == GL_TEXTURE_CUBE_MAP) return GL_TEXTURE_BINDING_CUBE_MAP;
    BroThrowException("Fialed to get tex binding for target " + eTarget);
    return 0;
}

bool RenderUtils::getTextureDataFromGpu(std::shared_ptr<Img32> __out_ image, GLuint iGLTexId, GLenum eTexTargetBase, int iCubeMapSide) {
    AssertOrThrow2(image != nullptr);
    GLint iSavedTextureBinding;
    GLenum eTexBinding = getTexBindingForTexTarget(eTexTargetBase);

    GLenum eTexTargetSide = eTexTargetBase;
    if (eTexTargetBase == GL_TEXTURE_CUBE_MAP) {
        AssertOrThrow2(iCubeMapSide >= 0 && iCubeMapSide < 6);
        eTexTargetSide = GL_TEXTURE_CUBE_MAP_POSITIVE_X + iCubeMapSide;
    }

    glGetIntegerv(eTexBinding, &iSavedTextureBinding);
    Gu::checkErrorsRt();

    std::dynamic_pointer_cast<GLContext>(std::dynamic_pointer_cast<GLContext>(Gu::getGraphicsContext()))->glActiveTexture(GL_TEXTURE0);
    glBindTexture(eTexTargetBase, iGLTexId);
    Gu::checkErrorsRt();
    {
        GLint w, h;
        int32_t iMipLevel = 0;
        glGetTexLevelParameteriv(eTexTargetSide, iMipLevel, GL_TEXTURE_WIDTH, &w);
        Gu::checkErrorsRt();

        glGetTexLevelParameteriv(eTexTargetSide, iMipLevel, GL_TEXTURE_HEIGHT, &h);
        Gu::checkErrorsRt();

        //Adding for the pick texture test.
        GLenum internalFormat = 0;
        glGetTexLevelParameteriv(eTexTargetSide, iMipLevel, GL_TEXTURE_INTERNAL_FORMAT, (GLint*)&internalFormat);
        Gu::checkErrorsRt();

        GLenum calculatedFmt = GL_RGBA;
        GLenum calculatedType = GL_UNSIGNED_BYTE;
        size_t bufsiz_bytes = 0;

        if (internalFormat == GL_RGBA) {
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_RGBA8) {
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_RGBA32F) {//All color buffers
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_R32UI) {//Pick buffer
            calculatedType = GL_UNSIGNED_INT;
            calculatedFmt = GL_RED_INTEGER;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_RGBA16F) {
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_R32F) {
            /*
            https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetTexImage.xhtml
            If the selected texture image does not contain four components, the following mappings are applied.
            Single-component textures are treated as RGBA buffers with red set to the single-component value,
            green set to 0, blue set to 0, and alpha set to 1.
            */
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else if (internalFormat == GL_R16F) {
            calculatedFmt = GL_RGBA;
            calculatedType = GL_UNSIGNED_BYTE;
            bufsiz_bytes = w * h * 4;
        }
        else {
            BroLogError("Invalid or Unsupported texture internal format when reading from GPU" +
                (int)internalFormat);
            Gu::debugBreak();
        }

        if (false) {
            //Print all image values as floats (tests to see if buffer was written to
            uint8_t* ts = new uint8_t[w * h * 4];
            size_t iNonZero = 0;
            uint8_t lastr, lastg, lastb, lasta;
            glGetTexImage(eTexTargetSide, iMipLevel, calculatedFmt, calculatedType, (GLvoid*)ts);
            for (GLint ih = 0; ih < h; ++ih) {
                for (GLint iw = 0; iw < w; ++iw) {
                    float r = ts[ih * (w * 4) + iw * 4 + 0];
                    float g = ts[ih * (w * 4) + iw * 4 + 1];
                    float b = ts[ih * (w * 4) + iw * 4 + 2];
                    float a = ts[ih * (w * 4) + iw * 4 + 3];
                    if (lastr != r || lastg != g || lastb != b || lasta != a) {
                        std::cout << " ,(" << r << "," << g << "," << b << "," << a << ")";
                        iNonZero++;
                    }
                    lastr = r; lastg = g; lastb = b; lasta = a;

                }
            }
            int nnn = 0;
            nnn++;
        }

        // char* buf = new char[bufsiz_bytes];
        // glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bi.getData()->ptr());
        //glGetTexImage(GL_TEXTURE_2D, iMipLevel, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bi.getData()->ptr());
        image->init(w, h, nullptr);
        glGetTexImage(eTexTargetSide, iMipLevel, calculatedFmt, calculatedType, (GLvoid*)image->getData()->ptr());
        Gu::checkErrorsRt();
    }
    glBindTexture(eTexTargetBase, iSavedTextureBinding);
    Gu::checkErrorsRt();


    return true;
}





}//ns game