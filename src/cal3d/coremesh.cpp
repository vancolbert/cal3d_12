//****************************************************************************//
// coremesh.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/coremesh.h"
#include "cal3d/error.h"
#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"

 /*****************************************************************************/
/** Constructs the core mesh instance.
  *
  * This function is the default constructor of the core mesh instance.
  *****************************************************************************/

CalCoreMesh::CalCoreMesh()
{
}

unsigned int
CalCoreMesh::size()
{
  unsigned int r = sizeof( CalCoreMesh );
  std::vector<CalCoreSubmesh *>::iterator iter1;
  for( iter1 = m_vectorCoreSubmesh.begin(); iter1 != m_vectorCoreSubmesh.end(); ++iter1 ) {
    r += (*iter1)->size();
  }
  return r;
}

 /*****************************************************************************/
/** Destructs the core mesh instance.
  *
  * This function is the destructor of the core mesh instance.
  *****************************************************************************/

CalCoreMesh::~CalCoreMesh()
{
  // destroy all core submeshes
  for(size_t i = 0; i < m_vectorCoreSubmesh.size(); ++i)
  {
    delete m_vectorCoreSubmesh[i];
  }

  m_vectorCoreSubmesh.clear();
}

 /*****************************************************************************/
/** Adds a core submesh.
  *
  * This function adds a core submesh to the core mesh instance.
  *
  * @param pCoreSubmesh A pointer to the core submesh that should be added.
  *
  * @return One of the following values:
  *         \li the assigned submesh \b ID of the added core submesh
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreMesh::addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh)
{
  // get next bone id
  int submeshId;
  submeshId = m_vectorCoreSubmesh.size();

  m_vectorCoreSubmesh.push_back(pCoreSubmesh);

  return submeshId;
}


 /*****************************************************************************/
/** Removes a core submesh.
  *
  * This function removes a core submesh from the core mesh instance.
  *
  * @param submesh ID of the core submesh that should be removed.
  *
  *****************************************************************************/
void CalCoreMesh::removeCoreSubmesh( int submeshID )
{
	m_vectorCoreSubmesh[ submeshID ] = 0;
}


 /*****************************************************************************/
/** Provides access to a core submesh.
  *
  * This function returns the core submesh with the given ID.
  *
  * @param id The ID of the core submesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSubmesh *CalCoreMesh::getCoreSubmesh(int id)
{
  if((id < 0) || (id >= (int)m_vectorCoreSubmesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE_NAME__, __LINE__);
    return 0;
  }

  return m_vectorCoreSubmesh[id];
}

 /*****************************************************************************/
/** Provides access to a core submesh.
  *
  * This function returns the core submesh with the given ID.
  *
  * @param id The ID of the core submesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

const CalCoreSubmesh *CalCoreMesh::getCoreSubmesh(int id) const
{
  if((id < 0) || (id >= (int)m_vectorCoreSubmesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE_NAME__, __LINE__);
    return 0;
  }

  return m_vectorCoreSubmesh[id];
}

 /*****************************************************************************/
/** Returns the number of core submeshes.
  *
  * This function returns the number of core submeshes in the core mesh
  * instance.
  *
  * @return The number of core submeshes.
  *****************************************************************************/

int CalCoreMesh::getCoreSubmeshCount() const
{
  return m_vectorCoreSubmesh.size();
}

 /*****************************************************************************/
/** Returns the core submesh vector.
  *
  * This function returns the vector that contains all core submeshes of the
  * core mesh instance.
  *
  * @return A reference to the core submesh vector.
  *****************************************************************************/

std::vector<CalCoreSubmesh *>& CalCoreMesh::getVectorCoreSubmesh()
{
  return m_vectorCoreSubmesh;
}

 /*****************************************************************************/
/** Returns the core submesh vector.
  *
  * This function returns the vector that contains all core submeshes of the
  * core mesh instance.
  *
  * @return A reference to the core submesh vector.
  *****************************************************************************/

const std::vector<CalCoreSubmesh *>& CalCoreMesh::getVectorCoreSubmesh() const
{
  return m_vectorCoreSubmesh;
}

 /*****************************************************************************/
/** Adds a core submesh.
  *
  * This function adds a core mesh as a blend target.
  * It adds appropriate CalCoreSubMorphTargets to each of the core sub meshes.
  *
  * @param pCoreMesh A pointer to the core mesh that shoulb become a blend target.
  * @param morphTarget A string to be assigned as the morph target's name
  *
  * @return One of the following values:
  *         \li the assigned morph target \b ID of the added blend target
  *         \li \b -1 if an error happened
  *****************************************************************************/

int CalCoreMesh::addAsMorphTarget(CalCoreMesh *pCoreMesh)
{
  //Check if the numbers of vertices allow a blending
  std::vector<CalCoreSubmesh *>& otherVectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();
  //int numsubs = getCoreSubmeshCount();
  //int othernumsubs = pCoreMesh->getCoreSubmeshCount();
  if (m_vectorCoreSubmesh.size() != otherVectorCoreSubmesh.size())
  {
    CalError::setLastError(CalError::INTERNAL, __FILE_NAME__, __LINE__, "This mesh has children with a different numbers of submeshes");
    return -1;
  }
  if (m_vectorCoreSubmesh.size() == 0)
  {
    CalError::setLastError(CalError::INTERNAL, __FILE_NAME__, __LINE__, "Mesh has no submeshes");
    return -1;
  }
  std::vector<CalCoreSubmesh *>::iterator iteratorCoreSubmesh = m_vectorCoreSubmesh.begin();
  std::vector<CalCoreSubmesh *>::iterator otherIteratorCoreSubmesh = otherVectorCoreSubmesh.begin();
  int subMorphTargetID = (*iteratorCoreSubmesh)->getCoreSubMorphTargetCount();
  while(iteratorCoreSubmesh != m_vectorCoreSubmesh.end())
  {
	int count1 = (*iteratorCoreSubmesh)->getVertexCount();
	int count2 = (*otherIteratorCoreSubmesh)->getVertexCount();

    if( count1 != count2 )
    {
		char buf[2048];
		snprintf(buf, sizeof(buf), "This mesh has a morph target child with different number of vertices: %d (%d vs child's %d)", subMorphTargetID, count1, count2);
      CalError::setLastError(CalError::INTERNAL, __FILE_NAME__, __LINE__, buf);
      return -1;
    }
    ++iteratorCoreSubmesh;
    ++otherIteratorCoreSubmesh;
  }
  //Adding the blend targets to each of the core sub meshes
  iteratorCoreSubmesh = m_vectorCoreSubmesh.begin();
  otherIteratorCoreSubmesh = otherVectorCoreSubmesh.begin();
  while(iteratorCoreSubmesh != m_vectorCoreSubmesh.end())
  {
    int vertexCount = (*otherIteratorCoreSubmesh)->getVertexCount();
    CalCoreSubMorphTarget *pCalCoreSubMorphTarget = new(std::nothrow) CalCoreSubMorphTarget();
    if(!pCalCoreSubMorphTarget->reserve(vertexCount)) return -1;
    std::vector<CalCoreSubmesh::Vertex>& vectorVertex = (*otherIteratorCoreSubmesh)->getVectorVertex();
    std::vector<CalCoreSubmesh::Vertex>::iterator iteratorVectorVertex = vectorVertex.begin();
    std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& textCoordVector = (*otherIteratorCoreSubmesh)->getVectorVectorTextureCoordinate();
    std::vector<CalCoreSubmesh::Vertex>& originVectorVertex = (*iteratorCoreSubmesh)->getVectorVertex();
    for(int i = 0;i<vertexCount;++i)
    {
      CalCoreSubMorphTarget::BlendVertex blendVertex;
      ///different than IMVU: blendvertex store only deltas
      blendVertex.position = (*iteratorVectorVertex).position-originVectorVertex[i].position;
      blendVertex.normal = (*iteratorVectorVertex).normal- originVectorVertex[i].normal;
      blendVertex.textureCoords.clear();
      blendVertex.textureCoords.reserve(textCoordVector.size());
      for( unsigned int tcI = 0; tcI < textCoordVector.size(); tcI++ )
      {
        blendVertex.textureCoords.push_back(textCoordVector[tcI][i]);
      }

      if(!pCalCoreSubMorphTarget->setBlendVertex(i,blendVertex)) return -1;

      ++iteratorVectorVertex;
    }
    (*iteratorCoreSubmesh)->addCoreSubMorphTarget(pCalCoreSubMorphTarget);
    ++iteratorCoreSubmesh;
    ++otherIteratorCoreSubmesh;
  }
  ///store morphid as a mesh attribute
  //m_morphTargets[morphTargetName]=subMorphTargetID;
  return subMorphTargetID;
}
 /*****************************************************************************/
/**
  * Set the name of the file in which the core mesh is stored, if any.
  *
  * @param filename The path of the file.
  *****************************************************************************/
  int CalCoreMesh::getMorphTargetId(std::string const & morphTargetName){
    return m_morphTargets[morphTargetName];

  }
 /*****************************************************************************/
/** Scale the Mesh.
  *
  * This function rescale all the data that are in the core mesh instance.
  *
  * @param factor A float with the scale factor
  *
  *****************************************************************************/


void CalCoreMesh::scale(float factor)
{
	std::vector<CalCoreSubmesh *>::iterator iteratorCoreSubmesh;
	for(iteratorCoreSubmesh = m_vectorCoreSubmesh.begin(); iteratorCoreSubmesh != m_vectorCoreSubmesh.end(); ++iteratorCoreSubmesh)
	{
		(*iteratorCoreSubmesh)->scale(factor);
	}
}

 /*****************************************************************************/
/**
  * Set the name of the file in which the core mesh is stored, if any.
  *
  * @param filename The path of the file.
  *****************************************************************************/

void CalCoreMesh::setFilename(const std::string& filename)
{
  m_filename = filename;
}

 /*****************************************************************************/
/**
  * Get the name of the file in which the core mesh is stored, if any.
  *
  * @return One of the following values:
  *         \li \b empty string if the mesh was not stored in a file
  *         \li \b the path of the file
  *
  *****************************************************************************/

const std::string& CalCoreMesh::getFilename(void) const
{
  return m_filename;
}

 /*****************************************************************************/
/**
  * Set the symbolic name of the core mesh.
  *
  * @param name A symbolic name.
  *****************************************************************************/

void CalCoreMesh::setName(const std::string& name)
{
  m_name = name;
}

 /*****************************************************************************/
/**
  * Get the symbolic name the core mesh.
  *
  * @return One of the following values:
  *         \li \b empty string if the mesh was no associated to a symbolic name
  *         \li \b the symbolic name
  *
  *****************************************************************************/

const std::string& CalCoreMesh::getName(void) const
{
  return m_name;
}
