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
#include <hdf5.h>
#include <string.h>

/**\ingroup MEDinterp
  \brief \MEDinterpBaseFunctionRdBrief
  \param fid             \fid
  \param interpname      \interpname
  \param basisfuncit     \basisfuncit
  \param ncoef        \ncoef
  \param power           \power
  \param coefficient     \coefficient
  \return \error
  \details \MEDinterpBaseFunctionRdDetails
  \remarks
  \li \MEDinterpBaseFunctionRdCm1
  \li \MEDinterpBaseFunctionRdCm2
 */

med_err
MEDinterpBaseFunctionRd( const med_idt          fid,
			 const char*      const interpname,
			 const int              basisfuncit,
			       med_int*         ncoef,
			       med_int*   const power,
			       med_float* const coefficient)
{
  med_idt           _interpid=0,_bid=0;
  med_err           _ret=-1;
  char              _path[MED_TAILLE_INTERP+MED_NAME_SIZE+1]=MED_INTERP;
  char              _basisfuncname[MED_MAX_PARA+1]="";
  med_int           _nvariable  =0;
  med_filter        _filter        = MED_FILTER_INIT;

  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();

  if (basisfuncit < 1 ) {
    MED_ERR_(_ret,MED_ERR_RANGE,MED_ERR_PARAMETER,"");
    ISCRUTE_int(basisfuncit);
    goto ERROR;
  }

  NOFINALBLANK(interpname,ERROR);
  strcat(_path,interpname);
  /*
   * Ouverture du DataGroup /INTERP/<interpname>
   */
  if ((_interpid = _MEDdatagroupOpen(fid,_path)) < 0) {
    MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_DATAGROUP,interpname);
    SSCRUTE(_path);goto ERROR;
  }

  /*
   * Lecture de l'attribut MED_NOM_NBV
   */
  if ( _MEDattrEntierLire(_interpid,MED_NOM_NBV,&_nvariable) < 0 ) {
    MED_ERR_(_ret,MED_ERR_READ,MED_ERR_ATTRIBUTE,_path);
    SSCRUTE(MED_NOM_NBV);ISCRUTE(_nvariable);
    goto ERROR;
  }


  sprintf(_basisfuncname,"%0*li",MED_MAX_PARA,(long ) basisfuncit);
  /*
   * Ouverture du DataGroup /INTERP/<interpname>/<basisfuncit>
   */
  if ((_bid = _MEDdatagroupOpen(_interpid,_basisfuncname)) < 0) {
    MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_DATAGROUP,_path);SSCRUTE(_basisfuncname);
    goto ERROR;
  }

  /*
   * Lecture de l'attribut MED_NOM_MCF
   */
  if ( _MEDattrEntierLire(_bid,MED_NOM_MCF,ncoef) < 0 ) {
    MED_ERR_(_ret,MED_ERR_READ,MED_ERR_ATTRIBUTE,_path);SSCRUTE(_basisfuncname);
    SSCRUTE(MED_NOM_MCF);ISCRUTE(*ncoef);
    goto ERROR;
  }

  /*TODO : Créer un _MEDdatasetSimpleWr */
  if ( MEDfilterEntityCr(fid, *ncoef, 1, 1, MED_ALL_CONSTITUENT,
			 MED_NO_INTERLACE,MED_UNDEF_PFLMODE,
			 MED_NO_PROFILE, MED_UNDEF_SIZE, NULL, &_filter) < 0 ) {
    MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_FILTER,MED_ERR_INTERNAL_MSG);
    goto ERROR;
  }

  if ( _MEDdatasetRd(_bid,MED_NOM_COE,MED_INTERNAL_FLOAT64,&_filter, (unsigned char * const) coefficient) < 0) {
    MED_ERR_(_ret,MED_ERR_WRITE,MED_ERR_DATASET,MED_NOM_COE);
    SSCRUTE(_path);SSCRUTE(_basisfuncname);
    goto ERROR;
  }

  if ( MEDfilterClose(&_filter) < 0 ) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILTER,MED_ERR_STRUCT_ELEMENT_MSG);
    SSCRUTE(_path);
    goto ERROR;
  }


  if ( MEDfilterEntityCr(fid, (*ncoef)*_nvariable, 1, 1, MED_ALL_CONSTITUENT,
			 MED_NO_INTERLACE,MED_UNDEF_PFLMODE,
			 MED_NO_PROFILE, MED_UNDEF_SIZE, NULL, &_filter) < 0 ) {
    MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_FILTER,MED_ERR_INTERNAL_MSG);
    goto ERROR;
  }

  if ( _MEDdatasetRd(_bid,MED_NOM_POW,MED_INTERNAL_INT,&_filter, (unsigned char * const) power) < 0) {
    MED_ERR_(_ret,MED_ERR_WRITE,MED_ERR_DATASET,MED_NOM_POW);
    SSCRUTE(_path);SSCRUTE(_basisfuncname);
    goto ERROR;
  }

  if ( MEDfilterClose(&_filter) < 0 ) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILTER,MED_ERR_STRUCT_ELEMENT_MSG);
    SSCRUTE(_path);
    goto ERROR;
  }


  _ret=0;
 ERROR:

  if (_interpid>0)            if (_MEDdatagroupFermer(_interpid) < 0) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_DATAGROUP,_path);
    ISCRUTE_id(_interpid);
  }

  if (_bid>0)            if (_MEDdatagroupFermer(_bid) < 0) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_DATAGROUP,_basisfuncname);
    ISCRUTE_id(_bid);SSCRUTE(_path);
  }

  return _ret;
}
