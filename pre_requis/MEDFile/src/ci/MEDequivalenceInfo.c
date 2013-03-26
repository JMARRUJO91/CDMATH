/*  This file is part of MED.
 *
 *  COPYRIGHT (C) 1999 - 2012  EDF R&D, CEA/DEN
 *  MED is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MED is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MED.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <med.h>
#include <med_config.h>
#include <med_outils.h>

#include <string.h>
#include <stdlib.h>

/**\ingroup MEDequivalence
  \brief \MEDequivalenceInfoBrief
  \param fid \fid
  \param meshname \meshname
  \param equivit \equivit
  \param equivname \equivname
  \param equivdescription \equivdescription
  \param nstep \nstep
  \param nocstpncorrespondence \nocstpncorrespondence
  \retval med_err  \error
  \details \MEDequivalenceInfoDetails
 */

med_err
MEDequivalenceInfo(const med_idt      fid,
		   const char * const meshname,
		   const int          equivit,
		   char * const       equivname,
		   char * const       equivdescription,
		   med_int * const    nstep,
		   med_int * const    nocstpncorrespondence
)
{
  char *  name = "_MEDequivalenceInfo";
  int     dummy=0;
  med_err fret=-1;
  med_int majeur, mineur, release;
  MedFuncType func;

  MEDfileNumVersionRd(fid, &majeur, &mineur, &release);
  func = _MEDversionedApi3(name,majeur,mineur,release);
  if ( func != (MedFuncType) NULL )
    func (dummy,
	  fid,
	  meshname,
	  equivit,
	  equivname,
	  equivdescription,
	  nstep,
	  nocstpncorrespondence,
	  &fret);
  return fret;
}
