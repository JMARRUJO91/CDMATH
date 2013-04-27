// Copyright (C) 2007-2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef __REGIONNODE_HXX__
#define __REGIONNODE_HXX__

#include "MeshRegion.hxx"

namespace INTERP_KERNEL
{

  /**
   * \brief Class containing a tuplet of a source region and a target region. 
   * This is used as the object to put on the stack in the depth-first search
   * in the bounding-box filtering process.
   */
  template<class ConnType>
  class RegionNode
  {
  public:
    
    RegionNode() { }
    
    ~RegionNode() { }
    
    /**
     *  Accessor to source region
     *
     * @return   reference to source region
     */
    MeshRegion<ConnType>& getSrcRegion() { return _srcRegion; }

    /**
     *  Accessor to target region
     *
     * @return   reference to target region
     */
    MeshRegion<ConnType>& getTargetRegion() { return _targetRegion; }

  private:
    
    /// source region
    MeshRegion<ConnType> _srcRegion;          
    
    /// target region
    MeshRegion<ConnType> _targetRegion;       

  };

}

#endif
