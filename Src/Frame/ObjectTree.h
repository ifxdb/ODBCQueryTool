/* 
  Copyright (C) 2017 Edwig Huisman

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/
#pragma once
#include "SQLInfo.h"
#include <list>

using namespace SQLComponents;

typedef enum treeTypes
{
  TREE_NOTHING = 0  
 ,TREE_TABLES
 ,TREE_PROCEDURES
 ,TREE_TABLE
 ,TREE_COLUMNS
 ,TREE_PRIMARY
 ,TREE_FOREIGN
 ,TREE_STATISTICS
 ,TREE_SPECIALS
 ,TREE_REFERENCEDBY
 ,TREE_PRIVILEGES
 ,TREE_PARAMETERS
}
TreeTypes;

typedef enum objectImage
{
  IMG_TABLES = 0
 ,IMG_TABLE 
 ,IMG_VIEWS
 ,IMG_VIEW
 ,IMG_CATALOGS
 ,IMG_CATALOG
 ,IMG_PROCEDURES
 ,IMG_PROCEDURE
 ,IMG_ALIASSES
 ,IMG_ALIAS
 ,IMG_COLUMN
 ,IMG_PRIMARY
 ,IMG_FOREIGN
 ,IMG_PARAMETER
 ,IMG_INDEX
 ,IMG_INFO
 ,IMG_ACCESS
 ,IMG_SCHEMA
}
ObjectImage;

//using WordList = std::list<CString>;

class ObjectTree: public CTreeCtrl
{
 DECLARE_DYNAMIC(ObjectTree)

public:
  ObjectTree();
 ~ObjectTree();

  void      ClearTree();
  void      SetFilter(CString p_filter);
  void      ExpandFirstTable(CString p_table);
  CString   GetFilter();
  void      CreateImageList();

  HTREEITEM InsertItem(CString p_string,HTREEITEM p_item,int p_data = 0);

private:
  ObjectImage TypeToImage(CString p_type);

protected:
  DECLARE_MESSAGE_MAP();

  void      InsertNoInfo   (HTREEITEM p_item);
  void      RemoveNoInfo   (HTREEITEM p_item);
  void      WordListToTree (WordList& p_list,HTREEITEM p_item,ObjectImage p_image);
  bool      IsSpecialNode  (CString& p_name);
  void      SetItemCount   (HTREEITEM p_theItem,int p_size);
  bool      PresetTable    (HTREEITEM p_theItem);
  bool      PresetProcedure(HTREEITEM p_theItem,WordList& p_list);
  void      FindTables     (HTREEITEM p_theItem);
  void      PrepareTable   (HTREEITEM p_theItem);
  void      FindColumns    (HTREEITEM p_theItem);
  void      FindPrimary    (HTREEITEM p_theItem);
  void      FindForeign    (HTREEITEM p_theItem);
  void      FindStatistics (HTREEITEM p_theItem);
  void      FindSpecials   (HTREEITEM p_theItem);
  void      FindReferenced (HTREEITEM p_theItem);
  void      FindPrivileges (HTREEITEM p_theItem);
  void      FindProcedures (HTREEITEM p_theItem);
  void      FindParameters (HTREEITEM p_theItem);

  CString     m_filter;
  bool        m_busy;
  CImageList  m_imageList;

public:
  afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
};

inline void
ObjectTree::SetFilter(CString p_filter)
{
  m_filter = p_filter;
}

inline CString
ObjectTree::GetFilter()
{
  return m_filter;
}
